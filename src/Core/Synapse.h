#ifndef __YINSPIRE__SYNAPSE__
#define __YINSPIRE__SYNAPSE__

#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include <assert.h>

namespace Yinspire {

  /*
   * Base class of all Synapses. Defines the structure that is special
   * for a Synapse, i.e. that a Synapse has a pre and a post-Neuron.
   */
  class Synapse : public NeuralEntity
  {
    protected:

      NeuralEntity *pre_neuron;
      NeuralEntity *post_neuron;

    public:

      inline NeuralEntity *get_pre_neuron() const {
          return pre_neuron;
      }

      inline NeuralEntity *get_post_neuron() const {
          return post_neuron;
      }

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
        connect(NeuralEntity *target, bool forward=true)
        {
          if (forward)
          {
            assert(post_neuron == NULL);
            post_neuron = target;
            target->connect(this, false);
          }
          else
          {
            assert(pre_neuron == NULL);
            pre_neuron = target;
          }
        }

      virtual void
        disconnect(NeuralEntity *target, bool forward=true)
        {
          if (forward)
          {
            assert(target == post_neuron);
            post_neuron = NULL;
            target->disconnect(this, false);
          }
          else
          {
            assert(target == pre_neuron);
            pre_neuron = NULL;
          }
        }

      virtual void
        each_incoming_connection(connection_iter yield, void *data)
        {
          yield(this, pre_neuron, data);
        }

      virtual void
        each_outgoing_connection(connection_iter yield, void *data)
        {
          yield(this, post_neuron, data);
        }

  };

} /* namespace Yinspire */

#endif
