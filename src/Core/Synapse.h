#ifndef __YINSPIRE__SYNAPSE__
#define __YINSPIRE__SYNAPSE__

#include "Core/Common.h"
#include "Core/NeuralEntity.h"

namespace Yinspire {

  class Neuron; // Forward declaration 

  /*
   * Base class of all Synapses. Defines the structure that is special
   * for a Synapse, i.e. that a Synapse has a pre and a post-Neuron.
   * Also each Synapse has a +weight+ and a +delay+.
   */
  class Synapse : public NeuralEntity
  {
    protected: 

      friend class Neuron;

      Neuron *pre_neuron;
      Neuron *post_neuron;

      /*
       * The fire weight of a Synapse.
       */
      real weight;

      /*
       * The propagation delay of a Synapse.
       */
      real delay;

    public:

      /*
       * Constructor.
       */
      Synapse() :
        pre_neuron(NULL),
        post_neuron(NULL),
        weight(0.0),
        delay(0.0) {}

      /*
       * Adding a pre synapse. Target must be a Neuron.
       */
      virtual void
        connect(NeuralEntity *target);

      virtual void
        disconnect(NeuralEntity *target);

      virtual void
        stimulate(real at, real weight, NeuralEntity *source);

  };

} /* namespace Yinspire */

#endif
