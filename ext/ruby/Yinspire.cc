#include "ruby.h"
#include "Simulator.h"

static VALUE mYinspire;
static VALUE cSimulator;
static VALUE cNeuralEntity;

using namespace Yinspire;

static VALUE
Simulator_s_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, NULL, new Simulator);
}

static VALUE
Simulator_destroy(VALUE self)
{
  Simulator *sim;
  Data_Get_Struct(self, Simulator, sim);
  delete sim;
  DATA_PTR(self) = NULL;
}

extern "C" void Init_Yinspire()
{
  mYinspire = rb_define_module("Yinspire");

  cSimulator = rb_define_class_under(mYinspire, "Simulator", rb_cObject);
  rb_define_alloc_func(cSimulator, Simulator_s_alloc);
  rb_define_method(cSimulator, "destroy", (VALUE(*)(...))Simulator_destroy, 0);

  //cNeuralEntity = rb_define_class_under(mYinspire, "NeuralEntity", rb_cObject);
}
