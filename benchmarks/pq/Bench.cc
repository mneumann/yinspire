#include "Benchmark.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <string>

struct ET_FLOAT 
{
  float priority;

  bool operator ()(ET_FLOAT &a, ET_FLOAT &b) { return (a.priority > b.priority); }
  static const char* element_type() { return "float"; }
  static inline bool less(const ET_FLOAT &a, const ET_FLOAT &b) { return a.priority < b.priority; }
};


struct ET_DOUBLE 
{
  double priority;

  bool operator ()(ET_DOUBLE &a, ET_DOUBLE &b) { return (a.priority > b.priority); }
  static const char* element_type() { return "double"; }
  static inline bool less(const ET_DOUBLE &a, const ET_DOUBLE &b) { return a.priority < b.priority; }
};


struct ET_STIMULI
{
  float priority;
  float weight;

  inline bool operator ()(ET_STIMULI &a, ET_STIMULI &b) { return (a.priority > b.priority); }
  static const char* element_type() { return "Stimuli/float"; }
  static inline bool less(const ET_STIMULI &a, const ET_STIMULI &b) { return (a.priority < b.priority); }
};

/*
 * Pairing heap needs additional fields
 */
struct ET_STIMULI_PH
{
  float priority;
  float weight;

  ET_STIMULI_PH *_next;
  ET_STIMULI_PH *_previous;
  ET_STIMULI_PH *_child;

  inline static bool less(const ET_STIMULI_PH* e1, const ET_STIMULI_PH* e2)
  {
    return e1->priority < e2->priority;
  }

  inline static ET_STIMULI_PH*& next(ET_STIMULI_PH* e) { return e->_next; }
  inline static ET_STIMULI_PH*& previous(ET_STIMULI_PH* e) { return e->_previous; }
  inline static ET_STIMULI_PH*& child(ET_STIMULI_PH* e) { return e->_child; }

  static const char* element_type() { return "Stimuli/float/PairingHeap"; }
};

/*
 * Calendar queue needs an additional "next" field.
 */
struct ET_STIMULI_CQ
{
  float _priority;
  float weight;

  ET_STIMULI_CQ *_next;

  inline static float priority(const ET_STIMULI_CQ* e)
  {
    return e->_priority;
  }

  inline static ET_STIMULI_CQ*& next(ET_STIMULI_CQ* e) { return e->_next; }

  static const char* element_type() { return "Stimuli/float/CalendarQueue"; }
};

#include "Bench_BinaryHeap.h"
#include "Bench_PairingHeap.h"
#include "Bench_CalendarQueue.h"
#include "Bench_StlPq.h"

enum {
  METHOD_CLASSIC_HOLD = 1,
  METHOD_UPDOWN = 2
};

template<class ET, class PQ, class ACC>
void measure(Distribution *dis, 
    int queue_size,
    int warmup_cycles,
    int empty_cycles,
    int cycles,
    int method)
{
  PQ pq;
  Benchmark<PQ, ACC> bm(&pq, dis);
  double time;
  const char *method_name;

  switch (method)
  {
    case METHOD_CLASSIC_HOLD:
      time = bm.classic_hold(queue_size, warmup_cycles, empty_cycles, cycles);
      method_name = "ClassicHold";
      break;

    case METHOD_UPDOWN:
      time = bm.updown(queue_size, warmup_cycles, empty_cycles, cycles);
      method_name = "UpDown";
      break;

    default:
      throw "invalid method";
  };

  std::cout << "Method:           " << method_name << std::endl;
  std::cout << "Algorithm:        " << ACC::algorithm_name() << std::endl;
  std::cout << "ElementSize:      " << sizeof(ET) << std::endl;
  std::cout << "ElementType:      " << ET::element_type() << std::endl;
  std::cout << "Distribution:     "; dis->output_name(std::cout); std::cout << std::endl;
  std::cout << "QueueSize:        " << queue_size << std::endl;
  std::cout << "WarmupCycles:     " << warmup_cycles << std::endl; 
  std::cout << "EmptyCycles:      " << empty_cycles << std::endl; 
  std::cout << "Cycles:           " << cycles << std::endl; 
  std::cout << "CompilerOptFlags: " << _COMPILER_OPTFLAGS_ << std::endl;
  std::cout << "CompilerName:     " << _COMPILER_NAME_ << std::endl;
  std::cout << "CompileDate:      " << _COMPILE_DATE_ << std::endl;
  std::cout << "Uname:            " << _UNAME_ << std::endl;
  std::cout << "CpuFreq:          " << _CPUFREQ_ << std::endl;
  std::cout << "Time:             " << time << std::endl;
  std::cout << std::endl;
}

