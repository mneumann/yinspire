/*
 * Ruby interface for Yinspire
 *
 * NOTE: Recorder, Simulator and NeuralEntity
 * are not garbage collected. Use method #destroy.
 *
 * Copyright (c) 2008 by Michael Neumann
 */

#include "ruby.h"
#include "Simulator.h"

static VALUE mYinspire;
static VALUE cRecorder;
static VALUE cSimulator;
static VALUE cNeuralEntity;
static ID id_record_fire;

using namespace Yinspire;

#define TO_STR(c) rb_str_new((c).c_str(), (c).size())
#define TO_SYM(c) ID2SYM(rb_intern((c).c_str()))

#define TRY_BEGIN try
#define TRY_END        \
  catch(string _ex) {  \
    rb_raise(rb_eRuntimeError, _ex.c_str()); \
  }

#define REC_WRAP(n) ((n) ? Data_Wrap_Struct(cRecorder, NULL, NULL, n) : Qnil)

#define REC_GET(self, rec) \
  Recorder *rec = NULL; \
  if (self != Qnil) Data_Get_Struct(self, Recorder, rec)

#define SIM_GET(self, sim) \
  Simulator *sim = NULL; \
  if (self != Qnil) Data_Get_Struct(self, Simulator, sim)

#define NE_WRAP(n) ((n) ? Data_Wrap_Struct(cNeuralEntity, NULL, NULL, n) : Qnil)

#define NE_GET(self, ne) \
  NeuralEntity *ne = NULL; \
  if (self != Qnil) Data_Get_Struct(self, NeuralEntity, ne)

class RubyRecorder : public Recorder
{
  public:

    VALUE obj;

  public:

    RubyRecorder() : obj(Qnil) {}

    virtual void
      record_fire(NeuralEntity *origin, real at, real weight)
      {
        if (obj != Qnil)
        {
          rb_funcall(obj, id_record_fire, 3, 
              NE_WRAP(origin), rb_float_new(at), rb_float_new(weight));
        }
      }
};

static VALUE
Recorder_s_alloc(VALUE klass)
{
  RubyRecorder *rec = new RubyRecorder;
  VALUE obj = Data_Wrap_Struct(klass, NULL, NULL, rec);
  rec->obj = obj;
  return obj;
}

static VALUE
Recorder_destroy(VALUE self)
{
  REC_GET(self, rec);
  TRY_BEGIN {
    delete rec;
  } TRY_END;
  DATA_PTR(self) = NULL;
  return Qnil;
}

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
Simulator_dump_yin(VALUE self, VALUE filename)
{
  SIM_GET(self, sim);
  TRY_BEGIN {
    sim->dump_yin(StringValueCStr(filename));
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
Simulator_set_default_recorder(VALUE self, VALUE recorder)
{
  SIM_GET(self, sim);
  REC_GET(recorder, rec);

  TRY_BEGIN {
    sim->set_default_recorder(rec);
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

static VALUE
NeuralEntity_connect(VALUE self, VALUE entity)
{
  NE_GET(self, ne);
  NE_GET(entity, target);
  TRY_BEGIN {
    ne->connect(target);
  } TRY_END;
  return entity;
}

static VALUE
NeuralEntity_disconnect(VALUE self, VALUE entity)
{
  NE_GET(self, ne);
  NE_GET(entity, target);
  TRY_BEGIN {
    ne->disconnect(target);
  } TRY_END;
  return entity;
}

static VALUE
NeuralEntity_stimulate(int argc, VALUE *argv, VALUE self)
{
  VALUE r_at, r_weight;
  VALUE r_source = Qnil;
  real at, weight;
  NeuralEntity *source;

  rb_scan_args(argc, argv, "21", &r_at, &r_weight, &r_source); 

  NE_GET(self, ne);
  at = NUM2DBL(r_at);
  weight = NUM2DBL(r_weight);
  if (r_source == Qnil)
    source = NULL;
  else 
    Data_Get_Struct(r_source, NeuralEntity, source);

  TRY_BEGIN {
    ne->stimulate(at, weight, source);
  } TRY_END;
  return Qnil;
}

static VALUE
NeuralEntity_get_recorder(VALUE self)
{
  NE_GET(self, ne);
  TRY_BEGIN {
    return REC_WRAP(ne->get_recorder());
  } TRY_END;
}

static VALUE
NeuralEntity_set_recorder(VALUE self, VALUE recorder)
{
  NE_GET(self, ne);
  REC_GET(recorder, rec);
  TRY_BEGIN {
    ne->set_recorder(rec);
  } TRY_END;
  return Qnil;
}


extern "C" void Init_yinspire_ext()
{
  id_record_fire = rb_intern("record_fire");

  mYinspire = rb_define_module("Yinspire");

  cRecorder = rb_define_class_under(mYinspire, "Recorder", rb_cObject);
  rb_define_alloc_func(cRecorder, Recorder_s_alloc);
  rb_define_method(cRecorder, "destroy", (VALUE(*)(...))Recorder_destroy, 0);

  cSimulator = rb_define_class_under(mYinspire, "Simulator", rb_cObject);
  rb_define_alloc_func(cSimulator, Simulator_s_alloc);
  rb_define_method(cSimulator, "destroy", (VALUE(*)(...))Simulator_destroy, 0);
  rb_define_method(cSimulator, "load_yin", (VALUE(*)(...))Simulator_load_yin, 1);
  rb_define_method(cSimulator, "dump_yin", (VALUE(*)(...))Simulator_dump_yin, 1);
  rb_define_method(cSimulator, "run", (VALUE(*)(...))Simulator_run, 1);
  rb_define_method(cSimulator, "each_entity", (VALUE(*)(...))Simulator_each_entity, 0);
  rb_define_method(cSimulator, "get_entity", (VALUE(*)(...))Simulator_get_entity, 1);
  rb_define_method(cSimulator, "create_entity", (VALUE(*)(...))Simulator_create_entity, 2);
  rb_define_method(cSimulator, "add_entity", (VALUE(*)(...))Simulator_add_entity, 1);
  rb_define_method(cSimulator, "default_recorder=", (VALUE(*)(...))Simulator_set_default_recorder, 1);

  cNeuralEntity = rb_define_class_under(mYinspire, "NeuralEntity", rb_cObject);
  rb_define_method(cNeuralEntity, "id", (VALUE(*)(...))NeuralEntity_id, 0);
  rb_define_method(cNeuralEntity, "type", (VALUE(*)(...))NeuralEntity_type, 0);
  rb_define_method(cNeuralEntity, "dump", (VALUE(*)(...))NeuralEntity_dump, 0);
  rb_define_method(cNeuralEntity, "load", (VALUE(*)(...))NeuralEntity_load, 1);
  rb_define_method(cNeuralEntity, "connect", (VALUE(*)(...))NeuralEntity_connect, 1);
  rb_define_method(cNeuralEntity, "disconnect", (VALUE(*)(...))NeuralEntity_disconnect, 1);
  rb_define_method(cNeuralEntity, "stimulate", (VALUE(*)(...))NeuralEntity_stimulate, -1);
  rb_define_method(cNeuralEntity, "recorder", (VALUE(*)(...))NeuralEntity_get_recorder, 0);
  rb_define_method(cNeuralEntity, "recorder=", (VALUE(*)(...))NeuralEntity_set_recorder, 1);
}
