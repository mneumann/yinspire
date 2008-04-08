#ifndef __YINSPIRE__COMMON__
#define __YINSPIRE__COMMON__

#define YINSPIRE_VERSION "1.0"

#include "Allocators/MemoryAllocator.h"
#include <stdlib.h>
#include <cmath>
#include <limits>
#include <string>

namespace Yinspire
{
  using namespace std;

  typedef unsigned int uint;
  typedef float real;
  const real Infinity = numeric_limits<real>::infinity();

  inline void fail(const string& msg1)
  {
    throw(msg1);
  }

  inline void fail(const string& msg1, const string& msg2)
  {
    throw(msg1 + msg2);
  }

};

#endif
