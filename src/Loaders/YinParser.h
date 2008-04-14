#ifndef __YINSPIRE__YIN_PARSER__
#define __YINSPIRE__YIN_PARSER__

#include <string>
#include <stdio.h>
#include "Loaders/YinTokenizer.h"
#include "Loaders/YinVisitor.h"
#include "Core/Common.h"
#include "Core/Properties.h"
#include "Core/Stimulus.h"
#include "Algorithms/Array.h"

namespace Yinspire {

  class YinParser 
  {
    protected:

      YinFileTokenizer *tokenizer;
      YinVisitor *visitor;
      string token;
      bool at_end;

    public:

      void parse(FILE *fh, YinVisitor *visitor);

    protected:

      void parse_command();

      void parse_ids(Array<string>& ids);
      void parse_properties(Properties &p);

      void parse_template(Array<string>& ids);
      void parse_entity(Array<string>& ids);
      void parse_connect(Array<string>& ids);
      void parse_stimulate(Array<string>& ids);
      void parse_stimulus(Stimulus& s);

      double convert_float();
      void assert_token_is_id();
      bool like_next_token();
      void need_next_token();

      void parse_error();
      void parse_error(const char* reason);
  };

} /* namespace Yinspire */

#endif
