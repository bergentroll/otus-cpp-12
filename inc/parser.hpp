#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <vector>

enum class State { root, block };

class Parser {
public:
  Parser(int packSize, std::ostream &stream = std::cout):
  packSize(packSize), stream(stream) { commands.reserve(packSize); }

  Parser* operator <<(std::string const &input) {
    commands.push_back(input);
    if (commands.size() == packSize) flushCommands();
    return this;
  }

private:
  State state { State::root };
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
