/*
 * Ruby interface for Yinspire
 *
 * Copyright (c) 2008 by Michael Neumann
 */

#include "ruby.h"
#include "Simulator.h"

static VALUE mYinspire;
static VALUE cSimulator;
static VALUE cNeuralEntity;

using namespace Yinspire;

#define SIM_GET(self, sim) \
  Simulator *sim; \
  Data_Get_Struct(self, Simulator, sim)

#define TRY_BEGIN try
#define TRY_END        \
  catch(string _ex) {  \
    rb_raise(rb_eRuntimeError, _ex.c_str()); \
  }

#define NE_WRAP(n) ((n) ? Data_Wrap_Struct(cNeuralEntity, NULL, NULL, n) : Qnil)

#define NE_GET(self, ne) \
  NeuralEntity *ne; \
  Data_Get_Struct(self, NeuralEntity, ne)

#define TO_STR(c) rb_str_new((c).c_str(), (c).size())
#define TO_SYM(c) ID2SYM(rb_intern((c).c_str()))

static VALUE
Simulator_s_alloc(VALUE klass)
{
  return Data_Wrap_Struct(klass, NULL, NULL, new Simulator);
}

static VALUE
Simulator_destroy(VALUE self)
{
  SIM_GET(self, sim);
  TRY_BEGIN {
    delete sim;
  } TRY_END;
  DATA_PTR(self) = NULL;
  return Qnil;
}

static VALUE
Simulator_load_yin(VALUE self, VALUE filename)
{
  SIM_GET(self, sim);
  TRY_BEGIN {
    sim->load_yin(StringValueCStr(filename));
  } TRY_END;
  return Qnil;
}

static VALUE
Simulator_run(VALUE self, VALUE stop_at)
{
  SIM_GET(self, sim);
  TRY_BEGIN {
    return rb_float_new(sim->run(NUM2DBL(stop_at)));
  } TRY_END;
}

static void
Simulator__entity_iter(Yinspire::NeuralEntity *n, void *d)
{
  rb_yield(NE_WRAP(n));
}

static VALUE
Simulator_each_entity(VALUE self)
{
  SIM_GET(self, sim);
  TRY_BEGIN {
    sim->each_entity(Simulator__entity_iter, NULL);
  } TRY_END;
  return Qnil;
}

static VALUE
Simulator_get_entity(VALUE self, VALUE id)
{
  SIM_GET(self, sim);
  TRY_BEGIN {
    return NE_WRAP(sim->get_entity(StringValuePtr(id)));
  } TRY_END;
  return Qnil;
}

static VALUE
Simulator_create_entity(VALUE self, VALUE type, VALUE id)
{
  SIM_GET(self, sim);
  TRY_BEGIN {
    return NE_WRAP(sim->create_entity(StringValuePtr(type), StringValuePtr(id)));
  } TRY_END;
  return Qnil;
}

static VALUE
Simulator_add_entity(VALUE self, VALUE entity)
{
  SIM_GET(self, sim);
  NE_GET(entity, ne);

  TRY_BEGIN {
    sim->add_entity(ne);
  } TRY_END;
  return Qnil;
}

static VALUE
NeuralEntity_id(VALUE self)
{
  NE_GET(self, ne);
  TRY_BEGIN {
    return TO_STR(ne->get_id());
  } TRY_END;
  return Qnil;
}

static VALUE
NeuralEntity_type(VALUE self)
{
  NE_GET(self, ne);
  TRY_BEGIN {
    return rb_str_new2(ne->type());
  } TRY_END;
  return Qnil;
}

static VALUE
NeuralEntity_dump(VALUE self)
{
  NE_GET(self, ne);
  VALUE h = rb_hash_new();

  TRY_BEGIN {
    Properties p;
    ne->dump(p);
    map<string, Property>& pp = p.get_properties(); 
    for (map<string, Property>::iterator i=pp.begin();
      i != pp.end(); i++)
    {
      switch (i->second.type)
      {
        case Property_Real:
          rb_hash_aset(h, TO_SYM(i->first), rb_float_new(i->second.value.real_value));
          break;
        case Property_Bool:
          rb_hash_aset(h, TO_SYM(i->first), i->second.value.bool_value ? Qtrue : Qfalse);
          break;
        default:
          fail("Invalid Property Type");
      };
    }
  } TRY_END;
  return h;
}

static int
each_i(VALUE key, VALUE value, VALUE props)
{
  Properties *p = (Properties*)props;
  char *key_str = NULL;

  switch (TYPE(key))
  {
    case T_STRING:
      key_str = StringValueCStr(key);
      break;
    case T_SYMBOL:
      key_str = rb_id2name(rb_to_id(key));
      break;
    default:
      rb_raise(rb_eRuntimeError, "Invalid key type in Hash (expect String or Symbol)");
  };

  if (value == Qfalse)
    p->set(key_str, false);
  else if (value == Qtrue)
    p->set(key_str, true);
  else if (TYPE(value) == T_FLOAT || TYPE(value) == T_FIXNUM || TYPE(value) == T_BIGNUM)
    p->set(key_str, (real)NUM2DBL(value));
  else
    rb_raise(rb_eRuntimeError, "Invalid value type in Hash (expect Boolean or Numeric)");

  return 0;
}

static VALUE
NeuralEntity_load(VALUE self, VALUE h)
{
  NE_GET(self, ne);
  Check_Type(h, T_HASH);

  TRY_BEGIN {
    Properties p;
    rb_hash_foreach(h, (int (*)(...))each_i, (VALUE)(&p));
    ne->load(p);
  } TRY_END;
  return Qnil;
}

extern "C" void Init_Yinspire()
{
  mYinspire = rb_define_module("Yinspire");

  cSimulator = rb_define_class_under(mYinspire, "Simulator", rb_cObject);
  rb_define_alloc_func(cSimulator, Simulator_s_alloc);
  rb_define_method(cSimulator, "destroy", (VALUE(*)(...))Simulator_destroy, 0);
  rb_define_method(cSimulator, "load_yin", (VALUE(*)(...))Simulator_load_yin, 1);
  rb_define_method(cSimulator, "run", (VALUE(*)(...))Simulator_run, 1);
  rb_define_method(cSimulator, "each_entity", (VALUE(*)(...))Simulator_each_entity, 0);
  rb_define_method(cSimulator, "get_entity", (VALUE(*)(...))Simulator_get_entity, 1);
  rb_define_method(cSimulator, "create_entity", (VALUE(*)(...))Simulator_create_entity, 2);
  rb_define_method(cSimulator, "add_entity", (VALUE(*)(...))Simulator_add_entity, 1);

  cNeuralEntity = rb_define_class_under(mYinspire, "NeuralEntity", rb_cObject);
  rb_define_method(cNeuralEntity, "id", (VALUE(*)(...))NeuralEntity_id, 0);
  rb_define_method(cNeuralEntity, "type", (VALUE(*)(...))NeuralEntity_type, 0);
  rb_define_method(cNeuralEntity, "dump", (VALUE(*)(...))NeuralEntity_dump, 0);
  rb_define_method(cNeuralEntity, "load", (VALUE(*)(...))NeuralEntity_load, 1);
}
