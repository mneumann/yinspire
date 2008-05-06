#ifndef __YINSPIRE__DUMPER__
#define __YINSPIRE__DUMPER__

#include "Core/Common.h"
#include "Core/NeuralNet.h"

namespace Yinspire {

  /*
   * Base class of all Dumpers. The purpose of a Dumper is to 
   * dump a neural net to a file.
   */
  class Dumper 
  {
    protected:

      NeuralNet *net;

    public:

      Dumper(NeuralNet *net)
      {
        this->net = net;
      }

      virtual void dump(FILE *fh) = 0;
  };

} /* namespace Yinspire */

#endif
