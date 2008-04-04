#ifndef __YINSPIRE__SCHEDULE_ENTITY__
#define __YINSPIRE__SCHEDULE_ENTITY__

#include "Core/Common.h"
#include "Algorithms/IndexedBinaryHeap.h"

namespace Yinspire {

  class Scheduler; // Forward reference

  class ScheduleEntity
  {
    protected: 

      friend class Scheduler;

      /*
       * Each ScheduleEntity has a pointer to the Scheduler.  This is used
       * for example to update it's scheduling or to report a fire event.
       */
      Scheduler *scheduler;

      /*
       * Index of this entity in the entity priority queue managed by the
       * Simulator. If schedule_index is zero then the entity is currently
       * not present in the priority queue and as such the entity is not
       * scheduled for a specific time.
       */
      uint schedule_index;

      /*
       * The timestamp of the earliest event in the local priority queue.
       */
      real schedule_at;

      /*
       * If stepped scheduling is used, points to the previous/next entity
       * in the schedule list.
       */
      ScheduleEntity *schedule_stepping_list_prev;
      ScheduleEntity *schedule_stepping_list_next;

      /*
       * To be able to modify the stepped scheduling list
       * (schedule_stepping_list_prev/next) during stepped schedule
       * processing, we build up an internal linked list that we use to
       * traverse all entities that require stepped schedule processing. 
       * 
       * This is cheaper than using an externalized linked list, as we
       * would have to allocate memory, which we overcome with this
       * approach.
       *
       * This is only used by the Scheduler!
       */
      ScheduleEntity *schedule_stepping_list_internal_next;

    public:

      /*
       * Constructor
       */
      ScheduleEntity() : 
        scheduler(NULL),
        schedule_index(0),
        schedule_at(Infinity),
        schedule_stepping_list_prev(NULL),
        schedule_stepping_list_next(NULL) {}

      /*
       * Destructor
       */
      virtual
        ~ScheduleEntity() {}

      /*
       * This method is called when a ScheduleEntity reached it's scheduling
       * time and should "do something".
       *
       * Overwrite it if you need this behaviour.
       */
      virtual void
        process() {} 

      /*
       * This method is called in each time-step, if a ScheduleEntity
       * uses stepped scheduling. 
       *
       * Overwrite it if you need this behaviour.
       */
      virtual void
        process_stepped(real at, real step) {}

    protected:

      /*
       * Schedules the entity at a specific time.
       */
      void
        schedule(real at);

      /*
       * Returns +true+ if stepped scheduling is enabled, +false+
       * otherwise.
       */
      bool schedule_stepping_enabled();

      /* 
       * Enable stepped scheduling. 
       */
      void
        schedule_enable_stepping();

      /* 
       * Disable stepped scheduling. 
       */
      void
        schedule_disable_stepping();

    public:

      /*
       * Accessor function for BinaryHeap
       */
      inline static bool
        less(const ScheduleEntity *a, const ScheduleEntity *b) 
        {
          return (a->schedule_at < b->schedule_at);
        }

      /*
       * Accessor function for BinaryHeap
       */
      inline static uint &
        index(ScheduleEntity *self)
        {
          return self->schedule_index;
        }

      typedef IndexedBinaryHeap<ScheduleEntity*, 
              MemoryAllocator<ScheduleEntity*>, ScheduleEntity> PQ;
  };

} /* namespace Yinspire */

#endif
