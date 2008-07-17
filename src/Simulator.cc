#include "Simulator.h"
#include "Dumpers/Dumper_Yin.h"
#include "Dumpers/Dumper_Dot.h"
#include "RegisterTypes.h"
#include "Test.h"

namespace Yinspire {

  Simulator::Simulator(Recorder *recorder, bool register_wrapper)
  { 
    factory.set_default_recorder(recorder);
    factory.set_default_scheduler(&scheduler);
    RegisterTypes(factory, register_wrapper);
    loader_yin = new Loader_Yin(&factory, &net);
  }

  Simulator::~Simulator()
  {
    delete loader_yin;
  }

  void Simulator::set_default_recorder(Recorder *recorder)
  {
    factory.set_default_recorder(recorder);
  }

  NeuralFactory &Simulator::get_factory()
  {
    return factory;
  }

  real Simulator::run(real stop_at)
  {
    return scheduler.schedule_run(stop_at);
  }

  void Simulator::load_yin(FILE *fh)
  {
    loader_yin->load(fh);
  }

  void Simulator::load_yin(const char *filename)
  {
    FILE *fh = open_in(filename);
    load_yin(fh);
    close_inout(fh);
  }

  void Simulator::dump_yin(FILE *fh)
  {
    Dumper_Yin dumper(&net);
    dumper.dump(fh);
  }

  void Simulator::dump_yin(const char *filename)
  {
    FILE *fh = open_out(filename);
    dump_yin(fh);
    close_inout(fh);
  }

  void Simulator::dump_dot(FILE *fh)
  {
    Dumper_Dot dumper(&net);
    dumper.dump(fh);
  }

  void Simulator::dump_dot(const char *filename)
  {
    FILE *fh = open_out(filename);
    dump_dot(fh);
    close_inout(fh);
  }

  bool Simulator::test()
  {
    /*
     * Run some tests to ensure correct program behaviour.
     */
    Test test;
    return test.run();
  }

  NeuralEntity *Simulator::get_entity(const string &id)
  {
    return net.get(id);
  }

  NeuralEntity *Simulator::create_entity(const string& type, const string &id)
  {
    return factory.create(type, id);
  }

  void Simulator::add_entity(NeuralEntity *entity)
  {
    net.add(entity);
  }

  void Simulator::each_entity(void (*yield)(NeuralEntity*, void*), void *data)
  {
    net.each(yield, data);
  }

  uint Simulator::num_entities()
  {
    return net.size();
  }

  FILE *Simulator::open_in(const char *fname)
  {
    FILE *fh = NULL;

    if (strcmp(fname, "-") == 0)
      fh = stdin;
    else
      fh = fopen(fname, "r");

    if (fh == NULL)
      fail("Couldn't open file: ", fname);

    return fh;
  }

  FILE *Simulator::open_out(const char *fname)
  {
    FILE *fh = NULL;

    if (strcmp(fname, "-") == 0)
      fh = stdout;
    else
      fh = fopen(fname, "w+");

    if (fh == NULL)
      fail("Couldn't open file: ", fname);

    return fh;
  }

  void Simulator::close_inout(FILE *fh)
  {
    if (fh != stdin && fh != stdout && fh != stderr)
      fclose(fh);
  }

} /* namespace Yinspire */
