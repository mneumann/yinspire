#include "Allocators/MemoryAllocator.h"
#include "Core/Simulator.h"
#include "Core/NeuralNet.h"
#include "Models/Neuron_Output.h"
#include "Models/Neuron_Input.h"
#include "Models/Neuron_SRM01.h"
#include "Models/Neuron_SRM02.h"
#include "Models/Synapse_Default.h"

int main()
{
  using namespace Yinspire;

  NeuralNet nn;
  Simulator simulator;
  NeuralEntity *entity;

  entity = new Neuron_Output;
  entity->init("id1", &simulator);
  nn.add(entity);

  entity = new Neuron_Input;
  entity->init("id2", &simulator);
  nn.add(entity);

  entity = new Neuron_SRM01;
  entity->init("id3", &simulator);
  nn.add(entity);

  entity = new Neuron_SRM02;
  entity->init("id4", &simulator);
  nn.add(entity);

  entity = new Synapse_Default;
  entity->init("id5", &simulator);
  nn.add(entity);

  return 0;
}
