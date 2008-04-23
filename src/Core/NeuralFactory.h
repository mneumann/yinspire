#ifndef __YINSPIRE__NEURAL_FACTORY__
#define __YINSPIRE__NEURAL_FACTORY__

#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include "Core/Scheduler.h"
#include "Core/Recorder.h"
#include "Core/Properties.h"
#include <string>
#include <map>

#define REGISTER_TYPE(factory, name) \
  factory.register_type(name::ctype(), name::create) 

#define REGISTER_TYPE_ALIAS(factory, alias_name, name) \
  factory.register_type(#alias_name, name::create) 
 
namespace Yinspire {

  /*
   * A NeuralFactory eases creation of NeuralEntities.
   */
  class NeuralFactory
  {
    protected: 

      typedef NeuralEntity* (*creator)(); 

      /*
       * Stores an "type" -> creation method mapping.
       */
      map<string, creator> types;

      /*
       * Default settings for new created NeuralEntities.
       */
      Scheduler *default_scheduler;
      Recorder *default_recorder;

    public:

      NeuralFactory(Scheduler *scheduler=NULL, Recorder *recorder=NULL) :
        default_scheduler(scheduler), default_recorder(recorder) {}

      Scheduler *get_default_scheduler() { return default_scheduler; }
      Recorder *get_default_recorder() { return default_recorder; }
      void set_default_scheduler(Scheduler *scheduler) { default_scheduler = scheduler; }
      void set_default_recorder(Recorder *recorder) { default_recorder = recorder; }

      void
        register_type(const string &type, creator method)
        {
          types[type] = method;
        }

      /*
       * Create a NeuralEntity of type +type+.  The type must have been
       * registered before using the +register_type+ method.
       */
      NeuralEntity*
        create(const string& type)
        {
          if (types.count(type) > 0)
          {
            NeuralEntity *n = types[type]();
            if (n)
            {
              n->set_scheduler(default_scheduler);
              n->set_recorder(default_recorder);
            }
            return n;
          }
          else
          {
            return NULL;
          }
        }

      NeuralEntity*
        create(const string& type, const string& id)
        {
          NeuralEntity *n = create(type);
          if (n)
          {
            n->set_id(id);
          }
          return n;
        }

      NeuralEntity*
        create(const string& type, const string& id, Properties& p)
        {
          NeuralEntity *n = create(type);
          if (n)
          {
            n->set_id(id);
            n->load(p);
          }
          return n;
        }

  };

} /* namespace Yinspire */

#endif
