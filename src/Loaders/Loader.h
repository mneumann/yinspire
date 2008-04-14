#ifndef __YINSPIRE__LOADER__
#define __YINSPIRE__LOADER__

#include "Core/Simulator.h"
#include "Core/NeuralFactory.h"
#include "Core/NeuralNet.h"
#include <stdio.h>

namespace Yinspire {

  /*
   * Base class of all Loaders. The purpose of a Loader is to construct a neural net
   * from a file.
   */
  class Loader 
  {
    protected:

      Simulator *simulator;
      NeuralFactory *factory;
      NeuralNet *net;

    public:

      Loader(Simulator *simulator, NeuralFactory *factory, NeuralNet *net)
      {
        this->simulator = simulator;
        this->factory = factory;
        this->net = net;
      }

      virtual void load(FILE *fh) = 0;
  };

} /* namespace Yinspire */

#endif
