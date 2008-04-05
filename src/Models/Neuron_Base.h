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
        stimulate(real at, real weight, NeuralEntity *source)
        {
          simulator()->record_stimuli(at, weight, source);
          stimuli_add(at, weight);
        }

    protected:

      void
        stimulate_synapses(real at, real weight)
        {
          if (hebb) stimulate_pre_synapses(at, weight);
          stimulate_post_synapses(at, weight);
        }

      inline real
        delta()
        {
          return schedule_at - last_fire_time - abs_refr_duration;
        }

   };

} /* namespace Yinspire */

#endif
