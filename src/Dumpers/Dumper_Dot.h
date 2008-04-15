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

      virtual void dump(FILE *fh);

  };

} /* namespace Yinspire */

#endif
