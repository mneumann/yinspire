#ifndef __YINSPIRE__SYNAPSE_BASE__
#define __YINSPIRE__SYNAPSE_BASE__

#include "Core/Synapse.h"

namespace Yinspire {

  class Synapse_Base : public Synapse
  {
    protected: 

      /*
       * The fire weight of the synapse.
       */
      real weight;

      /*
       * The propagation delay of the synapse.
       */
      real delay;

    public:

      Synapse_Base() :
        weight(0.0),
        delay(0.0) {}

      /* 
       * Only propagate the stimulation if it doesn't originate from the
       * post-neuron. Stimuli from a post-neuron are handled by a specific
       * Synapse class (e.g. Hebb).
       * 
       * NOTE: We ignore the weight parameter that is passed by the neuron.
       */
      virtual void 
        stimulate(real at, real weight, NeuralEntity *source)
        {
          if (source != post_neuron)
          {
            post_neuron->stimulate(at + delay, weight, this);
          }
        }

   };

} /* namespace Yinspire */

#endif
