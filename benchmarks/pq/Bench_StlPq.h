#include <vector>
#include <queue>

namespace BenchStlPq
{
  template<class ET>
  struct T
  {
    typedef std::priority_queue<ET, std::vector<ET>, ET> PQ; 

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
        return "std::priority_queue";
      }
    };
  };
};
