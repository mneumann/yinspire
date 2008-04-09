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
    fail("FATAL");
  }

  need_next_token();

  switch (cmd)
  {
    case cmd_unknown:
      fail("FATAL");

    case cmd_TEMPLATE:
      parse_template(ids);
      break;

    case cmd_ENTITY:
      parse_entity(ids);
      break;

    case cmd_CONNECT:
      parse_connect(ids);
      break;

    case cmd_STIMULATE:
      parse_stimulate(ids);
      break;

    default:
      fail("FATAL");

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

/*
 * NOTE: Method destroys "ids"!
 */
void YinParser::parse_connect(Array<string>& ids)
{
  Array<string> ids2;
  Array<string>& from_ids = ids;
  Array<string>& to_ids = ids2;

  while (true)
  {
    to_ids.clear();
    parse_ids(to_ids);
    if (to_ids.empty())
      parse_error("non-empty id list expected");

    for (int i = 0; i < from_ids.size(); i++)
      for (int j = 0; j < to_ids.size(); j++)
        visitor->on_connect(from_ids[i], to_ids[j]);

    Array<string>& tmp = from_ids;
    from_ids = to_ids;
    to_ids = tmp;

    if (token != "->")
      break;

    need_next_token();
  }
}

void YinParser::parse_stimulus(Stimulus &s)
{
  s.weight = Infinity;
  s.at = convert_float();
  if (like_next_token() && token == "@")
  {
    need_next_token();
    s.weight = s.at;
    s.at = convert_float();
    like_next_token();
  }
}

void YinParser::parse_stimulate(Array<string>& ids)
{
  Stimulus s;

  if (token == "{")
  {
    /*
     * Multiple stimulis are given
     */
    need_next_token();
    while (token != "}")
    {
      parse_stimulus(s);
      for (int i=0; i < ids.size(); i++)
      {
        visitor->on_stimulate(ids[i], s);
      }
      if (at_end) parse_error("unexpected end");
    }
    like_next_token();
  }
  else
  {
    /*
     * Only one stimuli
     */
    parse_stimulus(s);
    for (int i=0; i < ids.size(); i++)
    {
      visitor->on_stimulate(ids[i], s);
    }
  }
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
      p.dump((real)convert_float(), key);
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

double YinParser::convert_float()
{
  char *endptr = NULL;
  double res; 
  
  if (token == "," || token == "=" || token == "!" || 
      token == "<" || token == "->" || token == "{" ||
      token == "}" || token == "@")
    parse_error("invalid float");

  res = strtod(token.c_str(), &endptr);

  if (endptr != token.c_str() + token.size())  
  {
    parse_error("invalid float");
  }

  return res;
}

void YinParser::assert_token_is_id()
{
  if (token == "," || token == "=" || token == "!" || 
      token == "<" || token == "->" || token == "{" ||
      token == "}" || token == "@")
    parse_error("invalid id");
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
  string msg = "Parse error";
  msg += " (at token: " + token + "): " + reason;
  fail(msg);
}

} /* namespace Yinspire */
