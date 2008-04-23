#ifndef __YINSPIRE__RECORDER__
#define __YINSPIRE__RECORDER__

#include "Core/Common.h"

namespace Yinspire {

  class NeuralEntity; // Forward declaration

  class Recorder
  {
    public:

      Recorder() {}

      /*
       * All the record_before_xxx and record_after_xxx methods are
       * called before respectively after the NeuralEntity#xxx method.
       *
       * They are useful for debugging purposes etc. but not for real
       * simulations, as they are very expensive.
       */

      virtual void
        record_before_stimulate(NeuralEntity *origin, real at, real weight, NeuralEntity *source) {}
      virtual void
        record_after_stimulate(NeuralEntity *origin, real at, real weight, NeuralEntity *source) {}
      virtual void
        record_before_process(NeuralEntity *origin, real at) {}
      virtual void
        record_after_process(NeuralEntity *origin, real at) {}
      virtual void
        record_before_process_stepped(NeuralEntity *origin, real at, real step) {}
      virtual void
        record_after_process_stepped(NeuralEntity *origin, real at, real step) {}

      virtual void
        record_fire(NeuralEntity *origin, real at, real weight) {}

   };

} /* namespace Yinspire */

#endif
