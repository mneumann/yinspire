#ifndef __YINSPIRE__NEURON_SRM01__
#define __YINSPIRE__NEURON_SRM01__

#include "Models/Neuron_Base.h"

namespace Yinspire {

  /*
   * Formerly known as KernelbasedLIF
   */
  class Neuron_SRM01 : public Neuron_Base
  {
    DECLARE_ENTITY(Neuron_SRM01);

    protected: 

      real tau_m;
      real tau_ref;
      real ref_weight;
      real mem_pot;
      real const_threshold;

    public:

      Neuron_SRM01() :
        tau_m(0.0),
        tau_ref(0.0),
        ref_weight(0.0),
        mem_pot(0.0),
        const_threshold(0.0) {}

      virtual void
        load(Properties &p)
        {
          Neuron_Base::load(p);
          PROP_LOAD(p, tau_m);
          PROP_LOAD(p, tau_ref);
          PROP_LOAD(p, ref_weight);
          PROP_LOAD(p, mem_pot);
          PROP_LOAD(p, const_threshold);
        }

      virtual void
        dump(Properties &p)
        {
          Neuron_Base::dump(p);
          PROP_DUMP(p, tau_m);
          PROP_DUMP(p, tau_ref);
          PROP_DUMP(p, ref_weight);
          PROP_DUMP(p, mem_pot);
          PROP_DUMP(p, const_threshold);
        }

      virtual void
        stimulate(real at, real weight, NeuralEntity *source)
        {

          /*
           * Enqueue stimuli to local priority queue unless in absolute
           * refractory period.
           */
          if (at >= last_fire_time + abs_refr_duration)
          {
            stimuli_add(at, weight);
          }
        }

      virtual void
        process(real at)
        {
          /*
           * Sum up (and remove) all stimuli weights <= at
           */
          real weight = stimuli_sum(at);

          /*
           * Time since end of last absolute refractory period
           */
          real delta = at - last_fire_time - abs_refr_duration; 

          /*
           * Return if still in absolute refractory period
           */
          if (delta < 0.0)
            return;

          /*
           * Calculate new membrane potential
           */
          real decay = exp(-(at - last_spike_time) / tau_m);
          mem_pot = weight + mem_pot * decay;

          /*
           * Update last spike time
           */
          last_spike_time = at;

          /*
           * Calculate dynamic threshold
           */
          real threshold = const_threshold + 
                           ref_weight * exp(-delta/tau_ref);

          /*
           * Fire if potential exceeds threshold
           */
          if (mem_pot >= threshold)
          {
            fire(at, Infinity); 
          }
        }

      inline void
        fire(real at, real weight)
        {
          mem_pot = 0.0;
          last_fire_time = at;
          if (recorder)
            recorder->record_fire(this, at, weight);
          stimulate_synapses(at, weight);
        }

  };

} /* namespace Yinspire */

#endif
