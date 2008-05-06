#ifndef __YINSPIRE__NEURON_LIF01__
#define __YINSPIRE__NEURON_LIF01__

#include "Models/Neuron_Base.h"

namespace Yinspire {

  /*
   * Formerly known as ECurLIF
   *
   * NOTE: Doesn't handle infinite stimuli values correctly. Put a
   * Neuron_Input in front!
   */
  class Neuron_LIF01 : public Neuron_Base
  {
    DECLARE_ENTITY(Neuron_LIF01);

    protected: 

      real tau_m;
      real tau_s;
      real resistor;
      real current_max;
      real mem_pot;
      real mem_pot_bound;
      real input_current;
      real input_current_bound;
      real const_threshold;

    public:

      Neuron_LIF01() :
        tau_m(0.0),
        tau_s(0.0),
        resistor(0.0),
        current_max(0.0),
        mem_pot(0.0),
        mem_pot_bound(0.0),
        input_current(0.0),
        input_current_bound(0.0),
        const_threshold(0.0) {}

      virtual void
        load(Properties &p)
        {
          Neuron_Base::load(p);
          PROP_LOAD(p, tau_m);
          PROP_LOAD(p, tau_s);
          PROP_LOAD(p, resistor);
          PROP_LOAD(p, current_max);
          PROP_LOAD(p, mem_pot);
          PROP_LOAD(p, mem_pot_bound);
          PROP_LOAD(p, input_current);
          PROP_LOAD(p, input_current_bound);
          PROP_LOAD(p, const_threshold);
        }

      virtual void
        dump(Properties &p)
        {
          Neuron_Base::dump(p);
          PROP_DUMP(p, tau_m);
          PROP_DUMP(p, tau_s);
          PROP_DUMP(p, resistor);
          PROP_DUMP(p, current_max);
          PROP_DUMP(p, mem_pot);
          PROP_DUMP(p, mem_pot_bound);
          PROP_DUMP(p, input_current);
          PROP_DUMP(p, input_current_bound);
          PROP_DUMP(p, const_threshold);
        }

      virtual void
        process(real at)
        {
          /*
           * Sum up (and remove) all stimuli weights <= at
           */
          real weight = stimuli_sum(at);

          /*
           * Increase input current
           */ 
          input_current += weight * current_max / tau_s;  

          /*
           * Request stepped scheduling
           */
          schedule_enable_stepping();
        }

      virtual void
        process_stepped(real at, real step)
        {
          /*
           * incoming current at t, t+h/2, t+h
           */
          real current[3];

          /*
           * Calculate actual incoming current using the memoryless exp
           * function
           */
          real factor = exp(-step / (2*tau_s));
          current[0] = input_current;
          input_current *= factor;
          current[1] = input_current;
          input_current *= factor;
          current[2] = input_current;


          /*
           * Calculate new mebrane potential
           * (= 0 if in absolute refractory period)
           *
           * Integrate with Runge-Kutta
           */ 
          if (at >= last_fire_time + abs_refr_duration)
          {
            real k[4];
            k[0] = step * f(current[0], mem_pot);
            k[1] = step * f(current[1], mem_pot + k[0] / 2);
            k[2] = step * f(current[1], mem_pot + k[1] / 2);
            k[3] = step * f(current[2], mem_pot + k[2]);
            mem_pot += 1.0/6.0 * 
              (k[0] + 2 * k[1] + 2 * k[2] + k[3]); // + O(step^5) error 
          }
          else
          {
            mem_pot = 0.0;
          }

          if (mem_pot >= const_threshold)
          {
            fire(at, Infinity);
          }
          else if (mem_pot < mem_pot_bound &&
              current[0] < input_current_bound)
          {
            schedule_disable_stepping();
          }
        }

      inline void
        fire(real at, real weight)
        {
          mem_pot = 0.0;
          last_fire_time = at;
          if (recorder)
            recorder->record_fire(this, at, weight);
          stimulate_synapses(at, weight);
        }

      inline real
        f(real current, real u)
        {
          return ( (-1.0 / tau_m * u) + (resistor / tau_m * current) );
        }

  };

} /* namespace Yinspire */

#endif
