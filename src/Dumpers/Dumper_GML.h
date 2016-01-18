#pragma once

#include "Dumpers/Dumper.h"
#include "Core/NeuralNet.h"

namespace Yinspire {

  class Dumper_GML : public Dumper
  {
    public:

      Dumper_GML(NeuralNet *net) :
        Dumper(net) {}

      virtual void dump(FILE *fh);

  };

} /* namespace Yinspire */
