#include "Simulator.h"
#include "Core/Recorder.h"

using namespace Yinspire;
using namespace std;

class MyRecorder : public Recorder
{
  protected:

    FILE *out;

  public:

    MyRecorder() : out(NULL) {}

    void set_out(FILE *out)
    {
      this->out = out;
    }

    virtual void record_fire(NeuralEntity *origin, real at, real weight)
    {
      if (!out) return;

      fprintf(out, "%s\t!\t", origin ? origin->get_id().c_str() : "_SIMULATOR_"); 
      if (!isinf(weight))
        fprintf(out, "%f\t@\t", weight);
      fprintf(out, "%f\n", at);
    }
};

void usage()
{
  printf(
   "Usage: yinspire [options] file [file...]\n"
   "    --stop-at N             Stop simulator at N (default: Infinity)\n"
   "    --record FILE           Record fires to this file\n"
   "    --dump FILE             Dump net after simulation\n"
   "    --dump-dot FILE         Dump net after simulation in dot format\n"
   "    --version               Show version\n"
   "    --help                  Show this message\n");
}

void version()
{
  printf(
   "yinspire++ (version %s)\n"
   "    Copyright (c) 2007, 2008 by Michael Neumann (mneumann@ntecs.de).\n"
   "    Released under the Ruby license.\n", YINSPIRE_VERSION);
}

real str_to_real(char *arg)
{
  char *e;
  real res = strtod(arg, &e); 

  if (e != arg + strlen(arg))
  {
    fprintf(stderr, "invalid float: %s\n", arg);
    exit(1);
  }

  return res;
}

FILE *open_in(const char *fname)
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

FILE *open_out(const char *fname)
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

void close_inout(FILE *fh)
{
  if (fh != stdin && fh != stdout && fh != stderr)
    fclose(fh);
}

int main(int argc, char **argv)
{
  MyRecorder recorder;
  Simulator sim(&recorder);

  /*
   * Run some tests at startup to ensure correct program behaviour.
   */
  if (!sim.test())
  {
    fprintf(stderr, "Tests failed\n");
    return -1;
  }

  /*
   * Parse Command Line
   */
  int i;
  char *record_file = NULL;
  char *dump_file = NULL;
  char *dump_dot = NULL;
  real stop_at = Infinity;

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
    else if (arg == "--stop-at")
    {
      if (++i < argc)
      {
        stop_at = str_to_real(argv[i]);
      }
      else
      {
        fprintf(stderr, "Missing argument for --stop-at\n");
        usage();
        return 1;
      }
    }
    else if (arg == "--record")
    {
      if (++i < argc)
      {
        record_file = argv[i];
      }
      else
      {
        fprintf(stderr, "Missing argument for --record\n");
        usage();
        return 1;
      }
    }
    else if (arg == "--dump")
    {
      if (++i < argc)
      {
        dump_file = argv[i];
      }
      else
      {
        fprintf(stderr, "Missing argument for --dump\n");
        usage();
        return 1;
      }
    }
    else if (arg == "--dump-dot")
    {
      if (++i < argc)
      {
        dump_dot = argv[i];
      }
      else
      {
        fprintf(stderr, "Missing argument for --dump-dot\n");
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

  fprintf(stderr, "# LOG stop_at=%f\n", stop_at);

  if (record_file)
  {
    recorder.set_out(open_out(record_file));
  }
  else
  {
    sim.set_default_recorder(NULL);
  }

  try {
    for (; i < argc; i++)
    {
      fprintf(stderr, "# LOAD \"%s\"\n", argv[i]);
      FILE *fh = open_in(argv[i]);
      sim.load_yin(fh);
      close_inout(fh);
    }

    fprintf(stderr, "# LOG starting simulation...\n");
    real end_time = sim.run(stop_at);
    fprintf(stderr, "# LOG simulation ended at: %f\n", end_time);
  }
  catch (string msg)
  {
    fprintf(stderr, "# ERROR %s\n", msg.c_str()); 
    return 1;
  }

  if (dump_file)
  {
    fprintf(stderr, "# LOG Dump to %s\n", dump_file);
    FILE *fh = open_out(dump_file); 
    sim.dump_yin(fh);
    close_inout(fh);
  }

  if (dump_dot)
  {
    fprintf(stderr, "# LOG Dump (dot) to %s\n", dump_dot);
    FILE *fh = open_out(dump_dot); 
    sim.dump_dot(fh);
    close_inout(fh);
  }

  return 0;
}
