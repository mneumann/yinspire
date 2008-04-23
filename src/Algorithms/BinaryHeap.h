/*
 * An implicit Binary Heap.
 *
 * Copyright (c) 2007, 2008 by Michael Neumann (mneumann@ntecs.de)
 *
 * NOTE: We start counting from 1 in the elements array!
 *
 * Template parameters:
 *
 *   E:        Element type
 *   Alloc:    Allocator
 *   Acc:      Accessor struct. Defines ordering relation (less).
 *   MIN_CAPA: minimum number of elements
 *
 * Example:
 *
 *   struct Acc
 *   {
 *     static inline bool less(const int& a, const int& b)
 *     {
 *       return a < b;
 *     }
 *   };
 *
 *   BinaryHeap<int, MemoryAllocator, Acc> heap;
 *   heap.push(4);
 *   heap.pop();
 *   ...
 *
 */

#ifndef __YINSPIRE__BINARY_HEAP__
#define __YINSPIRE__BINARY_HEAP__

#include <assert.h>

/*
 * This is used to be able to keep track of
 * an elements index in the IndexedBinaryHeap subclass.
 * Unused in this class.
 */
template <typename E>
struct BinaryHeapDummyIndexer
{
  static inline void index_changed(E& e, unsigned int i) 
  {
    /* DUMMY */
  }
};

template <typename E, class Alloc, class Acc=E, class Idx=BinaryHeapDummyIndexer<E>, 
         unsigned int MIN_CAPA=16>
class BinaryHeap
{
    typedef unsigned int I; // index type

  public:

      BinaryHeap()
      {
        this->capacity = 0;
        this->size_ = 0;
        this->elements = NULL; // we do lazy allocation!
      }

      ~BinaryHeap()
      {
        if (this->elements != NULL)
        {
          Alloc::free(this->elements+1);
        }
        this->elements = NULL;
      }

    inline E&
      top() const
      {
        assert(this->size_ > 0);
        return this->elements[1];  
      }

    void
      pop()
      {
        remove(1);
      }

    inline void
      remove(I i)
      {
        assert(i <= this->size_);

        // 
        // Element i is removed from the heap and as such becomes
        // a "bubble" (free element). Move the bubble until
        // the bubble becomes a leaf element. 
        //
        Idx::index_changed(this->elements[i], 0);  // detach from heap
        I bubble = move_bubble_down(i);

        //
        // Now take the last element and insert it at the position of
        // the bubble. In case the bubble is already the last element we
        // are done.
        //
        if (bubble != this->size_)
        {
          insert_and_bubble_up(bubble, this->elements[this->size_]);
        }
        --this->size_;
      }

    void
      push(const E& element)
      {
        if (this->size_ >= this->capacity) resize(2*this->capacity);
        insert_and_bubble_up(++this->size_, element);
      }

    inline I
      size() const
      {
        return this->size_;
      }

    inline bool
      empty() const
      {
        return (this->size_ == 0);
      }
    
    /*
     * Returns NULL or a pointer to the parent of +element+.
     */
    E*
      find_parent(const E& element)
      {
        I i;

        //
        // Find the position of the first element that is less than +element+.
        // 
        for (i = this->size_; i != 0 && Acc::less(element, this->elements[i]); i /= 2);

        return (i == 0 ? NULL : &this->elements[i]); 
      }

    /*
     * Iterate over all elements (non-destructive)
     */
    void
      each(void (*yield)(const E&, void*), void *data)
      {
        for (I i=1; i <= this->size_; i++)
        {
          yield(this->elements[i], data);
        }
      }
 
  protected:

    /*
     * Insert +element+ into the heap beginning from
     * +i+ and searching upwards to the root for the 
     * right position (heap ordered) to insert.
     *
     * Element at index +i+ MUST be empty, i.e. unused!
     */
    inline void
      insert_and_bubble_up(I i, const E& element)
      {
        for (;i >= 2 && Acc::less(element, this->elements[i/2]); i /= 2)
        {
          store_element(i, this->elements[i/2]);
        }

        // finally store it into the determined hole
        store_element(i, element);
      }

    /*
     * Move the bubble (empty element) at +i+ down in direction
     * to the leaves. When the bubble reaches a leaf, stop and
     * return the index of the leaf element which is now empty.
     */
    inline I 
      move_bubble_down(I i)
      {
        const I sz = this->size_;
        I right_child = i * 2 + 1;

        while (right_child <= sz) 
        {
          if (Acc::less(this->elements[right_child-1], this->elements[right_child]))
          {
            --right_child; // minimum child is left child
          }

          store_element(i, this->elements[right_child]);
          i = right_child;
          right_child = i * 2 + 1;
        }

        //
        // Edge case (comparison with the last element)
        //
        if (right_child-1 == sz)
        {
          store_element(i, this->elements[right_child-1]);
          i = right_child-1;
        }

        return i;
      }

    /*
     * The 0'th element is never used (accessed), so 
     * we allocate only "capacity" elements (instead of capacity+1)
     * and move the pointer one element before the begin of the 
     * allocated memory. 
     */
    void
      resize(I new_capacity)
      {
        E *new_elements;

        if (new_capacity < MIN_CAPA) this->capacity = MIN_CAPA;  
        else this->capacity = new_capacity;

        //
        // We do lazy allocation!
        //
        if (this->elements != NULL)
        {
          new_elements = Alloc::realloc_n(this->elements+1, this->capacity);
        }
        else
        {
          new_elements = Alloc::alloc_n(this->capacity);
        }

        assert(new_elements != NULL);
        assert(this->capacity >= this->size_);

        //
        // move pointer so that we "introduce" a zero'th 
        // element.
        //
        this->elements = new_elements-1;
      }

    inline void
      store_element(I i, const E& element)
      {
        this->elements[i] = element;
        Idx::index_changed(this->elements[i], i); 
      }

  protected:

    I  size_;
    E *elements;
    I  capacity;
};

#endif
