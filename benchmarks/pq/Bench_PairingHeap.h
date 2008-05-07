#include "Algorithms/PairingHeap.h"
#include "Allocators/ChunkedFreelistAllocator.h"

namespace BenchPairingHeap
{
  template<class ET>
  struct T
  {
    typedef PairingHeap<ET> PQ;

    struct ACC
    {
      ChunkedFreelistAllocator<ET> *freelist;

      ACC()
      {
        this->freelist = new ChunkedFreelistAllocator<ET>(100000);
      }

      ~ACC()
      {
        delete this->freelist;
      }

      inline void hold(PQ *pq, double increment)
      {
        ET *e = pq->top();
        e->priority += increment;
        pq->pop();
        pq->push(e);
      }

      inline void push(PQ *pq, double priority)
      {
        ET *e = this->freelist->allocate();
        e->priority = priority;
        pq->push(e);
      }

      inline void pop(PQ *pq)
      {
        this->freelist->free(pq->top());
        pq->pop();
      }

      inline double pop_return_priority(PQ *pq)
      {
        ET *e = pq->top();
        double res = e->priority; 
        pq->pop();
        this->freelist->free(e);
        return res;
      }

      static const char* algorithm_name()
      {
        return "PairingHeap";
      }
    };
  };
};
