#ifndef __YINSPIRE__SIMULATOR__
#define __YINSPIRE__SIMULATOR__

#include "Core/Common.h"
#include "Core/Recorder.h"
#include "Core/NeuralNet.h"
#include "Core/NeuralFactory.h"
#include "Core/Properties.h"
#include "Loaders/Loader_Yin.h"
#include "Dumpers/Dumper_Yin.h"
#include "Dumpers/Dumper_Dot.h"
#include "RegisterTypes.h"
#include "Test.h"

namespace Yinspire {

  /*
   * Implement an easy to use collection of all the core classes
   * required to setup and run a simulation.
   */
  class Simulator 
  {
    protected:

      NeuralNet net;
      Scheduler scheduler;
      NeuralFactory factory;
      Loader_Yin *loader_yin;

    public:

      Simulator(Recorder *recorder=NULL)
      { 
        factory.set_default_recorder(recorder);
        factory.set_default_scheduler(&scheduler);
        RegisterTypes(factory);
        loader_yin = new Loader_Yin(&factory, &net);
      }

      ~Simulator()
      {
        delete loader_yin;
      }

      void
        set_default_recorder(Recorder *recorder)
        {
          factory.set_default_recorder(recorder);
        }

      real
        run(real stop_at=Infinity)
        {
          scheduler.schedule_run(stop_at);
        }

      void
        load_yin(FILE *fh)
        {
          loader_yin->load(fh);
        }

      void
        dump_yin(FILE *fh)
        {
          Dumper_Yin dumper(&net);
          dumper.dump(fh);
        }

      void
        dump_dot(FILE *fh)
        {
          Dumper_Dot dumper(&net);
          dumper.dump(fh);
        }

      bool
        test()
        {
          /*
           * Run some tests to ensure correct program behaviour.
           */
          Test test;
          return test.run();
        }
  };

} /* namespace Yinspire */

#endif
