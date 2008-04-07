#include "Loaders/YinParser.h"
#include <stdlib.h>

namespace Yinspire {

void YinParser::parse(const char *filename, YinVisitor *visitor)
{
  YinFileTokenizer t;
  tokenizer = &t;

  this->visitor = visitor;

  tokenizer->open(filename);
  at_end = false;

  like_next_token();

  while (!at_end)
  {
    parse_command();
  }

  tokenizer->close();
}

void YinParser::parse_command()
{
  Array<string> ids;
  enum {
    cmd_unknown = 0,
    cmd_TEMPLATE,
    cmd_ENTITY,
    cmd_CONNECT,
    cmd_STIMULATE } cmd = cmd_unknown;

  if (token == "ENTITY")
    cmd = cmd_ENTITY;
  else if (token == "CONNECT")
    cmd = cmd_CONNECT;
  else if (token == "STIMULATE")
    cmd = cmd_STIMULATE;
  else if (token == "TEMPLATE")
    cmd = cmd_TEMPLATE;

  if (cmd != cmd_unknown)
    need_next_token();

  parse_ids(ids);

  if (ids.empty())
    parse_error("empty id list");

  if (token == "=")
  {
    if (cmd == cmd_unknown)
      cmd = cmd_ENTITY;
    if (cmd != cmd_ENTITY)
      parse_error("expect '='");
  } 
  else if (token == "->")
  {
    if (cmd == cmd_unknown)
      cmd = cmd_CONNECT;
    if (cmd != cmd_CONNECT)
      parse_error("expect '->'");
  }
  else if (token == "!")
  {
    if (cmd == cmd_unknown)
      cmd = cmd_STIMULATE;
    if (cmd != cmd_STIMULATE)
      parse_error("expect '!'");
  }
  else if (token == "<")
  {
    if (cmd == cmd_unknown)
      cmd = cmd_TEMPLATE;
    if (cmd != cmd_TEMPLATE)
      parse_error("expect '<'"); 
  }
  else
  {
    parse_error("expect one of '=', '->', '!' or '<'");
  }

  if (cmd == cmd_unknown)
  {
    throw "FATAL";
  }

  need_next_token();

  switch (cmd)
  {
    case cmd_unknown:
      throw "FATAL";

    case cmd_TEMPLATE:
      parse_template(ids);
      break;

    case cmd_ENTITY:
      parse_entity(ids);
      break;

    case cmd_CONNECT:
      throw "not yet implemented";

    case cmd_STIMULATE:
      throw "not yet implemented";

    default:
      throw "FATAL";

  }
}

void YinParser::parse_template(Array<string>& ids)
{
  string type;
  Properties p;

  assert_token_is_id();
  type = token;

  if (like_next_token() && token == "{")
  {
    parse_properties(p);
  }

  for (int i=0; i < ids.size(); i++)
  {
    visitor->on_template(ids[i], type, p);
  }
}

void YinParser::parse_entity(Array<string>& ids)
{
  string type;
  Properties p;

  assert_token_is_id();
  type = token;

  if (like_next_token() && token == "{")
  {
    parse_properties(p);
  }

  for (int i=0; i < ids.size(); i++)
  {
    visitor->on_entity(ids[i], type, p);
  }
}

void YinParser::assert_token_is_id()
{
  if (token == "," || token == "=" || token == "!" || 
      token == "<" || token == "->" || token == "{" ||
      token == "}" || token == "@")
    parse_error("invalid id");
}

void YinParser::parse_properties(Properties& p)
{
  string key;

  if (token != "{")
    parse_error("expect '{'");

  need_next_token();

  while (true) {
    if (token == "}")
      break;

    assert_token_is_id();

    key = token;

    need_next_token();

    if (token != "=")
      parse_error("expect '='");

    need_next_token();

    /*
     * Convert string value to property
     */

    if (token == "true")
    {
      p.dump(true, key);
    }
    else if (token == "false")
    {
      p.dump(false, key);
    }
    else
    {
      // token is assumed to be a float
      char *endptr = NULL;
      real res = strtod(token.c_str(), &endptr);

      if (endptr != token.c_str() + token.size())  
      {
        parse_error("invalid float");
      }

      p.dump(res, key);
    }

    need_next_token();
  }
  like_next_token();
}

void YinParser::parse_ids(Array<string>& ids)
{
  bool expect_token = true;

  do {
    if (expect_token)
    {
      assert_token_is_id();
      ids.push(token);
      expect_token = false;
    }
    else if (token == ",")
    {
      expect_token = true;
    }
    else
    {
      break;
    }
  } while (like_next_token());

  if (expect_token)
    parse_error("id expected");
}

bool YinParser::like_next_token()
{
  bool res = tokenizer->next_token(token);
  if (res == false)
    at_end = true;
  return res;
}

void YinParser::need_next_token()
{
  if (!tokenizer->next_token(token))
    parse_error("unexpected end");
}

void YinParser::parse_error()
{
  parse_error("uknown reason");
}

void YinParser::parse_error(const char* reason)
{
  string msg = "Parse error: ";
  msg += reason;
  throw msg;
}

} /* namespace Yinspire */
