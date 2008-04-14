#ifndef __YINSPIRE__DUMPER_YIN__
#define __YINSPIRE__DUMPER_YIN__

#include "Dumpers/Dumper.h"
#include "Core/NeuralNet.h"

namespace Yinspire {

  class Dumper_Yin : public Dumper
  {
    public:

      Dumper_Yin(NeuralNet *net) :
        Dumper(net) {}

      virtual void dump(FILE *fh);

    protected:

      static void dump_entity(NeuralEntity *entity, void *data);
      static void dump_connections(NeuralEntity *entity, void *data);
      static void dump_connection(NeuralEntity *self, NeuralEntity *conn, void *data);
      static void dump_stimuli(NeuralEntity *entity, void *data);
      static void dump_stimulus(const Stimulus&, void *data);
      
  };

} /* namespace Yinspire */

#endif
