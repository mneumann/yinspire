#include "Core/Common.h"
#include "Loaders/Loader_Yin.h"
#include "Loaders/YinParser.h"

namespace Yinspire {

  void Loader_Yin::on_template(string& id, string& type, Properties &p)
  {
    if (templates.count(id) == 0)
    {
      templates[id] = pair<string, Properties>(type, p);
    }
    else
    {
      fail("duplicate template name: ", id);
    }
  }

  void Loader_Yin::on_entity(string& id, string& type, Properties &p)
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

      // this allows recursive template types, where 
      // a template inherits from another template.
      on_entity(id, entry.first, props);
    }
  }

  void Loader_Yin::on_connect(string& from_id, string& to_id)
  {
    NeuralEntity *from = net->get(from_id);
    NeuralEntity *to = net->get(to_id);

    if (from == NULL)
      fail("unknown id in connect: ", from_id);

    if (to == NULL)
      fail("unknown id in connect: ", to_id);

    from->connect(to);
  }

  void Loader_Yin::on_stimulate(string& id, Stimulus& s)
  {
    NeuralEntity *entity = net->get(id);
    if (entity)
    {
      entity->stimulate(s.at, s.weight, NULL); 
    }
    else
    {
      fail("unknown id in stimulate: ", id);
    }
  }

  void Loader_Yin::create_entity(string& id, string& type, Properties &p)
  {
    NeuralEntity *entity = factory->create(type, id, p); 
    if (entity != NULL)
    {
      net->add(entity);
    }
    else
    {
      fail("unknown entity type: ", type);
    }
  }

  void Loader_Yin::load(FILE *fh)
  {
    YinParser parser;
    parser.parse(fh, this);
  }

} /* namespace Yinspire */
