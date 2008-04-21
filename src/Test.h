#include "Core/Common.h"
#include <strings.h>

namespace Yinspire {

  class Test 
  {
    public:

      bool run() {
        return test_printf_infinity();
      }

      bool test_printf_infinity()
      {
        char buf[20];

        sprintf(buf, "%f", Infinity);
        if (strcasecmp(buf, "inf") != 0 &&
            strcasecmp(buf, "infinity") != 0)
          return false;

        sprintf(buf, "%f", -Infinity);
        if (strcasecmp(buf, "-inf") != 0 &&
            strcasecmp(buf, "-infinity") != 0)
          return false;

        sprintf(buf, "%f", Infinity+Infinity);
        if (strcasecmp(buf, "inf") != 0 &&
            strcasecmp(buf, "infinity") != 0)
          return false;

        sprintf(buf, "%f", Infinity-Infinity);
        if (strcasecmp(buf, "nan") != 0)
          return false;


        return true;
      }

  };

}
