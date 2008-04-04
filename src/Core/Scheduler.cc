#include "Core/Scheduler.h"
#include <algorithm> // min()

namespace Yinspire {

  void Scheduler::schedule_run(real stop_at)
  {
    while (true)
    {
      real next_stop = std::min(stop_at, schedule_next_step);

      /* 
       * Calculate all events from the priority queue until the next time
       * step is reached.
       */
      while (!schedule_pq.empty())
      {
        ScheduleEntity *top = schedule_pq.top();
        if (top->schedule_at >= next_stop)
          break;
        schedule_current_time = top->schedule_at; 
        schedule_pq.pop();
        top->process();
      }

      if (schedule_current_time >= stop_at)
        break;

      if (schedule_stepping_list_root == NULL && schedule_pq.empty())
        break;

      /* 
       * Calculate the entities that require stepped processing.
       */ 
      schedule_current_time = schedule_next_step; 

      if (schedule_stepping_list_root != NULL)
      {
        // FIXME: collect all entities in an array. then process them.
      }

      schedule_next_step += schedule_step;
    }
  }

  void Scheduler::schedule_update(ScheduleEntity *entity)
  {
    schedule_pq.update(entity);
  }

} /* namespace Yinspire */
