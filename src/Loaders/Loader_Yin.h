#ifndef __YINSPIRE__LOADER_YIN__
#define __YINSPIRE__LOADER_YIN__

#include <string>
#include <map>
#include "Core/Properties.h"
#include "Loaders/Loader.h"
#include "Loaders/YinVisitor.h"

namespace Yinspire {

  class Loader_Yin : public Loader, public YinVisitor
  {
    protected:

      // template type -> (type, Properties) mapping
      map<string, pair<string, Properties> > templates;

    public:

      Loader_Yin(NeuralFactory *factory, NeuralNet *net) :
        Loader(factory, net) {}

      virtual void load(FILE *fh);

      /* 
       * YinVisitor interface
       */
      virtual void on_template(string& id, string& type, Properties &p);
      virtual void on_entity(string& id, string& type, Properties &p);
      virtual void on_connect(string& from_id, string& to_id);
      virtual void on_stimulate(string& id, Stimulus& s);

    protected:

      void create_entity(string& id, string& type, Properties &p);
  };

} /* namespace Yinspire */

#endif
