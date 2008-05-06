#ifndef __YINSPIRE__LOADER__
#define __YINSPIRE__LOADER__

#include "Core/Common.h"
#include "Core/Scheduler.h"
#include "Core/NeuralFactory.h"
#include "Core/NeuralNet.h"

namespace Yinspire {

  /*
   * Base class of all Loaders. The purpose of a Loader is to construct a neural net
   * from a file.
   */
  class Loader 
  {
    protected:

      NeuralFactory *factory;
      NeuralNet *net;

    public:

      Loader(NeuralFactory *factory, NeuralNet *net)
      {
        this->factory = factory;
        this->net = net;
      }

      virtual void load(FILE *fh) = 0;
  };

} /* namespace Yinspire */

#endif
