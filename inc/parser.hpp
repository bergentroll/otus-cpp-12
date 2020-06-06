#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <vector>

enum class State { plain, block, nested };

class Parser {
public:
  Parser(int packSize, std::ostream &stream = std::cout):
  packSize(packSize), stream(stream) { commands.reserve(packSize); }

  // TODO Lexeme.
  // TODO Verification.
  Parser& operator <<(std::string const &token) {
    if (token == "{") {
      if (state == State::plain) {
        if (commands.size() > 0) flushCommands();
        state = State::block;
      } else if (state == State::block) {
        state = State::nested;
        ++nestingLevel;
      } else if (state == State::nested) {
        ++nestingLevel;
      }
    } else if (token == "}") {
      if (state == State::block) {
        if (commands.size() > 0) flushCommands();
        state = State::plain;
      } else if (state == State::nested) {
        --nestingLevel;
        if (nestingLevel == 0) state = State::block;
      }
    } else {
      commands.push_back(token);
      if (state == State::plain && commands.size() >= packSize) flushCommands();
    }

    return *this;
  }

  State getState() { return state; }

private:
  State state { State::plain };
  int nestingLevel { };
  std::size_t const packSize;
  std::ostream &stream;
  std::vector<std::string> commands;

  void flushCommands() {
    stream << "bulk: ";
    for (size_t i { }; i < commands.size(); ++i) {
      stream << commands[i];
      if (i < commands.size() - 1) stream << ", ";
    }
    stream << std::endl;
    commands.clear();
  }
};

#endif
