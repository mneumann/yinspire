#ifndef __YINSPIRE__DUMPER_DOT__
#define __YINSPIRE__DUMPER_DOT__

#include "Dumpers/Dumper.h"
#include "Core/NeuralNet.h"

namespace Yinspire {

  class Dumper_Dot : public Dumper
  {
    public:

      Dumper_Dot(NeuralNet *net) :
        Dumper(net) {}

      virtual void dump(const char* filename);

    protected:

      static void dump_entity(NeuralEntity *entity, void *data);
      static void dump_connection(NeuralEntity *self, NeuralEntity *conn, void *data);
      
  };

} /* namespace Yinspire */

#endif
