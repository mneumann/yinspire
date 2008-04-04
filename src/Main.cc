#include "Allocators/MemoryAllocator.h"
#include "Core/Simulator.h"
#include "Models/Neuron_Output.h"
#include "Models/Neuron_Input.h"
#include "Models/Neuron_SRM01.h"

int main()
{
  using namespace Yinspire;

  Neuron *o = new Neuron_Output;
  Neuron *i = new Neuron_Input;
  Neuron *s = new Neuron_SRM01;

  return 0;
}
