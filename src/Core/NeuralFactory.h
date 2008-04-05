#ifndef __YINSPIRE__NEURAL_FACTORY__
#define __YINSPIRE__NEURAL_FACTORY__

#include "Core/NeuralEntity.h"
#include <string>
#include <map>

#define REGISTER_TYPE(factory, name) \
  factory.register_type(name::ctype(), name::create) 
 
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
      std::map<std::string, creator> types;

    public:

      void
        register_type(const std::string &type, creator method)
        {
          types[type] = method;
        }

      /*
       * Create a NeuralEntity of type +type+.  The type must have been
       * registered before using the +register_type+ method.
       */
      NeuralEntity*
        create(const std::string& type)
        {
          return types[type]();
        }

  };

} /* namespace Yinspire */

#endif
