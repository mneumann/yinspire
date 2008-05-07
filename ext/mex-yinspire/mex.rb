$functions = {}
FDef = Struct.new(:arity_lhs, :arity_rhs, :body, :help)
def mex(name, arity_lhs, arity_rhs, body, help=nil)
  $functions[name] = FDef.new(arity_lhs, arity_rhs, body, help)
end

mex :Simulator_new, 1, 0, %{
  Simulator *sim = new Simulator;
  lhs[0] = ptr_to_mex(sim); 
}, "Create a new Simulator"

mex :Simulator_destroy, 0, 1, %{
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  delete sim;
}, "Destroy a Simulator instance"

mex :Simulator_test, 1, 1, %{
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  lhs[0] = logical_to_mex(sim->test());
}, "Run some tests"

mex :Simulator_load_yin, 0, 2, %q{
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  sim->load_yin(mex_to_string(rhs[1]).c_str());
}, "Load a YIN file into the Simulator"

mex :Simulator_run, 1, 2, %q{
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  double stop_at  = mex_to_double(rhs[1]);
  double end_time = sim->run(stop_at);
  lhs[0] = double_to_mex(end_time);
}, "Run the simulation"

mex :Simulator_num_entities, 1, 1, %q{
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  lhs[0] = double_to_mex((double)sim->num_entities());
}, "Return the number of entities"

mex :Simulator_entity_ids, 1, 1, %q{
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  array_with_index c;
  c.array = mex_create_cell_array(sim->num_entities());
  c.i = 0;
  sim->each_entity(fill_array_with_entities, &c);
  lhs[0] = c.array;
}, "Return the ids of all entities"

mex :Simulator_get_entity, 1, 2, %q{
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  lhs[0] = ptr_to_mex(sim->get_entity(mex_to_string(rhs[1])));
}, "Return a pointer to the Entity with the given id"

mex :Simulator_create_entity, 1, 3, %q{
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  NeuralEntity *n = sim->create_entity(mex_to_string(rhs[1]), mex_to_string(rhs[2]));
  sim->add_entity(n);
  lhs[0] = ptr_to_mex(n);
}, "Create a NeuralEntity of the given type and id, and attach it to the net"

mex :NeuralEntity_id, 1, 1, %q{
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  lhs[0] = string_to_mex(e->get_id());
}, "Return the id of the entity"

mex :NeuralEntity_type, 1, 1, %q{
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  lhs[0] = string_to_mex(e->type());
}, "Return the type of the entity"

mex :NeuralEntity_dump, 1, 1, %q{
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

mex :NeuralEntity_load, 0, 2, %q{
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  Properties p;

  mxArray *v = (mxArray*)rhs[1];

  for (int i = 0; i < mxGetNumberOfFields(v); i++)
  {
    const char *key = mxGetFieldNameByNumber(v, i);
    mxArray *value = mxGetFieldByNumber(v, 0, i);

    if (mxIsLogicalScalar(value))
    {
      p.dump((bool)mxIsLogicalScalarTrue(value), key);
    }
    else if (mxIsNumeric(value))
    {
      p.dump((real)mex_to_double(value), key);
    }
    else
    {
      fail("invalid type");
    }
  }

  e->load(p);
}

mex :NeuralEntity_connect, 0, 2, %q{
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  NeuralEntity *target  = (NeuralEntity*) mex_to_ptr(rhs[1]);

  e->connect(target);
}, "Connect"

mex :NeuralEntity_disconnect, 0, 2, %q{
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  NeuralEntity *target  = (NeuralEntity*) mex_to_ptr(rhs[1]);

  e->disconnect(target);
}, "Disconnect"

mex :NeuralEntity_stimulate, 0, 4, %q{
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  e->stimulate(mex_to_double(rhs[1]), mex_to_double(rhs[2]), (NeuralEntity*) mex_to_ptr(rhs[1]));
}, "Stimulate"


####################################################
####################################################

#
# Generate code for functions and the dispatch code
#
definition_code = ""
dispatch_code = "switch (fnn) {\n"
$functions.each_with_index do |kv, i|
  k, v = *kv

  definition_code << %{
    void
    fn_#{k}(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      #{v.body}
    }
  }

  dispatch_code << %{
    case #{i}:
      fn_#{k}(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  }
end
dispatch_code << %[
  default: throw string("invalid opcode");
}
]



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
    int fnn = (int) mex_to_double(rhs[0]);
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

Dir.chdir(ARGV.shift) do 
  $functions.each_with_index do |kv, i|
    name, fdef = *kv
    ARGV.each do |prefix|
      File.open("#{prefix}#{name}.m", 'w+') {|f|
        lhs_args = (1..fdef.arity_lhs).map {|a| "lhs_#{a}"}
        rhs_args = (1..fdef.arity_rhs).map {|a| "rhs_#{a}"}
        lhs = if lhs_args.empty?
               '' 
              else
                lhs_args.join(', ') + ' = '
              end
        rhs = rhs_args.join(', ')
        f << "function #{lhs}#{prefix}#{name}(#{rhs})\n"
        f << "% #{lhs}#{prefix}#{name}(#{rhs})\n"
        f << "% #{fdef.help || 'No documentation available'}\n"
        
        f << "  #{lhs}Yinspire(#{ ([i.to_s] + rhs_args).join(', ') })\n"
      }
    end
  end
end
