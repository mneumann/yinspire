#ifndef __YINSPIRE__STIMULUS__
#define __YINSPIRE__STIMULUS__

#include "Core/Common.h"
#include "Algorithms/BinaryHeap.h"

namespace Yinspire {

  /*
   * The data structure used for storing a fire impluse or any other form
   * of stimulation.
   */
  struct Stimulus
  {
    real at;
    real weight;

    inline static bool
      less(const Stimulus &a, const Stimulus &b)
      {
        return (a.at < b.at); 
      }

    typedef BinaryHeap<Stimulus, MemoryAllocator<Stimulus> > PQ;
  };

} /* namespace Yinspire */

#endif
