#include "Core/Neuron.h"
#include "Core/Synapse.h"

namespace Yinspire {

  void Neuron::connect(NeuralEntity *target)
  {
    Synapse *syn = dynamic_cast<Synapse*>(target);
    post_synapses.push(syn);
    syn->pre_neuron = this;
  }

  void Neuron::disconnect(NeuralEntity *target)
  {
    Synapse *syn = dynamic_cast<Synapse*>(target);
    post_synapses.remove(syn);
    syn->pre_neuron = NULL;
  }

  void Neuron::each_connection(void (*yield)(NeuralEntity *self, NeuralEntity *conn))
  {
    for (int i=0; i < post_synapses.size(); i++)
    {
      yield(this, post_synapses[i]);
    }
  }

  void Neuron::stimulate_pre_synapses(real at, real weight)
  {
    for (int i=0; i < pre_synapses.size(); i++)
    {
      pre_synapses[i]->stimulate(at, weight, this);
    }
  }

  void Neuron::stimulate_post_synapses(real at, real weight)
  {
    for (int i=0; i < post_synapses.size(); i++)
    {
      post_synapses[i]->stimulate(at, weight, this);
    }
  }

} /* namespace Yinspire */
