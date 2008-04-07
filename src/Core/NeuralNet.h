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
      std::map<std::string, NeuralEntity*> entities;

    public:

      /*
       * Adds a NeuralEntity to the net.
       */
      void
        add(NeuralEntity *entity)
        {
          if (entities.count(entity->id()) == 0)
          {
            entities[entity->id()] = entity;
          }
          else
          {
            throw "Duplicate NeuralEntity id";
          }
        }

      NeuralEntity *
        get(const std::string& id)
        {
          if (entities.count(id) == 0)
            return NULL;
          else
            return entities[id];
        }

  };

} /* namespace Yinspire */

#endif
