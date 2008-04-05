#ifndef __YINSPIRE__NEURON_INPUT__
#define __YINSPIRE__NEURON_INPUT__

#include "Models/Neuron_InputOutput.h"

namespace Yinspire {

  /*
   * Input neuron. Simply forwards stimuli.
   */
  class Neuron_Input : public Neuron_InputOutput
  {
    DECLARE_ENTITY(Neuron_Input);

    protected: 

      virtual void
        fire(real at, real weight)
        {
          stimulate_synapses(at, weight);
        }

   };

} /* namespace Yinspire */

#endif
