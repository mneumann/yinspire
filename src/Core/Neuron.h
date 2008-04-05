#ifndef __YINSPIRE__NEURON__
#define __YINSPIRE__NEURON__

#include "Core/Common.h"
#include "Core/NeuralEntity.h"
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

      virtual void
        each_incoming_connection(connection_iter yield, void *data);

      virtual void
        each_outgoing_connection(connection_iter yield, void *data);

    protected:

      void
        stimulate_pre_synapses(real at, real weight);

      void
        stimulate_post_synapses(real at, real weight);
   };

} /* namespace Yinspire */

#endif
