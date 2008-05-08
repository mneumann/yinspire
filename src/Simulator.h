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

      Simulator(Recorder *recorder=NULL, bool register_wrapper=false)
      { 
        factory.set_default_recorder(recorder);
        factory.set_default_scheduler(&scheduler);
        RegisterTypes(factory, register_wrapper);
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

      NeuralFactory &
        get_factory()
        {
          return factory;
        }

      real
        run(real stop_at=Infinity)
        {
          return scheduler.schedule_run(stop_at);
        }

      void
        load_yin(FILE *fh)
        {
          loader_yin->load(fh);
        }

      void
        load_yin(const char *filename)
        {
          FILE *fh = open_in(filename);
          load_yin(fh);
          close_inout(fh);
        }

      void
        dump_yin(FILE *fh)
        {
          Dumper_Yin dumper(&net);
          dumper.dump(fh);
        }

      void
        dump_yin(const char *filename)
        {
          FILE *fh = open_out(filename);
          dump_yin(fh);
          close_inout(fh);
        }

      void
        dump_dot(FILE *fh)
        {
          Dumper_Dot dumper(&net);
          dumper.dump(fh);
        }

      void
        dump_dot(const char *filename)
        {
          FILE *fh = open_out(filename);
          dump_dot(fh);
          close_inout(fh);
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

      NeuralEntity *
        get_entity(const string &id)
        {
          return net.get(id);
        }

      NeuralEntity *
        create_entity(const string& type, const string &id)
        {
          return factory.create(type, id);
        }

      void
        add_entity(NeuralEntity *entity)
        {
          net.add(entity);
        }

      void
        each_entity(void (*yield)(NeuralEntity*, void*), void *data)
        {
          net.each(yield, data);
        }

      uint
        num_entities()
        {
          return net.size();
        }

      public:

      /*
       * Helper methods for opening and closing files
       */

      FILE *
        open_in(const char *fname)
        {
          FILE *fh = NULL;

          if (strcmp(fname, "-") == 0)
            fh = stdin;
          else
            fh = fopen(fname, "r");

          if (fh == NULL)
            fail("Couldn't open file: ", fname);

          return fh;
        }

      FILE *
        open_out(const char *fname)
        {
          FILE *fh = NULL;

          if (strcmp(fname, "-") == 0)
            fh = stdout;
          else
            fh = fopen(fname, "w+");

          if (fh == NULL)
            fail("Couldn't open file: ", fname);

          return fh;
        }

      void
        close_inout(FILE *fh)
        {
          if (fh != stdin && fh != stdout && fh != stderr)
            fclose(fh);
        }

  };

} /* namespace Yinspire */

#endif
