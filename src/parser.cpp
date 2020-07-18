#include "parser.hpp"

using namespace std;
using namespace otus;

Parser::HandlerPtr Parser::Plain::readToken(string const & token) {
  if (token == "{") {
      if (parser.getBufferSize() > 0) parser.flushCommands();
      return HandlerPtr(new Block(parser));
  }
  else if (token == "}") {
    throw InvalidToken(token);
  } else {
    parser.commands.push_back(token);
    if (parser.getBufferSize() >= parser.packSize) parser.flushCommands();
  }
  return HandlerPtr(new Plain(parser));
}

Parser::HandlerPtr Parser::Block::readToken(string const & token) {
  if (token == "{") {
    return HandlerPtr(new Nested(parser, 1));
  } else if (token == "}") {
    if (parser.getBufferSize() > 0) parser.flushCommands();
    return HandlerPtr(new Plain(parser));
  } else {
      parser.commands.push_back(token);
  }
  return HandlerPtr(new Block(parser));
}

Parser::HandlerPtr Parser::Nested::readToken(string const & token) {
  if (token == "{") {
    ++level;
  } else if (token == "}") {
    --level;
    if (level == 0) return HandlerPtr(new Block(parser));
  } else {
    parser.commands.push_back(token);
  }
  return HandlerPtr(new Nested(parser, level));
}

void Parser::flushCommands() {
  notify();
  stream << "bulk: ";
  for (size_t i { }; i < commands.size(); ++i) {
    stream << commands[i];
    if (i < commands.size() - 1) stream << ", ";
  }
  stream << endl;
  commands.clear();
}
