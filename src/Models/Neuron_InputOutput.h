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
        process() 
        {
          Stimulus s;

          while (!stimuli_pq.empty() && stimuli_pq.top().at <= schedule_at)
          {
            s = stimuli_pq.top();
            stimuli_pq.pop();
            simulator()->record_fire(s.at, s.weight, this);
            fire(s.at, s.weight);
          }

          /*
           * NOTE: we don't have to remove the entity from the schedule if the
           * pq is empty.
           */
          if (!stimuli_pq.empty())
          {
            schedule(stimuli_pq.top().at);
          }
        }

    protected: 

      virtual void
        fire(real at, real weight) = 0;

   };

} /* namespace Yinspire */

#endif
