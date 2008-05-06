#ifndef __YINSPIRE__COMMON__
#define __YINSPIRE__COMMON__

#define YINSPIRE_VERSION "1.0"

#include "Allocators/MemoryAllocator.h"
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <limits>
#include <string>

namespace Yinspire
{
  using namespace std;

  typedef unsigned int uint;

#ifdef YINSPIRE_DOUBLE_PRECISION
  typedef double real;
#else
  typedef float real;
#endif

  const real Infinity = numeric_limits<real>::infinity();

#ifdef WIN32
  inline bool isinf(real n) { return !_finite(n) && !_isnan(n); }
  inline bool isnan(real n) { return _isnan(n); }
#endif

  inline void fprint_real(FILE *f, real n)
  {
    if (isinf(n))
    {
      if (n < 0.0)
        fprintf(f, "%s", "-Infinity");
      else
        fprintf(f, "%s", "Infinity");
    }
    else if (isnan(n))
    {
      fprintf(f, "%s", "NaN");
    }
    else
    {
      fprintf(f, "%g", n); 
    }
  }

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
