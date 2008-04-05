#ifndef __YINSPIRE__SYNAPSE__
#define __YINSPIRE__SYNAPSE__

#include "Core/Common.h"
#include "Core/NeuralEntity.h"

namespace Yinspire {

  class Neuron; // Forward declaration 

  /*
   * Base class of all Synapses. Defines the structure that is special
   * for a Synapse, i.e. that a Synapse has a pre and a post-Neuron.
   */
  class Synapse : public NeuralEntity
  {
    protected: 

      friend class Neuron;

      Neuron *pre_neuron;
      Neuron *post_neuron;

    public:

      /*
       * Constructor.
       */
      Synapse() :
        pre_neuron(NULL),
        post_neuron(NULL) {}

      /*
       * Adding a pre synapse. Target must be a Neuron.
       */
      virtual void
        connect(NeuralEntity *target);

      virtual void
        disconnect(NeuralEntity *target);

      virtual void
        each_incoming_connection(connection_iter yield, void *data);

      virtual void
        each_outgoing_connection(connection_iter yield, void *data);

  };

} /* namespace Yinspire */

#endif
