#ifndef __YINSPIRE__NEURON_SRM02__
#define __YINSPIRE__NEURON_SRM02__

#include "Common.h"
#include "Neuron_Base.h"

namespace Yinspire {

  class Neuron_SRM01 : public Neuron_Base
  {
    protected: 

      real tau_m;
      real tau_ref;
      real ref_weight;
      real mem_pot;
      real const_threshold;

    public:

      /*
       * Constructor
       */
      Neuron_SRM01() :
        tau_m(0.0),
        tau_ref(0.0),
        ref_weight(0.0),
        mem_pot(0.0),
        const_threshold(0.0) {}

      virtual void
        stimulate(real at, real weight, NeuralEntity *source)
        {
          if (at >= last_fire_time + abs_refr_duration)
          {
            //simulator()->count_event();
            Neuron_Base::stimulate(at, weight, source);
          }
        }

      virtual void
        process()
        {
          real weight = stimuli_sum(schedule_at);

          /*
           * Calculate new membrane potential
           */
          mem_pot = weight + mem_pot * real_exp(-(schedule_at - last_spike_time)/tau_m);
          last_spike_time = schedule_at;

          if (schedule_at >= last_fire_time + abs_refr_duration &&
              mem_pot >= const_threshold + dynamic_reset())
          {
            fire(schedule_at, Infinity);
          }
        }

      inline void
        fire(real at, real weight)
        {
          if (abs_refr_duration > 0.0)
          {
            schedule(at + abs_refr_duration);
          }

          if (isinf(mem_pot))
          {
            mem_pot = 0.0;
            reset = u_reset;
          }
          else
          {
            reset = dynamic_reset() + u_reset;
          }
          last_fire_time = at;

          simulator()->record_fire(at, weight, this);
          stimulate_synapses(at, weight);
        }

      inline real
        delta()
        {
          return schedule_at - last_fire_time - abs_refr_duration;
        }

      inline real
        dynamic_reset()
        {
          return reset * real_exp(-delta()/tau_ref);
        }
  };

} /* namespace Yinspire */

#endif
