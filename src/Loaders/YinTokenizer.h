#ifndef __YINSPIRE__YIN_TOKENIZER__
#define __YINSPIRE__YIN_TOKENIZER__

#include <string>

namespace Yinspire {

  class YinTokenizer
  {
    protected:

      int state;
      char *p;
      char *last;

      char *token_buf;
      int token_buf_sz;
      int token_buf_pos;

      enum {
        sw_start = 0,
        sw_minus,
        sw_value,
        sw_minus_in_value,
        sw_minus_on_stack,
        sw_comment
      };

      void to_buf(char ch);
      void error(const char *str);

      /*
       * Returns +true+ is a token was parsed successfully.
       * Returns +false+ if reached the end of the string.
       */
      bool parse_token();

      void clear_token();

      bool next_token_();

      /*
       * Is called by the tokenizer if it wants more content to tokenize.
       *
       * Return the size of the new string (0 == End of string).  Use
       * method feed() to set the new string.
       */
      virtual int wants_more() { return 0; }

      bool is_delim(char ch);

    public:

      YinTokenizer(char *token_buf, int token_buf_sz);

      /*
       * Feeds the tokenizer with a new string.
       */
      void feed(char *p, char *last);

      bool next_token();
  };

  class YinFileTokenizer : protected YinTokenizer
  {
    protected:

      int fh;
      int buffer_sz;
      char *buffer;

      virtual int wants_more();

    public:

      YinFileTokenizer(int token_buf_sz=100, int buffer_sz=16*1024) :
        YinTokenizer(new char[token_buf_sz], token_buf_sz)
      {
        this->fh = -1;
        this->buffer_sz = buffer_sz;
        buffer = new char[buffer_sz];
        feed(buffer, buffer); 
      }

      ~YinFileTokenizer()
      {
        delete[] buffer;
        delete[] token_buf;
      }

      void open(const char *filename);
      void close();

      bool next_token(std::string& token);
  };

} /* namespace Yinspire */

#endif
