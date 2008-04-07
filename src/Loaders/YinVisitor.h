#ifndef __YINSPIRE__YIN_VISITOR__
#define __YINSPIRE__YIN_VISITOR__

#include <string>
#include "Algorithms/Array.h"
#include "Core/Properties.h"
#include "Core/Stimulus.h"

namespace Yinspire {

  using namespace std;

  class YinVisitor
  {
    public:

      virtual void on_template(string& id, string& type, Properties &p) {}
      virtual void on_entity(string& id, string& type, Properties &p) {}
      virtual void on_connect(string& from_id, string& to_id) {}
      virtual void on_stimulate(string& id, Stimulus& stimulus) {}

  };

} /* namespace Yinspire */

#endif
