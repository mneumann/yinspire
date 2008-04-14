#ifndef __YINSPIRE__NEURON_BASE__
#define __YINSPIRE__NEURON_BASE__

#include "Core/Neuron.h"

namespace Yinspire {

  class Neuron_Base : public Neuron
  {
    protected: 

      /*
       * Duration of the absolute refraction period.
       */
      real abs_refr_duration;

      /*
       * Last spike time.
       *
       * "-Infinity" means "never".
       */
      real last_spike_time;

      /*
       * Last fire time.
       *
       * "-Infinity" means "never".
       */
      real last_fire_time;

      /*
       * Whether this neuron is a hebb neuron or not. A hebb neuron also
       * stimulates it's pre synapses upon firing.
       */
      bool hebb;

    public:

      Neuron_Base() :
        abs_refr_duration(0.0),
        last_spike_time(-Infinity),
        last_fire_time(-Infinity),
        hebb(false) {}

      virtual void
        load(Properties &p)
        {
          PROP_LOAD(p, abs_refr_duration);
          PROP_LOAD(p, last_spike_time);
          PROP_LOAD(p, last_fire_time);
          PROP_LOAD(p, hebb);
        }

      virtual void
        dump(Properties &p)
        {
          PROP_DUMP(p, abs_refr_duration);
          PROP_DUMP(p, last_spike_time);
          PROP_DUMP(p, last_fire_time);
          PROP_DUMP(p, hebb);
        }

      virtual void
        stimulate(real at, real weight, NeuralEntity *source)
        {
#ifdef YINSPIRE__EXPENSIVE_RECORD
          simulator()->record_stimulate(this, at, weight, source);
#endif
          stimuli_add(at, weight);
        }

      virtual void
        process(real at)
        {
#ifdef YINSPIRE__EXPENSIVE_RECORD
          simulator()->record_process(this, at);
#endif
        }

    protected:

      void
        stimulate_synapses(real at, real weight)
        {
          if (hebb) stimulate_pre_synapses(at, weight);
          stimulate_post_synapses(at, weight);
        }

   };

} /* namespace Yinspire */

#endif
