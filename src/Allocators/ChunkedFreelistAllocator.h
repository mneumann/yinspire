/*
 * A chunked freelist Allocator
 *
 * Copyright (c) 2007, 2008 by Michael Neumann (mneumann@ntecs.de)
 *
 * Memory is allocated in chunks of size +chunk_size+. Chunks are never
 * freed except when the destructor is called.
 */

#ifndef __YINSPIRE__CHUNKED_FREELIST_ALLOCATOR__
#define __YINSPIRE__CHUNKED_FREELIST_ALLOCATOR__

#include <assert.h>

template <class T, class ACC = T>
class ChunkedFreelistAllocator
{

    template <class TT>
    struct Chunk
    {
      Chunk<TT> *next_chunk;
      TT *array; 
    };

  public:

    ChunkedFreelistAllocator(unsigned int chunksize)
    {
      this->freelist = NULL;
      this->chunklist = NULL;
      this->chunksize = chunksize;
    }

    T*
      allocate()
      {

        // alloc new chunk if no more free elements are available
        if (this->freelist == NULL) alloc_chunk();

        assert(this->freelist != NULL);

        T* e = this->freelist; 
        this->freelist = ACC::next(e);
        ACC::next(e) = NULL;

        return e;
      }

    void
      free(T* e)
      {
        //assert(ACC::next(e) == NULL);
        ACC::next(e) = this->freelist;
        this->freelist = e;
      }

    void
      free_list(T* first, T* last)
      {
        assert(last != NULL);
        //assert(ACC::next(first) == NULL);
        ACC::next(last) = this->freelist; 
        this->freelist = first;
      }

  protected:

    void
      alloc_chunk()
      {
        Chunk<T> *new_chunk = new Chunk<T>;
        new_chunk->next_chunk = this->chunklist;
        this->chunklist = new_chunk;

        new_chunk->array = new T[this->chunksize]; 

        // put all elements of new chunk on freelist
        for (unsigned int i=0; i<this->chunksize-1; i++)
        {
          ACC::next(&new_chunk->array[i]) = &new_chunk->array[i+1];
        }

        ACC::next(&new_chunk->array[this->chunksize-1]) = this->freelist;
        this->freelist = &new_chunk->array[0];
      }

  private:

    T *freelist;
    Chunk<T> *chunklist;
    unsigned int chunksize;
};

#endif
