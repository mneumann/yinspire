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

      inline real get_weight() const {
          return weight;
      }

      inline real get_delay() const {
          return delay;
      }

      Synapse_Base() :
        weight(0.0),
        delay(0.0) {}

      virtual void
        load(Properties &p)
        {
          PROP_LOAD(p, weight);
          PROP_LOAD(p, delay);
        }

      virtual void
        dump(Properties &p)
        {
          PROP_DUMP(p, weight);
          PROP_DUMP(p, delay);
        }

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
            /*
             * Synapse fires with it's own weight, not the weight passed
             * by the Neuron.
             */
            post_neuron->stimulate(at + delay, this->weight, this);
          }
        }

   };

} /* namespace Yinspire */

#endif
