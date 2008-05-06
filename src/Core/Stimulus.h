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

    Stimulus() {}
    Stimulus(real _at, real _weight) : at(_at), weight(_weight) {}

    void
      output(FILE *f) const
      {
        if (!isinf(weight))
        {
          fprint_real(f, weight);
          fprintf(f, "\t@\t");
        }
        fprint_real(f, at);
        fprintf(f, "\n");
      }
  };

} /* namespace Yinspire */

#endif
