#include "Core/Common.h"
#include "Core/Simulator.h"
#include "Core/NeuralNet.h"
#include "Core/NeuralFactory.h"
#include "Core/Properties.h"
#include "Loaders/Loader_Yin.h"
#include "Dumpers/Dumper_Yin.h"
#include "RegisterTypes.h"

using namespace Yinspire;
using namespace std;

class MySimulator : public Simulator
{
  virtual void record_output_fire(real at, real weight, NeuralEntity *source)
  {
    fprintf(stdout, "%s\t!\t", source ? source->id().c_str() : "_SIMULATOR_"); 
    if (!isinf(weight))
      fprintf(stdout, "%f\t@\t", weight);
    fprintf(stdout, "%f\n", at);
  }

  virtual void record_fire(real at, real weight, NeuralEntity *source)
  {
    record_output_fire(at, weight, source);
  }
};

void usage()
{
  printf(
   "Usage: yinspire [options] file [file...]\n"
   "    --set X=Y                   Set option\n"
   "    --set stop_at=N             Stop simulator at N (default: Infinity)\n"
   "    --set stimuli_tolerance=N   Stimuli tolerance (default: 0.0)\n"
   "    --version                   Show version\n"
   "    --help                      Show this message\n");
}

void version()
{
  printf(
   "yinspire++ (version %s)\n"
   "    Copyright (c) 2007, 2008 by Michael Neumann (mneumann@ntecs.de).\n"
   "    Released under the Ruby license.\n", YINSPIRE_VERSION);
}

bool get_float(string arg, string flag, real &res)
{ 
  string set = arg;
  if (set.find(flag) == 0)
  {
    set.erase(0, flag.size());
    const char *n = set.c_str(); 
    char *e;
    res = strtod(n, &e);
    if (e != n + set.size())
    {
      fprintf(stderr, "invalid float at --set %s\n", arg.c_str());
      exit(1);
    }
    return true;
  }
  return false;
}

int main(int argc, char **argv)
{
  MySimulator simulator;
  NeuralFactory factory;
  NeuralNet nn;
  Loader_Yin loader(&simulator, &factory, &nn);
  RegisterTypes(factory);

  /*
   * Parse Command Line
   */
  vector<string> settings;
  int i;

  for (i = 1; i < argc; i++)
  {
    string arg(argv[i]);
    if (arg == "--version")
    {
      version();
      return 0;
    }
    else if (arg == "--help")
    {
      usage();
      return 0;
    }
    else if (arg == "--set")
    {
      if (++i < argc)
      {
        settings.push_back(argv[i]);
      }
      else
      {
        fprintf(stderr, "Missing argument for --set\n");
        usage();
        return 1;
      }
    }
    else if (arg.find("--") == 0)
    {
      fprintf(stderr, "Invalid argument %s\n\n", arg.c_str());
      usage();
      return 1;
    }
    else
    {
      break;
    }
  }

  if (i >= argc)
  {
    usage();
    return 0;
  }

  real stop_at = Infinity;

  for (int j=0; j < settings.size(); j++)
  {
    string set = settings[j];

    if (!get_float(set, "stop_at=", stop_at) && 
        !get_float(set, "stimuli_tolerance=", simulator.stimuli_tolerance))
    {
      fprintf(stderr, "Invalid argument --set %s\n", set.c_str());
      return 1;
    }
  }

  fprintf(stderr, "# SET stop_at=%f\n", stop_at);
  fprintf(stderr, "# SET stimuli_tolerance=%f\n", simulator.stimuli_tolerance);

  try {
    for (; i < argc; i++)
    {
      fprintf(stderr, "# LOAD \"%s\"\n", argv[i]);
      loader.load(argv[i]);
    }

    fprintf(stderr, "# LOG starting simulation...\n");
    simulator.run(stop_at);
  }
  catch (string msg)
  {
    fprintf(stderr, "# ERROR %s\n", msg.c_str()); 
    return 1;
  }

  Dumper_Yin dumper(&nn);
  dumper.dump("out");

  return 0;
}
