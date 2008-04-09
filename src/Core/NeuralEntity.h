#ifndef __YINSPIRE__NEURAL_ENTITY__
#define __YINSPIRE__NEURAL_ENTITY__

#include "Core/Common.h"
#include "Core/ScheduleEntity.h"
#include "Core/Stimulus.h"
#include "Core/Properties.h"
#include <string>

#define DECLARE_ENTITY(name)        \
  public:                           \
    static NeuralEntity* create() { \
      return new name;              \
    }                               \
    static const char* ctype() {    \
      return #name;                 \
    }                               \
    virtual const char* type() {    \
      return #name;                 \
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
      string id_;

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
      const string&
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
        init(const string& id, Simulator *simulator)
        {
          id_ = id;
          scheduler = (Scheduler*) simulator;
        }

      /*
       * Load the internal state of a NeuralEntity from +p+.
       *
       * Note that loading does not neccessarily reset the internal
       * state of the entity!
       */
      virtual void
        load(Properties &p) = 0; 

      /*
       * Dump the internal state of a NeuralEntity into +p+. This does
       * not include the connections and internal stimuli. 
       */ 
      virtual void
        dump(Properties &p) = 0;

      /*
       * Iterator function type used in the each_* methods.
       */
      typedef void (*connection_iter)(NeuralEntity* self, 
                                      NeuralEntity* conn, 
                                      void* data);

      /*
       * Connect with +target+.
       */
      virtual void
        connect(NeuralEntity *target, bool forward=true) = 0;

      /*
       * Disconnect from +target+.
       */
      virtual void
        disconnect(NeuralEntity *target, bool forward=true) = 0;

      /*
       * Calls +yield+ for each incoming connection.
       */
      virtual void
        each_incoming_connection(connection_iter yield, void *data) = 0;

      /*
       * Calls +yield+ for each outgoing connection.
       */
      virtual void
        each_outgoing_connection(connection_iter yield, void *data) = 0;

      /*
       * Stimulate an entity at a specific time with a specific weight.
       */
      virtual void
        stimulate(real at, real weight, NeuralEntity *source) = 0;

      /*
       * Calls +yield+ for each Stimuli in the stimuli_pq.
       */
      void
        each_stimulus(void(*yield)(const Stimulus&, void*), void *data);

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
