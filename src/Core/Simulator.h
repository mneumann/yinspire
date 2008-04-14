#ifndef __YINSPIRE__SIMULATOR__
#define __YINSPIRE__SIMULATOR__

#include "Core/Common.h"
#include "Core/Scheduler.h"

namespace Yinspire {

  class NeuralEntity; // Forward declaration

  class Simulator : public Scheduler
  {
    public: 

      friend class NeuralEntity;

      /*
       * The tolerance (time difference) up to which local stimuli are
       * accumulated.
       */
      real stimuli_tolerance;

    public:

      Simulator() :
        stimuli_tolerance(0.0) {}

#ifdef YINSPIRE__EXPENSIVE_RECORD
      virtual void
        record_stimulate(NeuralEntity *origin, real at, real weight, NeuralEntity *source) {}

      virtual void
        record_process(NeuralEntity *origin, real at) {}
#endif

      virtual void
        record_fire(real at, real weight, NeuralEntity *source) {}

      virtual void
        record_output_fire(real at, real weight, NeuralEntity *source) {}

      void
        run(real stop_at=Infinity)
        {
          schedule_run(stop_at);
        }
   };

} /* namespace Yinspire */

#endif
