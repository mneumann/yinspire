#include "Core/Synapse.h"
#include "Core/Neuron.h"
#include <assert.h>

namespace Yinspire {

  void Synapse::connect(NeuralEntity *target)
  {
    post_neuron = dynamic_cast<Neuron*>(target);
    post_neuron->pre_synapses.push(this);
  }

  void Synapse::disconnect(NeuralEntity *target)
  {
    assert(target == post_neuron);
    dynamic_cast<Neuron*>(target)->pre_synapses.remove(this);
    post_neuron = NULL;
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
