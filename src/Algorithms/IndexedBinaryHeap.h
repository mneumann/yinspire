/*
 * An Indexed Binary Heap
 *
 * Copyright (c) 2007, 2008 by Michael Neumann (mneumann@ntecs.de)
 *
 * The Indexed Binary Heap keeps track of the indices of it's elements
 * stored in the heap to allow efficient updating of their priorities. 
 *
 * The requirement was to modify an elements priority. In a regular
 * implicit binary heap this is an inefficient operation as the element
 * has to be found prior to modifying it's priority.  And finding an
 * element is O(n) in an implicit binary heap due to it's non sorted
 * nature (i.e. can't apply binary search). By keeping track of the
 * elements index and storing this value inside the elements structure,
 * the complexity of modifying an elements priority is reduced to 
 * O(log n) in the worst-case! 
 *
 * NOTE: Index 0 of the elements array is unused.  It's the index that
 * should be used to denote that an element is NOT actually present in
 * the binary heap.
 *
 * See documentation of BinaryHeap as well.
 *
 * Example:
 *
 *   struct E
 *   {
 *     float schedule_at;
 *     unsigned int schedule_index;
 *
 *     inline static bool less(const E& e1, const E& e2)
 *     {
 *       return e1.schedule_at < e2.schedule_at;
 *     }
 *
 *     inline static unsigned int& index(const E& e)
 *     {
 *       return e.schedule_index;
 *     }
 *   };
 *
 *   IndexedBinaryHeap<E, MemoryAllocator> heap;
 *   ...
 *
 */

#ifndef __YINSPIRE__INDEXED_BINARY_HEAP__
#define __YINSPIRE__INDEXED_BINARY_HEAP__

#include "BinaryHeap.h"

template <typename E, class Acc=E>
struct BinaryHeapIndexer
{
  static inline void index_changed(E& e, unsigned int i) 
  {
    Acc::index(e) = i;
  }
};


template <typename E, class Alloc, class Acc=E, unsigned int MIN_CAPA=1024> 
class IndexedBinaryHeap : public BinaryHeap<E, Alloc, Acc, BinaryHeapIndexer<E, Acc>, MIN_CAPA>
{
    typedef unsigned int I; // index type
    typedef BinaryHeap<E, Alloc, Acc, BinaryHeapIndexer<E, Acc>, MIN_CAPA> super;
    typedef BinaryHeapIndexer<E, Acc> Idx;

  public:

    void
      update(const E& element)
      {
        I i = Acc::index(element);
        if (i == 0)
        {
          super::push(element);
        }
        else
        {
          // FIXME: use propagate up/down instead
          Idx::index_changed(this->elements[i], 0);  // detach from heap
          I bubble = super::move_bubble_down(i);
          super::insert_and_bubble_up(bubble, element); 
        }
      }

};

#endif
