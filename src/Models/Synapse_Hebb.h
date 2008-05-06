#ifndef __YINSPIRE__SYNAPSE_HEBB__
#define __YINSPIRE__SYNAPSE_HEBB__

#include "Models/Synapse_Base.h"

namespace Yinspire {

  class Synapse_Hebb : public Synapse_Base
  {
    DECLARE_ENTITY(Synapse_Hebb);

    protected:

      real last_fire_time;
      real current_fire_time;
      real learning_rate;
      real decrease_rate;
      Array<real> pre_synaptic_spikes;

    public:

      Synapse_Hebb() :
        last_fire_time(-Infinity),
        current_fire_time(-Infinity),
        learning_rate(0.0),
        decrease_rate(0.0) {}

      virtual void
        load(Properties &p)
        {
          Synapse_Base::load(p);
          PROP_LOAD(p, last_fire_time);
          PROP_LOAD(p, current_fire_time);
          PROP_LOAD(p, learning_rate);
          PROP_LOAD(p, decrease_rate);
        }

      virtual void
        dump(Properties &p)
        {
          Synapse_Base::dump(p);
          PROP_DUMP(p, last_fire_time);
          PROP_DUMP(p, current_fire_time);
          PROP_DUMP(p, learning_rate);
          PROP_DUMP(p, decrease_rate);
        }

      virtual void
        stimulate(real at, real _weight, NeuralEntity *source)
        {
          if (source != post_neuron)
          {
            pre_synaptic_spikes.push(at);

            if (last_fire_time > 0.0)
            {
              real delta_time = last_fire_time - at;  
              real delta_weight = learning_rate * learning_window(delta_time);
              uint sz = pre_synaptic_spikes.size();

              if (sz > 1)
              {
                delta_time = pre_synaptic_spikes[sz - 2] - at;
              }

              delta_weight += decrease_rate * delta_time;
              weight += (1.0 - fabs(weight)) * delta_weight;
            }

            /*
             * Forward stimuli
             */
            post_neuron->stimulate(at+delay, weight, this);
          }
          else
          {
            real delta_weight = 0.0;

            last_fire_time = current_fire_time; 
            current_fire_time = at;

            for (int i=0; i < pre_synaptic_spikes.size(); i++)
            {
              delta_weight += learning_rate * 
                learning_window(at - pre_synaptic_spikes[i]);
            }

            weight += (1.0 - fabs(weight)) * delta_weight;
            pre_synaptic_spikes.clear();
          }
        }

      inline real
        learning_window(real delta_x, real pos_ramp=1, real neg_ramp=1,
            real pos_decay=10, real neg_decay=8)
        {
          return ( (delta_x >= 0) ? 
            (pos_ramp * delta_x * exp(-delta_x/pos_decay)) : 
            (neg_ramp * delta_x * exp(delta_x/neg_decay)) );
        }

  };

} /* namespace Yinspire */

#endif
