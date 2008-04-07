#ifndef __YINSPIRE__YIN_VISITOR__
#define __YINSPIRE__YIN_VISITOR__

#include <string>
#include <vector>
#include <map>

using namespace std;

class YinVisitor
{
  public:

    virtual void on_template(vector<string>& ids, string& type, map<string,string>& args) = 0;
    virtual void on_entity(vector<string>& ids, string& type, map<string,string>& args) = 0;
    virtual void on_connect(vector<string>& from_ids, vector<string>& to_ids) = 0;
    virtual void on_stimulate(vector<string>& ids, vector<string>& stimuli) = 0;  

};

#endif
