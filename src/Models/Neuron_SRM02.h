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

      Neuron_SRM02() :
        tau_m(0.0),
        tau_ref(0.0),
        reset(0.0),
        u_reset(0.0),
        mem_pot(0.0),
        const_threshold(0.0) {}

      virtual void
        load(Properties &p)
        {
          Neuron_Base::load(p);
          PROP_LOAD(p, tau_m);
          PROP_LOAD(p, tau_ref);
          PROP_LOAD(p, reset);
          PROP_LOAD(p, u_reset);
          PROP_LOAD(p, mem_pot);
          PROP_LOAD(p, const_threshold);
        }

      virtual void
        dump(Properties &p)
        {
          Neuron_Base::dump(p);
          PROP_DUMP(p, tau_m);
          PROP_DUMP(p, tau_ref);
          PROP_DUMP(p, reset);
          PROP_DUMP(p, u_reset);
          PROP_DUMP(p, mem_pot);
          PROP_DUMP(p, const_threshold);
        }

      virtual void
        process(real at)
        {
          real weight = stimuli_sum(at);
          const real delta = at - last_fire_time - abs_refr_duration; 

          /*
           * Calculate new membrane potential
           */
          mem_pot = weight + mem_pot * exp(-(at - last_spike_time)/tau_m);
          last_spike_time = at;

          if (delta < 0.0)
            return;

          /*
           * Calculate dynamic reset
           */
          const real dynamic_reset = reset * exp(-delta/tau_ref);

          if (mem_pot >= const_threshold + dynamic_reset)
          {
            fire(at, Infinity, dynamic_reset);
          }

          Neuron_Base::process(at);
        }

      inline void
        fire(real at, real weight, real dynamic_reset)
        {
          if (abs_refr_duration > 0.0 &&
              at + abs_refr_duration < schedule_at)
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
          if (recorder)
            recorder->record_fire(this, at, weight);
          stimulate_synapses(at, weight);
        }

  };

} /* namespace Yinspire */

#endif
