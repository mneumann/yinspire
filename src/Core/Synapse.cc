#include "Common.h"
#include "Synapse.h"
#include "Neuron.h"

namespace Yinspire {

  void Synapse::connect(NeuralEntity *target)
  {
    dynamic_cast<Neuron*>(target)->pre_synapses.push(this);
    post_neuron = target;
  }

  void Synapse::disconnect(NeuralEntity *target)
  {
    dynamic_cast<Neuron*>(target)->pre_synapses.remove(this);
    post_neuron = NULL;
  }

  /* 
   * Only propagate the stimulation if it doesn't originate from the
   * post Neuron.  Stimuli from a post Neuron are handled by a specific
   * Synapse class (e.g. Hebb).
   * 
   * NOTE: We ignore the weight parameter that is passed by the Neuron.
   */

  void Synapse::stimulate(real at, real weight, NeuralEntity *source)
  {
    if (source != post_neuron)
    {
      post_neuron->stimulate(at + delay, weight, this);
    }
  }

} /* namespace Yinspire */
