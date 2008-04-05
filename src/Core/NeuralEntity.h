#ifndef __YINSPIRE__NEURAL_ENTITY__
#define __YINSPIRE__NEURAL_ENTITY__

#include "Core/ScheduleEntity.h"
#include "Core/Stimulus.h"
#include <string>

#define DECLARE_ENTITY(name)      \
  public:                         \
    static NeuralEntity* make() { \
      return new name;            \
    }                             \
    static const char* ctype() {  \
      return #name;               \
    }                             \
    virtual const char* type() {  \
      return #name;               \
    }

namespace Yinspire {

  class Simulator; // Forward declaration

  /*
   * NeuralEntity is the base class of all entities in a neural net,
   * i.e. Neurons and Synapses. 
   */
  class NeuralEntity : public ScheduleEntity
  {
    protected: 

      /*
       * Each NeuralEntity has an +id+ associated which uniquely
       * identifies itself within a Simulator instance. This +id+ 
       * SHOULD NOT be changed, because it is used as a key in
       * the Simulator.
       */
      std::string id_;

      /*
       * Each NeuralEntity has it's own local stimuli priority queue.
       * Neurons make use of this whereas Synapses currently not. 
       *
       * It's quite low overhead to have this in the NeuralEntity class,
       * just around 12 additional bytes.
       */
      Stimulus::PQ stimuli_pq;

    public:

      /*
       * Returns the +id+ of the entity.
       */
      const std::string&
        id()
        {
          return id_;
        }

      /*
       * Returns the name of the type, e.g.  "Neuron_SRM01" or
       * "Synapse_Default".
       */
      virtual const char *
        type() = 0;

      /*
       * Initialize the NeuralEntity with an id and a simulator. 
       *
       * Doing the same in a constructor would lead to a lot of
       * repetition in each subclass.
       */
      void
        init(const std::string& id, Simulator *simulator)
        {
          id_ = id;
          scheduler = (Scheduler*) simulator;
        }

      /*
       * Load the internal state of a NeuralEntity
       * from +data+.
       *
       * Note that loading does not neccessarily reset
       * the internal state of the entity!
       */
      //virtual void
        //load_properties(Properties *data); 

      /*
       * Dump the internal state of a NeuralEntity
       * and return it. Internal state does not contain 
       * the network connection (they have to be dumped
       * separatly by the simulator using +each_connection+.
       */ 
      //virtual void
        //dump_properties(Properties *into);

      /*
       * Connect with +target+.
       */
      virtual void
        connect(NeuralEntity *target) = 0;

      /*
       * Disconnect from +target+.
       */
      virtual void
        disconnect(NeuralEntity *target) = 0;

      /*
       * Disconnect from all connections. Uses +each_connection+ and
       * +disconnect+.
       */
      void disconnect_all();

      /*
       * Calls +yield+ for each outgoing connection.
       */
      virtual void each_connection(
          void (*yield)(NeuralEntity *self, NeuralEntity *conn)) = 0;

      /*
       * Stimulate an entity at a specific time with a specific weight.
       */
      virtual void
        stimulate(real at, real weight, NeuralEntity *source) = 0;

    protected:

      Simulator *
        simulator() const
        {
          return (Simulator*) scheduler;
        }

      /*
       * Add a Stimuli to the local priority queue.
       */
      void
        stimuli_add(real at, real weight);

      /* 
       * Consume all Stimuli until +till+ and return the sum of the weights.
       */
      real
        stimuli_sum(real till);

      /*
       * Consume all Stimuli until +till+ and return the sum of the weights.
       * This treats infinitive values specially and instead of summing them,
       * it sets +is_inf+ to +true+.
       */
      real
        stimuli_sum_inf(real till, bool &is_inf);

  };

} /* namespace Yinspire */

#endif
