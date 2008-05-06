#include "Core/Scheduler.h"
#include <algorithm> // min()

namespace Yinspire {

  real Scheduler::schedule_run(real stop_at)
  {
    real current = schedule_current_time;
    while (true)
    {
      real next_stop = min(stop_at, schedule_next_step);

      /* 
       * Calculate all events from the priority queue until the next time
       * step is reached.
       */
      while (!schedule_pq.empty())
      {
        ScheduleEntity *top = schedule_pq.top();
        current = top->schedule_at; 
        if (current > next_stop)
          break;
        schedule_pq.pop();
        top->schedule_at = Infinity; 
        top->process(schedule_current_time = current);
      }

      if (schedule_stepping_list_root == NULL && schedule_pq.empty())
        break;

      if (current > stop_at)
        break;

      /* 
       * Calculate the entities that require stepped processing.
       */ 
      current = schedule_current_time = schedule_next_step; 

      if (schedule_stepping_list_root != NULL)
      {
        // FIXME: collect all entities in an array. then process them.
      }

      schedule_next_step += schedule_step;
    }

    return schedule_current_time;
  }

  void Scheduler::schedule_update(ScheduleEntity *entity)
  {
    schedule_pq.update(entity);
  }

} /* namespace Yinspire */
