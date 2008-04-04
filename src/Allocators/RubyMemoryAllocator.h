#ifndef __YINSPIRE__RUBY_MEMORY_ALLOCATOR__
#define __YINSPIRE__RUBY_MEMORY_ALLOCATOR__

#include "ruby.h"
#include <stdlib.h>

/*
 * Provides some basic utility functions for allocating and releasing
 * memory.
 */
template <typename T> 
class MemoryAllocator
{
  public:

    inline static T*
      alloc_n(size_t n)
      {
        return ALLOC_N(T, n);
      }

    inline static T*
      realloc_n(T* old_ptr, size_t n)
      {
        REALLOC_N(old_ptr, T, n);
        return old_ptr;
      }

    inline static void
      free(T* ptr)
      {
        ::free(ptr);
      }

};

#endif