#define MEASURE(ns, et) measure<et, ns::T<et>::PQ, ns::T<et>::ACC>( \
      dis, queue_size, warmup_cycles, empty_cycles, cycles, method);

#define ARG_GET(varname, meth) \
  if (argp < argc) { \
    varname = meth(argv[argp++]); \
  } else { \
    throw #varname " required"; \
  }

void run(int argc, char **argv)
{
  Distribution *dis;
  int queue_size;
  int warmup_cycles;
  int empty_cycles;
  int cycles;
  int method;
  std::string method_name;
  std::string distribution;
  std::string algorithm;
  std::string element_type;

  int argp = 1; 

  ARG_GET(queue_size, atoi);
  ARG_GET(warmup_cycles, atoi);
  ARG_GET(empty_cycles, atoi);
  ARG_GET(cycles, atoi);

  ARG_GET(method_name, std::string);

  if (method_name == "ClassicHold")
  {
    method = METHOD_CLASSIC_HOLD;
  }
  else if (method_name == "UpDown")
  {
    method = METHOD_UPDOWN;
  }
  else
  {
    throw "invalid method name";
  }

  ARG_GET(distribution, std::string);

  if (distribution == "Random")
  {
    dis = new RandomDistribution();
  }
  else if (distribution == "Exponential")
  {
    double exponential_a;
    ARG_GET(exponential_a, atof);
    dis = new ExponentialDistribution(exponential_a);
  }
  else if (distribution == "Uniform")
  {
    double uniform_a, uniform_b;
    ARG_GET(uniform_a, atof);
    ARG_GET(uniform_b, atof);
    dis = new UniformDistribution(uniform_a, uniform_b);
  }
  else if (distribution == "Triangular")
  {
    double triangular_a, triangular_b;
    ARG_GET(triangular_a, atof);
    ARG_GET(triangular_b, atof);
    dis = new TriangularDistribution(triangular_a, triangular_b);
  }
  else if (distribution == "NegativeTriangular")
  {
    double negtriangular_a, negtriangular_b;
    ARG_GET(negtriangular_a, atof);
    ARG_GET(negtriangular_b, atof);
    dis = new NegativeTriangularDistribution(negtriangular_a, negtriangular_b);
  }
  else if (distribution == "Bimodal")
  {
    double bimodal_a, bimodal_b;
    ARG_GET(bimodal_a, atof);
    ARG_GET(bimodal_b, atof);
    dis = new BimodalDistribution(bimodal_a, bimodal_b);
  }
  else if (distribution == "Pareto")
  {
    double pareto_a;
    ARG_GET(pareto_a, atof);
    dis = new ParetoDistribution(pareto_a);
  }
  else
  {
    throw "invalid distribution";
  }

  ARG_GET(algorithm, std::string);

  if (algorithm == "BinaryHeap")
  {
    ARG_GET(element_type, std::string);

    if (element_type == "FLOAT")
    {
      MEASURE(BenchBinaryHeap, ET_FLOAT);
    }
    else if (element_type == "DOUBLE")
    {
      MEASURE(BenchBinaryHeap, ET_DOUBLE);
    }
    else if (element_type == "STIMULI")
    {
      MEASURE(BenchBinaryHeap, ET_STIMULI);
    }
    else
    {
      throw "invalid element type";
    }
  }
  else if (algorithm == "StlPq")
  {
    ARG_GET(element_type, std::string);

    if (element_type == "FLOAT")
    {
      MEASURE(BenchStlPq, ET_FLOAT);
    }
    else if (element_type == "DOUBLE")
    {
      MEASURE(BenchStlPq, ET_DOUBLE);
    }
    else if (element_type == "STIMULI")
    {
      MEASURE(BenchStlPq, ET_STIMULI);
    }
  }
  else if (algorithm == "PairingHeap")
  {
    ARG_GET(element_type, std::string);

    if (element_type == "STIMULI")
    {
      MEASURE(BenchPairingHeap, ET_STIMULI_PH);
    }
    else
    {
      throw "invalid element type";
    }
  }
  else if (algorithm == "CalendarQueue")
  {
    ARG_GET(element_type, std::string);

    if (element_type == "STIMULI")
    {
      MEASURE(BenchCalendarQueue, ET_STIMULI_CQ);
    }
    else
    {
      throw "invalid element type";
    }
  }
  else
  {
    throw "invalid algorithm";
  }

  if (argp != argc)
  {
    throw "too much arguments";
  } 
}

int main(int argc, char** argv)
{
  try {
    run(argc, argv);
  } catch (const char *err)
  {
    std::cerr << "ERROR: " << err << std::endl; 
    return 1;
  }
  return 0;
}
