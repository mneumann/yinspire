#ifndef __YINSPIRE__SCHEDULER__
#define __YINSPIRE__SCHEDULER__

#include "Core/Common.h"
#include "Core/ScheduleEntity.h"

namespace Yinspire {

  class Scheduler
  {
    protected: 

      friend class ScheduleEntity;

      /*
       * The current scheduling time.
       */
      real schedule_current_time;

      /*
       * The time step used for stepped scheduling.
       */
      real schedule_step;

      /*
       * The time of the next step.
       */
      real schedule_next_step;

      /*
       * Priority queue used to schedule the entities.
       */
      ScheduleEntity::PQ schedule_pq; 

      /*
       * If stepped scheduling is used, this points to the first entiy in the
       * stepped schedule list.
       */
      ScheduleEntity *schedule_stepping_list_root;

    public:

      /*
       * Constructor
       */
      Scheduler() : 
        schedule_current_time(0.0),
        schedule_step(Infinity),
        schedule_next_step(Infinity),
        schedule_stepping_list_root(NULL) {}

      /*
       * Run the simulation. Returns the time when the simulation stops
       * (schedule_current_time).
       */
      real
        schedule_run(real stop_at=Infinity);

      /*
       * If a ScheduleEntity has changed it's scheduling time, it has to
       * call this method to reflect the change within the priority
       * queue.
       */
      void
        schedule_update(ScheduleEntity *entity);
  };

} /* namespace Yinspire */

#endif
