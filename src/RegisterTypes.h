#ifndef __YINSPIRE__REGISTER_TYPES__
#define __YINSPIRE__REGISTER_TYPES__

#include "Core/NeuralFactory.h"

/*
 * Models
 */
#include "Models/Neuron_Output.h"
#include "Models/Neuron_Input.h"
#include "Models/Neuron_SRM01.h"
#include "Models/Neuron_SRM02.h"
#include "Models/Neuron_LIF01.h"
#include "Models/Synapse_Default.h"
#include "Models/Synapse_Hebb.h"

namespace Yinspire {

  GEN_WRAPPER_CLASS(Neuron_Input);
  GEN_WRAPPER_CLASS(Neuron_Output);
  GEN_WRAPPER_CLASS(Neuron_SRM01);
  GEN_WRAPPER_CLASS(Neuron_SRM02);
  GEN_WRAPPER_CLASS(Neuron_LIF01);
  GEN_WRAPPER_CLASS(Synapse_Default);
  GEN_WRAPPER_CLASS(Synapse_Hebb);

  void RegisterTypes(NeuralFactory &factory, bool wrapper=false)
  {
    if (!wrapper)
    {
      REGISTER_TYPE(factory, Neuron_Input);
      REGISTER_TYPE(factory, Neuron_Output);
      REGISTER_TYPE(factory, Neuron_SRM01);
      REGISTER_TYPE(factory, Neuron_SRM02);
      REGISTER_TYPE(factory, Neuron_LIF01);
      REGISTER_TYPE(factory, Synapse_Default);
      REGISTER_TYPE(factory, Synapse_Hebb);
    }
    else
    {
      REGISTER_TYPE(factory, Wrap_Neuron_Input);
      REGISTER_TYPE(factory, Wrap_Neuron_Output);
      REGISTER_TYPE(factory, Wrap_Neuron_SRM01);
      REGISTER_TYPE(factory, Wrap_Neuron_SRM02);
      REGISTER_TYPE(factory, Wrap_Neuron_LIF01);
      REGISTER_TYPE(factory, Wrap_Synapse_Default);
      REGISTER_TYPE(factory, Wrap_Synapse_Hebb);
    }
  }

} /* namespace Yinspire */

#endif
