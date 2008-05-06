#ifndef __YINSPIRE__NEURON_INPUTOUTPUT__
#define __YINSPIRE__NEURON_INPUTOUTPUT__

#include "Models/Neuron_Base.h"

namespace Yinspire {

  /*
   * Abstract base class for input and output neurons.
   */
  class Neuron_InputOutput : public Neuron_Base
  {
    public:

      /*
       * Process each stimuli separatly, i.e. DO NOT sum stimuli with
       * the same timestamp.
       */
      virtual void
        process(real at) 
        {
          while (!stimuli_pq.empty() && stimuli_pq.top().at <= at)
          {
            Stimulus s = stimuli_pq.top();
            stimuli_pq.pop();
            if (recorder)
              recorder->record_fire(this, s.at, s.weight);
            fire(s.at, s.weight);
          }

          stimuli_reschedule();
        }

    protected: 

      virtual void
        fire(real at, real weight) = 0;

   };

} /* namespace Yinspire */

#endif
