#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include "Core/Properties.h"
#include "Dumpers/Dumper_Yin.h"

namespace Yinspire {

  void Dumper_Yin::dump(FILE *fh)
  {
    net->each(dump_entity, fh);
    net->each(dump_connections, fh);
    net->each(dump_stimuli, fh);
  }

  void Dumper_Yin::dump_entity(NeuralEntity *entity, void *data)
  {
    FILE *f = (FILE*) data;
    Properties p;
    entity->dump(p);

    fprintf(f, "ENTITY %s = %s {\n", entity->get_id().c_str(), entity->type());
    p.output(f);
    fprintf(f, "}\n");
  }

  void Dumper_Yin::dump_connections(NeuralEntity *entity, void *data)
  {
    entity->each_outgoing_connection(dump_connection, data);
  }

  void Dumper_Yin::dump_connection(NeuralEntity *self, NeuralEntity *conn, void *data)
  {
    FILE *f = (FILE*) data;
    fprintf(f, "CONNECT %s -> %s\n", self->get_id().c_str(), conn->get_id().c_str());
  }

  struct dump_stimuli_info
  {
    FILE *f;
    int i; 
    NeuralEntity *entity;
  };

  void Dumper_Yin::dump_stimuli(NeuralEntity *entity, void *data)
  {
    dump_stimuli_info info;
    info.f = (FILE*) data;
    info.i = 0;
    info.entity = entity;

    entity->each_stimulus(dump_stimulus, &info);

    if (info.i > 0)
      fprintf(info.f, "}\n");
  }

  void Dumper_Yin::dump_stimulus(const Stimulus& s, void *data)
  {
    dump_stimuli_info *info = (dump_stimuli_info*) data;

    if (info->i++ == 0)
      fprintf(info->f, "STIMULATE %s ! {\n", info->entity->get_id().c_str());

    if (!isinf(s.weight))
    {
      fprint_real(info->f, s.weight); 
      fprintf(info->f, " @ ");
    }
    fprint_real(info->f, s.at); 
    fprintf(info->f, "\n");
  }

} /* namespace Yinspire */
