#ifndef __YINSPIRE__NEURAL_ENTITY__
#define __YINSPIRE__NEURAL_ENTITY__

#include "Core/Common.h"
#include "Core/ScheduleEntity.h"
#include "Core/Stimulus.h"
#include "Core/Properties.h"
#include "Core/Recorder.h"
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

/*
 * GEN_WRAPPER_CLASS generates a class that calls record_before_xxx and
 * recorder_after_xxx in method xxx.
 */
#define GEN_WRAPPER_CLASS(klass)                                          \
  class Wrap_ ## klass : public klass {                                   \
    typedef Wrap_ ## klass super;                                         \
    public:                                                               \
      virtual void stimulate(real at, real weight, NeuralEntity *source)  \
      {                                                                   \
        if (recorder)                                                     \
          recorder->record_before_stimulate(this, at, weight, source);    \
        super::stimulate(at, weight, source);                             \
        if (recorder)                                                     \
          recorder->record_after_stimulate(this, at, weight, source);     \
      }                                                                   \
      virtual void process(real at)                                       \
      {                                                                   \
        if (recorder)                                                     \
          recorder->record_before_process(this, at);                      \
        super::process(at);                                               \
        if (recorder)                                                     \
          recorder->record_after_process(this, at);                       \
      }                                                                   \
      virtual void process_stepped(real at, real step)                    \
      {                                                                   \
        if (recorder)                                                     \
          recorder->record_before_process_stepped(this, at, step);        \
        super::process_stepped(at, step);                                 \
        if (recorder)                                                     \
          recorder->record_after_process_stepped(this, at, step);         \
      }                                                                   \
  };


namespace Yinspire {

  /*
   * NeuralEntity is the base class of all entities in a neural net,
   * i.e. Neurons and Synapses. 
   */
  class NeuralEntity : public ScheduleEntity
  {
    protected: 

      /*
       * Each NeuralEntity has an +id+ associated which uniquely
       * identifies itself within a NeuralNet instance. This +id+ 
       * SHOULD NOT be changed, because it is used as a key.
       */
      string id;

      /*
       * Each NeuralEntity has it's own local stimuli priority queue.
       * Neurons make use of this whereas Synapses currently not. 
       *
       * It's quite low overhead to have this in the NeuralEntity class,
       * just around 12 additional bytes.
       */
      Stimulus::PQ stimuli_pq;

      /*
       * Instance of the recorder to use for this NeuralEntity.
       */
      Recorder *recorder;

    public:

      /*
       * Constructor
       */
      NeuralEntity() : recorder(NULL) {}

      /*
       * Returns the +id+ of the entity.
       */
      const string&
        get_id()
        {
          return this->id;
        }

      /*
       * Set the +id+ of the entity.
       */
      void
        set_id(const string& id)
        {
          this->id = id;
        }

      void
        set_recorder(Recorder *recorder)
        {
          this->recorder = recorder;
        }

      Recorder*
        get_recorder()
        {
          return this->recorder;
        }

      /*
       * Returns the name of the type, e.g.  "Neuron_SRM01" or
       * "Synapse_Default".
       */
      virtual const char *
        type() = 0;

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

      /*
       * If you add or remove stimuli's to/from the stimuli_pq, call
       * this method to reschedule the NeuralEntity according to the new
       * top element.
       */
      void
        stimuli_reschedule();

  };

} /* namespace Yinspire */

#endif
