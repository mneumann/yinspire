#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include "Core/Neuron.h"
#include "Core/Synapse.h"
#include "Core/Properties.h"
#include "Dumpers/Dumper_GML.h"
#include "Models/Synapse_Base.h"

namespace Yinspire {

  static void dump_node(NeuralEntity *entity, void *data)
  {
    FILE *fh = (FILE*) data;

    /*
     * Reject Synapses
     */ 
    if (dynamic_cast<Neuron*>(entity) == NULL) return;

    fprintf(fh, "  node\n");
    fprintf(fh, "  [\n");
    fprintf(fh, "    id %s\n", entity->get_id().c_str());
    fprintf(fh, "  ]\n");
  }

  static void dump_edge(NeuralEntity *entity, void *data)
  {
    FILE *fh = (FILE*) data;

    /*
     * Reject Neurons
     */ 
    const Synapse *syn = dynamic_cast<const Synapse*>(entity);
    if (syn) {
        fprintf(fh, "  edge\n");
        fprintf(fh, "  [\n");
        fprintf(fh, "    source %s\n", syn->get_pre_neuron()->get_id().c_str());
        fprintf(fh, "    target %s\n", syn->get_post_neuron()->get_id().c_str());

        const Synapse_Base *syn_base = dynamic_cast<const Synapse_Base*>(syn);
        if (syn_base) {
            fprintf(fh, "    weight %f\n", syn_base->get_weight());
            fprintf(fh, "    delay %f\n", syn_base->get_delay());
        }

        fprintf(fh, "  ]\n");
    }
  }

  void Dumper_GML::dump(FILE *fh)
  {
    fprintf(fh, "graph\n");
    fprintf(fh, "[\n");
    fprintf(fh, "  directed 1\n");

    net->each(dump_node, fh);
    net->each(dump_edge, fh);
    fprintf(fh, "]\n");
  }

} /* namespace Yinspire */
