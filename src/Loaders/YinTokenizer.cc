#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include "Core/Common.h"
#include "Loaders/YinTokenizer.h"

namespace Yinspire {

YinTokenizer::YinTokenizer(char *token_buf, int token_buf_sz)
{
  this->state = 0; 
  this->p = NULL;
  this->last = NULL;
  this->token_buf_pos = 0; 
  this->token_buf = token_buf;
  this->token_buf_sz = token_buf_sz;
}

void YinTokenizer::to_buf(char ch)
{
  if (token_buf_pos >= token_buf_sz) fail("TokenBuffer full!");
  token_buf[token_buf_pos++] = ch;
}

bool YinTokenizer::is_delim(char ch)
{
  if (isspace(ch) || ch == '#' || 
      ch == '=' || ch == '<' || ch == '@' || ch == ',' ||
      ch == '{' || ch == '}' || ch == '!')
    return true;
  else
    return false;
}

bool YinTokenizer::parse_token()
{
  char ch;
  for (; p < last; ++p)
  {
    ch = *p;

    switch (state)
    {
      case sw_start:

        if (isspace(ch))
        {
          break;
        }

        if (ch == '#')
        {
          state = sw_comment;
          break;
        }

        to_buf(ch);

        if (is_delim(ch))
        {
          ++p;
          return true;
        }

        if (ch == '-')
        {
          state = sw_minus;
          break;
        }

        state = sw_value; 
        break;

      case sw_minus:

        if (ch == '>')
        {
          to_buf(ch);
          ++p;
          return true;
        }

      case sw_value:

        if (is_delim(ch))
        {
          return true; 
        }

        to_buf(ch);

        if (ch == '-')
        {
          state = sw_minus_in_value; 
        }

        break;

      case sw_minus_in_value:

        if (ch == '>')
        {
          // Push back character (see clear_token()).
          state = sw_minus_on_stack;
          --token_buf_sz;
          return true;
        }

        if (is_delim(ch))
        {
          return true; 
        }

        to_buf(ch);

        if (ch == '-')
        {
          state = sw_minus_in_value;
        }
        else
        {
          state = sw_value;
        }

        break;

      case sw_comment:

        if (ch == '\n' || ch == '\r')
        {
          state = sw_start;
        }

        break;

      default:

        fail("invalid character");

    }
  }

  return false;
}

void YinTokenizer::clear_token()
{
  token_buf_pos = 0;

  if (state == sw_minus_on_stack) 
  {
    token_buf[0] = '-';
    token_buf_pos++;
  }
  state = 0;
}

void YinTokenizer::feed(char *p, char *last)
{
  this->p = p;
  this->last = last;
}

bool YinTokenizer::next_token_()
{
  if (parse_token()) return true;

  int sz = wants_more();
  if (sz > 0)
  {
    return next_token_();
  }
  else if (sz == 0)
  {
    if (token_buf_pos == 0)
      return false;
    else
      return true;
  }
  else
  {
    fail("Fatal error");
  }
}

bool YinTokenizer::next_token()
{
  clear_token();
  return next_token_();
}

/* */

int YinFileTokenizer::wants_more()
{
  int sz = read(fh, buffer, buffer_sz);
  if (sz < 0)
  {
    fail("file error");
  }
  feed(buffer, buffer+sz);
  return sz;
}

bool YinFileTokenizer::next_token(std::string& token)
{
  bool res = YinTokenizer::next_token();
  // FIXME: optimimal?
  token = std::string(token_buf, token_buf_pos);
  return res;
}

void YinFileTokenizer::open(const char *filename)
{
  fh = ::open(filename, O_RDONLY);
  if (fh < 0)
    fail("couldn't open file");
}

void YinFileTokenizer::close()
{
  ::close(this->fh);
}

} /* namespace Yinspire */
