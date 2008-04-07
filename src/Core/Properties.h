#ifndef __YINSPIRE__PROPERTIES__
#define __YINSPIRE__PROPERTIES__

#include "Core/Common.h"
#include <string>
#include <map>
#include <ostream>

#define PROP_LOAD(properties, var) \
  properties.load(var, #var)

#define PROP_DUMP(properties, var) \
  properties.dump(var, #var)

namespace Yinspire {

  enum {Property_Real, Property_Bool};
  struct Property
  {
    int type;
    union {
      real real_value;
      bool bool_value;
    } value;
  };

  /*
   * This class is used in NeuralEntity#load and #dump
   * methods.
   */
  class Properties
  {
    protected: 

      std::map<std::string, Property> properties;

    public:

      void
        load(real &variable, const std::string &name)
        {
          if (properties.count(name) == 0)
            return;
          Property &prop = properties[name]; 
          if (prop.type != Property_Real)
            throw "Invalid Property Type";
          variable = prop.value.real_value;
        }

      void
        load(bool &variable, const std::string &name)
        {
          if (properties.count(name) == 0)
            return;
          Property &prop = properties[name]; 
          if (prop.type != Property_Bool)
            throw "Invalid Property Type";
          variable = prop.value.bool_value;
        }

      void
        dump(real variable, const std::string &name)
        {
          Property prop;
          prop.type = Property_Real;
          prop.value.real_value = variable; 
          properties[name] = prop; 
        }

      void
        dump(bool variable, const std::string &name)
        {
          Property prop;
          prop.type = Property_Bool;
          prop.value.bool_value = variable; 
          properties[name] = prop; 
        }

      /*
       * Erases all properties.
       */
      void
        clear()
        {
          properties.clear();
        }

      /*
       * Overwrite properties from +p+.
       */
      void
        update(Properties &p)
        {
          using namespace std;
          for (map<string, Property>::iterator i=p.properties.begin();
              i != p.properties.end(); i++)
          {
            properties[i->first] = i->second;   
          }
        }

      void
        output(std::ostream &s)
        {
          using namespace std;
          for (map<string, Property>::iterator i=properties.begin();
              i != properties.end(); i++)
          {
            s << i->first << " = "; 
            switch (i->second.type)
            {
              case Property_Real:
                s << i->second.value.real_value;
                break;
              case Property_Bool:
                s << i->second.value.bool_value;
                break;
              default:
                throw "Invalid Property Type";
            };
            s << endl;
          }
        }
  };

} /* namespace Yinspire */

#endif
