#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include "Core/Properties.h"
#include "Dumpers/Dumper_Yin.h"
#include <stdio.h>

namespace Yinspire {

  void Dumper_Yin::dump(const char* filename)
  {
    FILE *fh = (strcmp(filename, "-") == 0) ? stdout : fopen(filename, "w+");
    if (!fh)
      fail("could not open file for writing: ", filename);
    net->each(dump_entity, fh);
    net->each(dump_connections, fh);
    net->each(dump_stimuli, fh);
    if (strcmp(filename, "-") != 0) fclose(fh);
  }

  void Dumper_Yin::dump_entity(NeuralEntity *entity, void *data)
  {
    FILE *f = (FILE*) data;
    Properties p;
    entity->dump(p);

    fprintf(f, "ENTITY %s = %s {\n", entity->id().c_str(), entity->type());
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
    fprintf(f, "CONNECT %s -> %s\n", self->id().c_str(), conn->id().c_str());
  }

  void Dumper_Yin::dump_stimuli(NeuralEntity *entity, void *data)
  {
    FILE *f = (FILE*) data;
    fprintf(f, "STIMULATE %s ! {\n", entity->id().c_str());
    entity->each_stimulus(dump_stimulus, data);
    fprintf(f, "}\n");
  }

  void Dumper_Yin::dump_stimulus(const Stimulus& s, void *data)
  {
    FILE *f = (FILE*) data;
    if (isinf(s.weight))
      fprintf(f, "%f\n", s.at);
    else
      fprintf(f, "%f @ %f\n", s.weight, s.at);
  }

} /* namespace Yinspire */
