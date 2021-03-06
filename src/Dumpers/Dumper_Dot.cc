#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include "Core/Neuron.h"
#include "Core/Synapse.h"
#include "Core/Properties.h"
#include "Dumpers/Dumper_Dot.h"
#include "Models/Synapse_Base.h"
#include <string>
#include <map>

namespace Yinspire {

  struct Context {
      FILE *fh;
      std::map<string, int> mapping;
      int next_id;
  };

  static void dump_node(NeuralEntity *entity, void *data)
  {
    Context *c = (Context*) data;
    FILE *fh = c->fh;

    /*
     * Reject Synapses
     */ 
    if (dynamic_cast<Neuron*>(entity) == NULL) return;

    int my_id = c->next_id++;
    c->mapping[entity->get_id()] = my_id;

    fprintf(fh, "%d [", my_id);
    fprintf(fh, "label=\"%s\"", entity->get_id().c_str());
    fprintf(fh, "];\n");
  }

  static void dump_edge(NeuralEntity *entity, void *data)
  {
    Context *c = (Context*) data;
    FILE *fh = c->fh;

    /*
     * Reject Neurons
     */ 
    const Synapse *syn = dynamic_cast<const Synapse*>(entity);
    if (syn) {
        int source_id = c->mapping[syn->get_pre_neuron()->get_id()];
        int target_id = c->mapping[syn->get_post_neuron()->get_id()];

        fprintf(fh, "%d -> %d [", source_id, target_id);
        fprintf(fh, "name=\"%s\"", entity->get_id().c_str());

        const Synapse_Base *syn_base = dynamic_cast<const Synapse_Base*>(syn);
        if (syn_base) {
            fprintf(fh, " weight=%f", syn_base->get_weight());
            fprintf(fh, " delay=%f", syn_base->get_delay());
        }

        fprintf(fh, "]\n");
    }
  }

  void Dumper_Dot::dump(FILE *fh)
  {
    Context c;
    c.fh = fh;
    c.next_id = 1;

    fprintf(fh, "digraph {\n");
    fprintf(fh, "node [shape = circle];\n");

    net->each(dump_node, &c);
    net->each(dump_edge, &c);
    fprintf(fh, "}\n");
  }

} /* namespace Yinspire */
