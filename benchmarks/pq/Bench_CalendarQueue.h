#include "Algorithms/CalendarQueue.h"
#include "Allocators/ChunkedFreelistAllocator.h"

namespace BenchCalendarQueue
{
  template<class ET>
  struct T
  {
    typedef CalendarQueue<ET> PQ;

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
        ET *e = pq->pop();
        e->_priority += increment;
        pq->push(e);
      }

      inline void push(PQ *pq, double priority)
      {
        ET *e = this->freelist->allocate();
        e->_priority = priority;
        pq->push(e);
      }

      inline void pop(PQ *pq)
      {
        this->freelist->free(pq->pop());
      }

      inline double pop_return_priority(PQ *pq)
      {
        ET *e = pq->pop();
        double res = e->_priority; 
        this->freelist->free(e);
        return res;
      }

      static const char* algorithm_name()
      {
        return "CalendarQueue";
      }
    };
  };
};
