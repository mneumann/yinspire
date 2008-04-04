#include "Common.h"
#include "Neuron.h"
#include "Synapse.h"

namespace Yinspire {

  void Neuron::connect(NeuralEntity *target)
  {
    post_synapses.push(target);
    dynamic_cast<Synapse*>(target)->pre_neuron = this;
  }

  void Neuron::disconnect(NeuralEntity *target)
  {
    post_synapses.remove(target);
    dynamic_cast<Synapse*>(target)->pre_neuron = NULL;
  }

  void Neuron::stimulate_pre_synapses(real at, real weight)
  {
    for (int i=0; i < pre_synapses.size(); i++)
      pre_synapses[i]->stimulate(at, weight);
  }

  void Neuron::stimulate_post_synapses(real at, real weight)
  {
    for (int i=0; i < post_synapses.size(); i++)
      post_synapses[i]->stimulate(at, weight);
  }

} /* namespace Yinspire */
