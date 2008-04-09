#ifndef __YINSPIRE__NEURON__
#define __YINSPIRE__NEURON__

#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include "Algorithms/Array.h"

namespace Yinspire {

  /*
   * Base class of all Neurons. Defines the structure that is special
   * for a Neuron, i.e. that a Neuron has pre- and post-Synapses. 
   */
  class Neuron : public NeuralEntity
  {
    protected: 

      Array<NeuralEntity*> pre_synapses;
      Array<NeuralEntity*> post_synapses;

    public:

      virtual void
        connect(NeuralEntity *target, bool forward=true)
        {
          if (forward)
          {
            post_synapses.push(target);
            target->connect(this, false);
          }
          else
          {
            pre_synapses.push(target);
          }
        }

      virtual void
        disconnect(NeuralEntity *target, bool forward=true)
        {
          if (forward)
          {
            post_synapses.remove(target);
            target->disconnect(this, false);
          }
          else
          {
            pre_synapses.remove(target);
          }
        }

      virtual void
        each_incoming_connection(connection_iter yield, void *data)
        {
          for (int i=0; i < pre_synapses.size(); i++)
            yield(this, pre_synapses[i], data);
        }

      virtual void
        each_outgoing_connection(connection_iter yield, void *data)
        {
          for (int i=0; i < post_synapses.size(); i++)
            yield(this, post_synapses[i], data);
        }

    protected:

      void
        stimulate_pre_synapses(real at, real weight)
        {
          for (int i=0; i < pre_synapses.size(); i++)
            pre_synapses[i]->stimulate(at, weight, this);
        }

      void
        stimulate_post_synapses(real at, real weight)
        {
          for (int i=0; i < post_synapses.size(); i++)
            post_synapses[i]->stimulate(at, weight, this);
        }
   };

} /* namespace Yinspire */

#endif
