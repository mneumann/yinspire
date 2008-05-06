#ifndef __YINSPIRE__NEURAL_NET__
#define __YINSPIRE__NEURAL_NET__

#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include <string>
#include <map>

namespace Yinspire {

  /*
   * A NeuralNet is a collection of NeuralEntities.
   */
  class NeuralNet
  {
    protected: 

      /*
       * Stores an "id" -> NeuralEntity mapping.
       */
      map<string, NeuralEntity*> entities;

    public:

      /* 
       * Destructor
       */
      ~NeuralNet()
      {
        destroy_all();
      }

      /*
       * Adds a NeuralEntity to the net.
       */
      void
        add(NeuralEntity *entity)
        {
          if (entities.count(entity->get_id()) == 0)
          {
            entities[entity->get_id()] = entity;
          }
          else
          {
            fail("Duplicate NeuralEntity id: ", entity->get_id());
          }
        }

      NeuralEntity *
        get(const string& id)
        {
          if (entities.count(id) == 0)
            return NULL;
          else
            return entities[id];
        }

      /*
       * Destroy all NeuralEntities contained in the NeuralNet, assuming
       * no other references exist.
       */
      void
        destroy_all()
        {
          for (map<string, NeuralEntity*>::iterator i = entities.begin();
              i != entities.end(); i++)
          {
            delete i->second;
          }
          entities.clear();
        }

      /*
       * Iterates over all entities
       */
      void
        each(void (*yield)(NeuralEntity*, void*), void *data)
        {
          for (map<string, NeuralEntity*>::iterator i = entities.begin();
              i != entities.end(); i++)
          {
            yield(i->second, data);
          }
        }

      uint 
        size()
        {
          return entities.size();
        }

  };

} /* namespace Yinspire */

#endif
