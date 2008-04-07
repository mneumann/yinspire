#include "Core/Synapse.h"
#include "Core/Neuron.h"
#include <assert.h>

namespace Yinspire {

  void Synapse::connect(NeuralEntity *target)
  {
    Neuron *n = dynamic_cast<Neuron*>(target);
    if (n)
    {
      post_neuron = n;
      post_neuron->pre_synapses.push(this);
    }
    else
    {
      fail("Synapse::connect failed for ", target->id());
    }
  }

  void Synapse::disconnect(NeuralEntity *target)
  {
    assert(target == post_neuron);
    Neuron *n = dynamic_cast<Neuron*>(target);
    if (n)
    {
      n->pre_synapses.remove(this);
      post_neuron = NULL;
    }
    else
    {
      fail("Synapse::disconnect failed for ", target->id());
    }
  }

  void Synapse::each_incoming_connection(connection_iter yield, void *data)
  {
    yield(this, pre_neuron, data);
  }

  void Synapse::each_outgoing_connection(connection_iter yield, void *data)
  {
    yield(this, post_neuron, data);
  }

} /* namespace Yinspire */
