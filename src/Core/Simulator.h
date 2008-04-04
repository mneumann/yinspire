#ifndef __YINSPIRE__SIMULATOR__
#define __YINSPIRE__SIMULATOR__

#include "Common.h"
#include "Scheduler.h"

namespace Yinspire {

  class Simulator : public Scheduler
  {
    protected: 

      friend class NeuralEntity;

      /*
       * The tolerance (time difference) up to which local stimuli are
       * accumulated.
       */
      real stimuli_tolerance;

    public:

      Simulator() :
        stimuli_tolerance(Infinity) {}

      virtual void
        record_fire(real at, real weight, NeuralEntity *source) {};

      void
        run(real stop_at=Infinity)
        {
          schedule_run(stop_at);
        }
   };

} /* namespace Yinspire */

#endif
