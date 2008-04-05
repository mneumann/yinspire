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

  void Synapse::each_connection(void (*yield)(NeuralEntity *self, NeuralEntity *conn))
  {
    yield(this, post_neuron);
  }

} /* namespace Yinspire */
