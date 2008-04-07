#ifndef __YINSPIRE__YIN_PARSER__
#define __YINSPIRE__YIN_PARSER__

#include <string>
#include <vector>
#include <map>

#include "Loaders/YinTokenizer.h"
#include "Loaders/YinVisitor.h"

using namespace std;

class YinParser 
{
  protected:

    YinFileTokenizer *tokenizer;
    YinVisitor *visitor;
    string token;
    bool at_end;

  public:

    void parse(const char *filename, YinVisitor *visitor);

  protected:

    void parse_command();

    void parse_ids(vector<string>& ids);
    void parse_arguments(map<string,string>& args);

    void parse_template(vector<string>& ids);
    void parse_entity(vector<string>& ids);

    void assert_token_is_id();
    bool like_next_token();
    void need_next_token();

    void parse_error();
    void parse_error(const char* reason);
};

#endif
