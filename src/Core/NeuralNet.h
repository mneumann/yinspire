#ifndef __YINSPIRE__NEURAL_NET__
#define __YINSPIRE__NEURAL_NET__

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
          // TODO: Throw exception in case of a duplicate id.
          entities[entity->id()] = entity;
        }

      NeuralEntity *
        get(const std::string& id)
        {
          return entities[id];
        }

  };

} /* namespace Yinspire */

#endif
