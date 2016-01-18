#ifndef __YINSPIRE__SIMULATOR__
#define __YINSPIRE__SIMULATOR__

#include "Core/Common.h"
#include "Core/NeuralNet.h"
#include "Core/Scheduler.h"
#include "Core/NeuralFactory.h"
#include "Core/Recorder.h"
#include "Loaders/Loader_Yin.h"

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

      Simulator(Recorder *recorder=NULL, bool register_wrapper=false);

      ~Simulator();

      void
        set_default_recorder(Recorder *recorder);

      NeuralFactory &
        get_factory();

      real
        run(real stop_at=Infinity);

      void
        load_yin(FILE *fh);

      void
        load_yin(const char *filename);

      void
        dump_yin(FILE *fh);

      void
        dump_yin(const char *filename);

      void
        dump_dot(FILE *fh);

      void
        dump_dot(const char *filename);

      void
        dump_gml(FILE *fh);

      void
        dump_gml(const char *filename);

      bool
        test();

      NeuralEntity *
        get_entity(const string &id);

      NeuralEntity *
        create_entity(const string& type, const string &id);

      void
        add_entity(NeuralEntity *entity);

      void
        each_entity(void (*yield)(NeuralEntity*, void*), void *data);

      uint
        num_entities();

      public:

      /*
       * Helper methods for opening and closing files
       */

      FILE *
        open_in(const char *fname);

      FILE *
        open_out(const char *fname);

      void
        close_inout(FILE *fh);

  };

} /* namespace Yinspire */

#endif
