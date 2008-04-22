#include "Core/Common.h"
#include "Core/NeuralEntity.h"
#include "Core/Simulator.h"

namespace Yinspire {

  void NeuralEntity::each_stimulus(void(*yield)(const Stimulus&, void*), void *data)
  {
    stimuli_pq.each(yield, data);
  }

  void NeuralEntity::stimuli_add(real at, real weight)
  {
    Stimulus s; s.at = at; s.weight = weight;
    Stimulus *parent = stimuli_pq.find_parent(s);

    if (parent != NULL && parent->at == s.at)
    {
      parent->weight += s.weight;
    }
    else
    {
      stimuli_pq.push(s);
      stimuli_reschedule();
    }
  }

  real NeuralEntity::stimuli_sum(real till)
  {
    real weight = 0.0;

    while (!stimuli_pq.empty() && stimuli_pq.top().at <= till)
    {
      weight += stimuli_pq.top().weight;
      stimuli_pq.pop();
    }

    stimuli_reschedule();
    return weight;
  }

  real NeuralEntity::stimuli_sum_inf(real till, bool &is_inf)
  {
    real weight = 0.0;
    is_inf = false;

    while (!stimuli_pq.empty() && stimuli_pq.top().at <= till)
    {
      if (isinf(stimuli_pq.top().weight))
      {
        is_inf = true;
      }
      else
      {
        weight += stimuli_pq.top().weight;
      }
      stimuli_pq.pop();
    }

    stimuli_reschedule();
    return weight;
  }

  void NeuralEntity::stimuli_reschedule()
  {
    if (stimuli_pq.empty())
    {
      schedule_at = Infinity;
    }
    else
    {
      schedule(stimuli_pq.top().at);
    }
  }

} /* namespace Yinspire */
