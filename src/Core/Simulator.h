#ifndef __YINSPIRE__SIMULATOR__
#define __YINSPIRE__SIMULATOR__

#include "Core/Common.h"
#include "Core/Scheduler.h"

namespace Yinspire {

  class NeuralEntity; // Forward declaration

  class Simulator : public Scheduler
  {
    public:

      Simulator() {}

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
