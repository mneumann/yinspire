#include "Loaders/Loader_Yin.h"
#include "Loaders/YinParser.h"
#include "Loaders/YinVisitor.h"

using namespace std;

namespace Yinspire {

  class YinLoaderVisitor : public YinVisitor
  {
    protected:

      Simulator *simulator;
      NeuralFactory *factory;
      NeuralNet *net;

      // template type -> (type, Properties) mapping
      map<string, pair<string, Properties> > templates;

    public:

      YinLoaderVisitor(Simulator *simulator, NeuralFactory *factory, NeuralNet *net)
      {
        this->simulator = simulator;
        this->factory = factory;
        this->net = net;
      }

      virtual void on_template(string& id, string& type, Properties &p)
      {
        if (templates.count(id) == 0)
        {
          templates[id] = pair<string, Properties>(type, p);
        }
        else
        {
          error("duplicate template name");
        }
      }

      virtual void on_entity(string& id, string& type, Properties &p)
      {
        if (templates.count(type) == 0) 
        {
          create_entity(id, type, p);
        }
        else
        {
          pair<string, Properties>& entry = templates[type];

          // duplicate properties from template
          Properties props = entry.second; 

          // and update with given properties
          props.update(p);

          create_entity(id, entry.first, props);
        }
      }

      virtual void on_connect(string& from_id, string& to_id)
      {
        NeuralEntity *from = net->get(from_id);
        NeuralEntity *to = net->get(to_id);

        if (from != NULL && to != NULL)
        {
          from->connect(to);
        }
        else
        {
          error("unknown id in connect");
        }
      }

      virtual void on_stimulate(string& id, Stimulus& s)
      {
        NeuralEntity *entity = net->get(id);
        if (entity)
        {
          entity->stimulate(s.at, s.weight, NULL); 
        }
        else
        {
          error("unknown id in stimulate");
        }
      }

    protected:

      void create_entity(string& id, string& type, Properties &p)
      {
        NeuralEntity *entity = factory->create(type); 
        if (entity != NULL)
        {
          entity->init(id, simulator);
          net->add(entity);
          entity->load(p);
        }
        else
        {
          error("unknown entity type");
        }
      }

      void error(const char* msg)
      {
        throw msg;
      }

  };

  void Loader_Yin::load(const char* filename)
  {
    YinLoaderVisitor visitor(simulator, factory, net);
    YinParser parser;
    parser.parse(filename, &visitor);
  }

} /* namespace Yinspire */
