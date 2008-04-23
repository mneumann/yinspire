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
#include "Models/Synapse_Default.h"

namespace Yinspire {

  GEN_WRAPPER_CLASS(Neuron_Input);
  GEN_WRAPPER_CLASS(Neuron_Output);
  GEN_WRAPPER_CLASS(Neuron_SRM01);
  GEN_WRAPPER_CLASS(Neuron_SRM02);
  GEN_WRAPPER_CLASS(Synapse_Default);

  void RegisterTypes(NeuralFactory &factory, bool wrapper=false)
  {
    if (!wrapper)
    {
      REGISTER_TYPE(factory, Neuron_Input);
      REGISTER_TYPE(factory, Neuron_Output);
      REGISTER_TYPE(factory, Neuron_SRM01);
      REGISTER_TYPE(factory, Neuron_SRM02);
      REGISTER_TYPE(factory, Synapse_Default);
    }
    else
    {
      REGISTER_TYPE(factory, Wrap_Neuron_Input);
      REGISTER_TYPE(factory, Wrap_Neuron_Output);
      REGISTER_TYPE(factory, Wrap_Neuron_SRM01);
      REGISTER_TYPE(factory, Wrap_Neuron_SRM02);
      REGISTER_TYPE(factory, Wrap_Synapse_Default);
    }
  }

} /* namespace Yinspire */

#endif
