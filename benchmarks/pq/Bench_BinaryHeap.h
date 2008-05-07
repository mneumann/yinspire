#include "Algorithms/BinaryHeap.h"
#include "Allocators/MemoryAllocator.h"

namespace BenchBinaryHeap
{
  template<class ET>
  struct T
  {
    typedef BinaryHeap<ET, MemoryAllocator<ET> > PQ;

    struct ACC
    {
      inline void hold(PQ *pq, double increment)
      {
        ET e = pq->top();
        e.priority += increment;
        pq->pop();
        pq->push(e);
      }

      inline void push(PQ *pq, double priority)
      {
        ET e;
        e.priority = priority;
        pq->push(e);
      }

      inline void pop(PQ *pq)
      {
        pq->pop();
      }

      inline double pop_return_priority(PQ *pq)
      {
        ET e = pq->top();
        pq->pop();
        return e.priority;
      }

      static const char* algorithm_name()
      {
        return "BinaryHeap";
      }
    };
  };
};
