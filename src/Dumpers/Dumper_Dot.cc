#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include "Core/Neuron.h"
#include "Core/Synapse.h"
#include "Core/Properties.h"
#include "Dumpers/Dumper_Dot.h"

namespace Yinspire {

  void Dumper_Dot::dump(FILE *fh)
  {
    fprintf(fh, "digraph {\n");
    fprintf(fh, "node [shape = circle];\n");
    net->each(dump_entity, fh);
    fprintf(fh, "}\n");
  }

  void Dumper_Dot::dump_entity(NeuralEntity *entity, void *data)
  {
    FILE *f = (FILE*) data;

    /*
     * Reject Synapses
     */ 
    if (dynamic_cast<Neuron*>(entity) == NULL) return;

    entity->each_outgoing_connection(dump_connection, data);
  }

  void Dumper_Dot::dump_connection(NeuralEntity *self, NeuralEntity *conn, void *data)
  {
    FILE *f = (FILE*) data;
    Synapse *syn = dynamic_cast<Synapse*>(conn);
    if (syn == NULL) 
      fail("FATAL");

    fprintf(f, "\"%s\" -> \"%s\" [label = \"%s\"];\n", 
        self->id().c_str(), 
        syn->post_neuron->id().c_str(),
        conn->id().c_str()); 
  }

} /* namespace Yinspire */
