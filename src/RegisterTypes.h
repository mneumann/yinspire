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

  void RegisterTypes(NeuralFactory &factory)
  {
    REGISTER_TYPE(factory, Neuron_Input);
    REGISTER_TYPE(factory, Neuron_Output);
    REGISTER_TYPE(factory, Neuron_SRM01);
    REGISTER_TYPE(factory, Neuron_SRM02);
    REGISTER_TYPE(factory, Synapse_Default);
  }

} /* namespace Yinspire */

#endif
