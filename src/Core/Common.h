#ifndef __YINSPIRE__COMMON__
#define __YINSPIRE__COMMON__

#include "Allocators/MemoryAllocator.h"
#include <stdlib.h>
#include <math.h>
#define Infinity INFINITY 
#include <string>

namespace Yinspire
{
  typedef unsigned int uint;
  typedef float real;

  inline real real_exp(real x) { return expf(x); }

  inline void fail(const std::string& msg1)
  {
    throw(msg1);
  }

  inline void fail(const std::string& msg1, const std::string& msg2)
  {
    throw(msg1 + msg2);
  }

};

#endif
