#include "Core/ScheduleEntity.h"
#include "Core/Scheduler.h"

namespace Yinspire {

  void ScheduleEntity::schedule(real at)
  {
    if (at < schedule_at)
    {
      schedule_at = at;
      scheduler->schedule_update(this);
    }
  }

  bool ScheduleEntity::schedule_stepping_enabled()
  {
    return (schedule_stepping_list_prev != NULL && 
        schedule_stepping_list_next != NULL);
  }

  void ScheduleEntity::schedule_enable_stepping()
  {
    if (!schedule_stepping_enabled())
    {
      ScheduleEntity*& root = scheduler->schedule_stepping_list_root; 
      if (root != NULL)
      {
        schedule_stepping_list_prev = root;
        schedule_stepping_list_next = root->schedule_stepping_list_next;
        root->schedule_stepping_list_next = this; 
        schedule_stepping_list_next->schedule_stepping_list_prev = this; 
      }
      else
      {
        root = this; 
        schedule_stepping_list_prev = this;
        schedule_stepping_list_next = this;
      }
    }
  }

  void ScheduleEntity::schedule_disable_stepping() 
  {
    if (schedule_stepping_enabled())
    {
      if (schedule_stepping_list_prev != schedule_stepping_list_next)
      {
        schedule_stepping_list_prev->schedule_stepping_list_next = 
          schedule_stepping_list_next; 
        schedule_stepping_list_next->schedule_stepping_list_prev = 
          schedule_stepping_list_prev;  
      }
      else
      {
        /*
         * We are the last entity in the stepping list.
         */
        scheduler->schedule_stepping_list_root = NULL;
        schedule_stepping_list_prev = NULL;
        schedule_stepping_list_next = NULL;
      }
    }
  }

} /* namespace Yinspire */
