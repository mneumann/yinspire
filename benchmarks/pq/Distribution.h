#include <math.h>
#include <ostream>
#include <boost/random.hpp>

class Distribution
{
  protected:

    boost::mt19937 eng;
    boost::uniform_real<> dis;
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > gen;

  public:

    Distribution() : gen(eng, dis) {}

    virtual double next() = 0;
    virtual void output_name(std::ostream &o) { o << "undefined"; } 

    void 
      seed(unsigned seed)
      {
        eng.seed(seed);
      }
};

struct RandomDistribution : public Distribution
{
  virtual double next() { return gen(); }
  virtual void output_name(std::ostream &o) { o << "Random"; } 
};

struct ExponentialDistribution : public Distribution
{
  double a;
  ExponentialDistribution(double _a) : a(_a) {}

  virtual double next() { return a * -log(gen()); }

  virtual void output_name(std::ostream &o) { o << "Exponential(" << this->a << ")"; } 
};

struct UniformDistribution : public Distribution
{
  double a, b;

  UniformDistribution(double _a, double _b) : a(_a), b(_b) {} 

  virtual double next() { return (a + (b-a)*gen()); }
  virtual void output_name(std::ostream &o) { o << "Uniform(" << this->a << "," << this->b << ")"; } 
};

struct TriangularDistribution : public Distribution
{
  double a, b;

  TriangularDistribution(double _a, double _b) : a(_a), b(_b) {} 

  virtual double next() { return (a + (b-a)*sqrt(gen())); }
  virtual void output_name(std::ostream &o) { o << "Triangular(" << this->a << "," << this->b << ")"; } 
};

struct NegativeTriangularDistribution : public Distribution
{
  double a, b;

  NegativeTriangularDistribution(double _a, double _b) : a(_a), b(_b) {} 

  virtual double next() { return (a + (b-a)*(1.0 - sqrt(1.0 - gen()))); }
  virtual void output_name(std::ostream &o) { o << "NegativeTriangular(" << this->a << "," << this->b << ")"; } 
};

struct BimodalDistribution : public Distribution
{
  double a, b;

  BimodalDistribution(double _a=9.95238, double _b=0.1) : a(_a), b(_b) {} 

  virtual double next() { return a * gen() + (gen() < b ? a : 0); }
    
  virtual void output_name(std::ostream &o) { o << "Bimodal(" << this->a << "," << this->b << ")"; } 
};

struct ParetoDistribution : public Distribution
{
  double a;

  ParetoDistribution(double _a) : a(_a) {} 

  virtual double next() { return pow(1.0/(1.0 - gen()), 1.0 / a); }
    
  virtual void output_name(std::ostream &o) { o << "Pareto(" << this->a << ")"; } 
};
