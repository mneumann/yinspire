#ifndef __YINSPIRE__NEURON_OUTPUT__
#define __YINSPIRE__NEURON_OUTPUT__

#include "Models/Neuron_InputOutput.h"

namespace Yinspire {

  /*
   * Output neuron. Used to record fire events.
   */
  class Neuron_Output : public Neuron_InputOutput
  {
    DECLARE_ENTITY(Neuron_Output);

    protected: 

      virtual void
        fire(real at, real weight)
        {
          simulator()->record_output_fire(at, weight, this);
        }

   };

} /* namespace Yinspire */

#endif
