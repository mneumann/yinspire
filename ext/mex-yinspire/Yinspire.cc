#include "mex.h"
#include "Simulator.h"
//#include "matrix.h"
#include <string.h>
#include <stdio.h>

using namespace Yinspire;
using namespace std;

struct FireTime
{
  NeuralEntity *origin;
  real at;
  real weight;
};

class MexRecorder : public Recorder
{
  public:

    Array<FireTime> fire_times;
  
    virtual void
      record_fire(NeuralEntity *origin, real at, real weight)
      {
        FireTime f;
        f.origin = origin;
        f.at = at;
        f.weight = weight;
        fire_times.push(f);
      }
};

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


  
    void
    fn_NeuralEntity_stimulate(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  e->stimulate(mex_to_double(rhs[1]), mex_to_double(rhs[2]), (NeuralEntity*) mex_to_ptr(rhs[1]));

    }
  
    void
    fn_Simulator_run(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  double stop_at  = mex_to_double(rhs[1]);
  double end_time = sim->run(stop_at);
  lhs[0] = double_to_mex(end_time);

    }
  
    void
    fn_Simulator_set_default_recorder(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  MexRecorder *rec = (MexRecorder*) mex_to_ptr(rhs[1]);
  sim->set_default_recorder(rec);

    }
  
    void
    fn_NeuralEntity_type(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  lhs[0] = string_to_mex(e->type());

    }
  
    void
    fn_NeuralEntity_set_recorder(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  MexRecorder *rec = (MexRecorder*) mex_to_ptr(rhs[1]);
  e->set_recorder(rec);

    }
  
    void
    fn_Simulator_num_entities(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  lhs[0] = double_to_mex((double)sim->num_entities());

    }
  
    void
    fn_NeuralEntity_dump(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
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
  
    void
    fn_Simulator_new(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim = new Simulator;
  lhs[0] = ptr_to_mex(sim); 

    }
  
    void
    fn_Recorder_new(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  MexRecorder *rec = new MexRecorder;
  lhs[0] = ptr_to_mex(rec); 

    }
  
    void
    fn_Simulator_entity_ids(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  array_with_index c;
  c.array = mex_create_cell_array(sim->num_entities());
  c.i = 0;
  sim->each_entity(fill_array_with_entities, &c);
  lhs[0] = c.array;

    }
  
    void
    fn_NeuralEntity_load(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
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
  
    void
    fn_Simulator_destroy(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  delete sim;

    }
  
    void
    fn_Recorder_destroy(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  MexRecorder *rec = (MexRecorder*) mex_to_ptr(rhs[0]);
  delete rec;

    }
  
    void
    fn_Simulator_get_entity(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  lhs[0] = ptr_to_mex(sim->get_entity(mex_to_string(rhs[1])));

    }
  
    void
    fn_NeuralEntity_connect(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  NeuralEntity *target  = (NeuralEntity*) mex_to_ptr(rhs[1]);

  e->connect(target);

    }
  
    void
    fn_Simulator_test(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  lhs[0] = logical_to_mex(sim->test());

    }
  
    void
    fn_Recorder_clear(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  MexRecorder *rec = (MexRecorder*) mex_to_ptr(rhs[0]);
  rec->fire_times.clear();

    }
  
    void
    fn_Simulator_create_entity(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim  = (Simulator*) mex_to_ptr(rhs[0]);
  NeuralEntity *n = sim->create_entity(mex_to_string(rhs[1]), mex_to_string(rhs[2]));
  sim->add_entity(n);
  lhs[0] = ptr_to_mex(n);

    }
  
    void
    fn_NeuralEntity_disconnect(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  NeuralEntity *target  = (NeuralEntity*) mex_to_ptr(rhs[1]);

  e->disconnect(target);

    }
  
    void
    fn_Simulator_load_yin(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  Simulator *sim = (Simulator*) mex_to_ptr(rhs[0]);
  sim->load_yin(mex_to_string(rhs[1]).c_str());

    }
  
    void
    fn_Recorder_get_data(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  MexRecorder *rec = (MexRecorder*) mex_to_ptr(rhs[0]);

  mxArray *v = mxCreateCellMatrix(3, rec->fire_times.size()); 
  for (int i = 0; i < rec->fire_times.size(); i++)
  {
    FireTime &f = rec->fire_times[i];
    mxSetCell(v, i*3+0, ptr_to_mex(f.origin));
    mxSetCell(v, i*3+1, double_to_mex(f.at));
    mxSetCell(v, i*3+2, double_to_mex(f.weight));
  }
  lhs[0] = v;

    }
  
    void
    fn_NeuralEntity_id(int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
    {
      
  NeuralEntity *e  = (NeuralEntity*) mex_to_ptr(rhs[0]);
  lhs[0] = string_to_mex(e->get_id());

    }
  
  
  void
  mexFunction (int nlhs, mxArray *lhs[], int nrhs, const mxArray *rhs[])
  {
    int fnn = (int) mex_to_double(rhs[0]);
    try {
      switch (fnn) {

    case 0:
      fn_NeuralEntity_stimulate(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 1:
      fn_Simulator_run(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 2:
      fn_Simulator_set_default_recorder(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 3:
      fn_NeuralEntity_type(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 4:
      fn_NeuralEntity_set_recorder(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 5:
      fn_Simulator_num_entities(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 6:
      fn_NeuralEntity_dump(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 7:
      fn_Simulator_new(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 8:
      fn_Recorder_new(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 9:
      fn_Simulator_entity_ids(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 10:
      fn_NeuralEntity_load(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 11:
      fn_Simulator_destroy(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 12:
      fn_Recorder_destroy(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 13:
      fn_Simulator_get_entity(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 14:
      fn_NeuralEntity_connect(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 15:
      fn_Simulator_test(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 16:
      fn_Recorder_clear(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 17:
      fn_Simulator_create_entity(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 18:
      fn_NeuralEntity_disconnect(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 19:
      fn_Simulator_load_yin(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 20:
      fn_Recorder_get_data(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
    case 21:
      fn_NeuralEntity_id(nlhs, lhs, nrhs-1, &rhs[1]);
      break;
  
  default: throw string("invalid opcode");
}

    }
    catch (string msg)
    {
      string err = "# ERROR "; err += msg;
      mexErrMsgTxt(err.c_str());
    }

    // TODO
  }
