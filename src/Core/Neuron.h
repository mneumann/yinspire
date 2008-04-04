#ifndef __YINSPIRE__NEURON__
#define __YINSPIRE__NEURON__

#include "NeuralEntity.h"
#include "Algorithms/Array.h"

namespace Yinspire {

  class Synapse; // Forward declaration

  /*
   * Base class of all Neurons. Defines the structure that is special
   * for a Neuron, i.e. that a Neuron has pre- and post-Synapses. 
   */
  class Neuron : public NeuralEntity
  {
    protected: 

      friend class Synapse;

      Array<Synapse*> pre_synapses;
      Array<Synapse*> post_synapses;

    public:

      virtual void
        connect(NeuralEntity *target);

      virtual void
        disconnect(NeuralEntity *target);

    protected:

      void
        stimulate_pre_synapses(real at, real weight);

      void
        stimulate_post_synapses(real at, real weight);
   };

} /* namespace Yinspire */

#endif
