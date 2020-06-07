#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Parser {
public:
  class InvalidToken: public std::logic_error {
  public:
    explicit InvalidToken(const std::string &input):
    std::logic_error("unexpected token: " + input) { }
  };

  Parser(int packSize, std::ostream &stream = std::cout):
  packSize(packSize), stream(stream) { commands.reserve(packSize); }

  Parser& operator <<(std::string const &token) {
    handler = handler->readToken(token);
    return *this;
  }

private:
  class Handler; class Plain; class Block; class Nested;
  using HandlerPtr = std::unique_ptr<Handler>;

  class Handler {
  public:
    [[ nodiscard ]]
    virtual HandlerPtr readToken(std::string const & token) = 0;
  };

  class Plain: public Handler {
  public:
    Plain(Parser &p): p(p) { }

    HandlerPtr readToken(std::string const & token) override {
      if (token == "{") {
          if (p.commands.size() > 0) p.flushCommands(); // TODO Notify.
          return HandlerPtr(new Block(p));
      }
      else if (token == "}") {
        throw InvalidToken(token);
      } else {
        p.commands.push_back(token); // TODO Notify.
        if (p.commands.size() >= p.packSize) p.flushCommands();
      }
      return HandlerPtr(new Plain(p));
    }

  private:
    Parser &p;
  };

  class Block: public Handler {
  public:
    Block(Parser &p): p(p) { }

    HandlerPtr readToken(std::string const & token) override {
      if (token == "{") {
        return HandlerPtr(new Nested(p, 1));
      } else if (token == "}") {
        if (p.commands.size() > 0) p.flushCommands();
        return HandlerPtr(new Plain(p));
    } else {
        p.commands.push_back(token);
    }
    return HandlerPtr(new Block(p));
  }

  private:
    Parser &p;
  };

  class Nested: public Handler {
  public:
    Nested(Parser &p, int level): p(p), level(level) { }

    HandlerPtr readToken(std::string const & token) override {
      if (token == "{") {
        ++level;
      } else if (token == "}") {
        --level;
        if (level == 0) return HandlerPtr(new Block(p));
      } else {
        p.commands.push_back(token);
      }
      return HandlerPtr(new Nested(p, level));
    }

  private:
    Parser &p;
    int level;
  };

  HandlerPtr handler { new Plain(*this) };
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
