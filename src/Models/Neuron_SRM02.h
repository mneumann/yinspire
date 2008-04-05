#ifndef __YINSPIRE__NEURON_SRM02__
#define __YINSPIRE__NEURON_SRM02__

#include "Models/Neuron_Base.h"

namespace Yinspire {

  /*
   * Formerly known as SpecialEKernel
   */
  class Neuron_SRM02 : public Neuron_Base
  {
    DECLARE_ENTITY(Neuron_SRM02);

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

      virtual void
        process()
        {
          real weight = stimuli_sum(schedule_at);

          /*
           * Calculate new membrane potential
           */
          mem_pot = weight + mem_pot * real_exp(-(schedule_at - last_spike_time)/tau_m);
          last_spike_time = schedule_at;

          if (schedule_at < last_fire_time + abs_refr_duration)
            return;

          /*
           * Calculate dynamic reset
           */
          const real dynamic_reset = reset * real_exp(-delta()/tau_ref);

          if (mem_pot >= const_threshold + dynamic_reset)
          {
            fire(schedule_at, Infinity, dynamic_reset);
          }
        }

      inline void
        fire(real at, real weight, real dynamic_reset)
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
            reset = dynamic_reset + u_reset;
          }

          last_fire_time = at;
          simulator()->record_fire(at, weight, this);
          stimulate_synapses(at, weight);
        }

  };

} /* namespace Yinspire */

#endif
