#include "Allocators/MemoryAllocator.h"
#include "Core/Simulator.h"
#include "Core/NeuralNet.h"
#include "Core/NeuralFactory.h"
#include "Models/Neuron_Output.h"
#include "Models/Neuron_Input.h"
#include "Models/Neuron_SRM01.h"
#include "Models/Neuron_SRM02.h"
#include "Models/Synapse_Default.h"
#include <iostream>

int main()
{
  using namespace Yinspire;
  using namespace std;

  NeuralNet nn;
  Simulator simulator;
  NeuralFactory factory;
  NeuralEntity *entity;

  REGISTER_TYPE(factory, Neuron_Input);
  REGISTER_TYPE(factory, Neuron_Output);
  REGISTER_TYPE(factory, Neuron_SRM01);
  REGISTER_TYPE(factory, Neuron_SRM02);
  REGISTER_TYPE(factory, Synapse_Default);

  entity = factory.create("Neuron_Output");
  entity->init("id1", &simulator);
  nn.add(entity);

  entity = factory.create("Neuron_Input");
  entity->init("id2", &simulator);
  nn.add(entity);

  entity = factory.create("Neuron_SRM01");
  entity->init("id3", &simulator);
  nn.add(entity);

  entity = factory.create("Neuron_SRM02");
  entity->init("id4", &simulator);
  nn.add(entity);

  entity = factory.create("Synapse_Default");
  entity->init("id5", &simulator);
  nn.add(entity);

  cout << nn.get("id5")->id() << endl;
  cout << nn.get("id4")->type() << endl;

  return 0;
}
