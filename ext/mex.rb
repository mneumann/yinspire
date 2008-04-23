$functions = {}
FDef = Struct.new(:arity, :body, :help)
def mex(name, arity, body, help=nil)
  $functions[name] = FDef.new(arity, body, help)
end

mex :Yinspire_Simulator_new, 0, %{
  Simulator *sim = new Simulator;
  lhs[0] = ptr_to_mex(sim); 
}, "Create a new Simulator"

mex :Yinspire_Simulator_delete, 1, %{
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  delete sim;
}, "Delete a Simulator instance"

mex :Yinspire_Simulator_test, 1, %{
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  lhs[0] = logical_to_mex(sim->test());
}, "Run some tests"

mex :Yinspire_Simulator_load_yin, 2, %q{
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  sim->load_yin(mex_to_string(rhs[1]).c_str());
}, "Load a YIN file into the Simulator"

mex :Yinspire_Simulator_run, 2, %q{
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  double stop_at  = mex_to_double(rhs[1]);
  double end_time = sim->run(stop_at);
  lhs[0] = double_to_mex(end_time);
}, "Run the simulation"

mex :Yinspire_Simulator_num_entities, 1, %q{
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  lhs[0] = double_to_mex((double)sim->num_entities());
}, "Return the number of entities"

mex :Yinspire_Simulator_entity_ids, 1, %q{
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  array_with_index c;
  c.array = mex_create_cell_array(sim->num_entities());
  c.i = 0;
  sim->each_entity(fill_array_with_entities, &c);
  lhs[0] = c.array;
}, "Return the ids of all entities"

mex :Yinspire_Simulator_get_entity_by_id, 2, %q{
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  lhs[0] = ptr_to_mex(sim->get_entity(mex_to_string(rhs[1])));
}, "Return a pointer to the Entity with the given id"

mex :Yinspire_NeuralEntity_get_id, 1, %q{
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  lhs[0] = string_to_mex(e->get_id());
}, "Return the id of the entity"

mex :Yinspire_NeuralEntity_get_properties, 1, %q{
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  Properties p;
  e->dump(p);
  map<string, Property>& pp = p.get_properties(); 

  char **keys;
  keys = new char*[pp.size()];

  int j = 0;
  for (map<string, Property>::iterator i=pp.begin();
    i != pp.end(); i++)
  {
    keys[j] = (char*)i->first.c_str(); 
    j += 1;
  }

  int d = 1;
  mxArray *v = mxCreateStructArray(1, &d, j, (const char**)keys);

  for (map<string, Property>::iterator i=pp.begin();
    i != pp.end(); i++)
  {
    switch (i->second.type)
    {
      case Property_Real:
        mxSetField(v, 0, (char*)i->first.c_str(), double_to_mex(i->second.value.real_value)); 
        break;
      case Property_Bool:
        mxSetField(v, 0, (char*)i->first.c_str(), logical_to_mex(i->second.value.bool_value)); 
        break;
      default:
        fail("Invalid Property Type");
    };
  }

  lhs[0] = v;
}


####################################################
####################################################

#
# Generate code for functions and the dispatch code
#
definition_code = ""
dispatch_code = ""
$functions.each do |k, v|
  definition_code << %{
    void
    fn_#{k}(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      #{v.body}
    }
  }

  dispatch_code << %{
    if (strcmp(fn, #{k.to_s.inspect}) == 0)
    {
      fn_#{k}(nlhs, lhs, nrhs, rhs);
      return;
    }
  }
end


f = File.open('Yinspire.cc', 'w+')
f << <<HEADER 
#include "mex.h"
#include "Simulator.h"
//#include "matrix.h"
#include <string.h>
#include <stdio.h>

using namespace Yinspire;
using namespace std;

mxArray *mex_create_cell_array(int n)
{
  return mxCreateCellArray(1, &n);
}

double mex_to_double(const mxArray *v)
{
  //return mxGetScalar(v);
  return *mxGetPr(v);
}

mxArray* double_to_mex(double v)
{
  return mxCreateDoubleScalar(v);
}

mxArray *string_to_mex(const string &str)
{
  return mxCreateString(str.c_str());
}

string mex_to_string(const mxArray *v)
{
  char *ptr = mxGetChars(v);
  int len = mxGetN(v);
  return string(ptr, len);
}

mxArray *logical_to_mex(int i)
{
  return mxCreateLogicalScalar(i);
}

mxArray* ptr_to_mex(void* ptr) 
{
  const int sz[] = {1};
  mxArray *v = mxCreateNumericArray(1, sz, mxUINT64_CLASS, mxREAL);
  *((void**)mxGetData(v)) = ptr;
  return v;
}

void* mex_to_ptr(const mxArray* v) 
{
  return *((void**)mxGetData(v));
}

/* ------------------------------------------ */

struct array_with_index {
  mxArray *array;
  int i;
};

void fill_array_with_entities(NeuralEntity *entity, void *data)
{
  array_with_index *c = (array_with_index*) data;
  mxSetCell(c->array, c->i, string_to_mex(entity->get_id()));
  c->i += 1;
}

/* ------------------------------------------ */

HEADER

f << %{
  #{definition_code}
  
  void
  mexFunction (int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
  {
    const char *fn = mexFunctionName();
    try {
      #{dispatch_code}
    }
    catch (string msg)
    {
      string err = "# ERROR "; err += msg;
      mexErrMsgTxt(err.c_str());
    }

    // TODO
  }
}
f.close

# compile
`mkoctfile --mex -I../src -L../build/Release -lyinspirelib Yinspire.cc`

`rm -rf build`
Dir.mkdir('build')
`cp Yinspire.mex build`
Dir.chdir('build') do 
  $functions.each_key do |k|
    File.link "Yinspire.mex", "#{k}.mex"  
  end
  # TODO: generate help files
end
