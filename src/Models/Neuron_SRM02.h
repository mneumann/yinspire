#ifndef __YINSPIRE__NEURON_SRM02__
#define __YINSPIRE__NEURON_SRM02__

#include "Common.h"
#include "Neuron_Base.h"

namespace Yinspire {

  class Neuron_SRM02 : public Neuron_Base
  {
    protected: 

      real tau_m;
      real tau_ref;
      real reset;
      real u_reset;
      real mem_pot;
      real const_threshold;

    public:

      /*
       * Constructor
       */
      Neuron_SRM02() :
        tau_m(0.0),
        tau_ref(0.0),
        reset(0.0),
        u_reset(0.0),
        mem_pot(0.0),
        const_threshold(0.0) {}

      // FIXME: move into Neuron_Base
      virtual void
        stimulate(real at, real weight, NeuralEntity *source)
        {
          //simulator()->count_event();
          Neuron_Base::stimulate(at, weight, source);
        }

      virtual void
        process(real at)
        {
          real weight = stimuli_sum(at);

          /*
           * Calculate new membrane potential
           */
          mem_pot = weight + mem_pot * real_exp(-(at - last_spike_time)/tau_m);
          last_spike_time = at;

          if (at >= last_fire_time + abs_refr_duration &&
              mem_pot >= const_threshold + dynamic_reset())
          {
            fire(at, Infinity);
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
          return at - last_fire_time - abs_refr_duration;
        }

      inline real
        dynamic_reset()
        {
          return reset * real_exp(-delta()/tau_ref);
        }
  };

} /* namespace Yinspire */

#endif
