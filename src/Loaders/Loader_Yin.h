#ifndef __YINSPIRE__LOADER_YIN__
#define __YINSPIRE__LOADER_YIN__

#include "Core/Loader.h"

namespace Yinspire {

  class Loader_Yin : public Loader
  {
    public:

      Loader_Yin(Simulator *simulator, NeuralFactory *factory, NeuralNet *net) :
        Loader(simulator, factory, net) {}

      virtual void load(const char* filename);
  };

} /* namespace Yinspire */

#endif
