#include "Allocators/MemoryAllocator.h"
#include "Core/Simulator.h"
#include "Models/Neuron_Output.h"
#include "Models/Neuron_Input.h"

int main()
{
  using namespace Yinspire;

  Neuron *o = new Neuron_Output;
  Neuron *i = new Neuron_Input;

  return 0;
}
