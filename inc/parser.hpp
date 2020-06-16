#ifndef OTUS_PARSER_HPP
#define OTUS_PARSER_HPP

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <observer.hpp>

namespace otus {
  class Parser: public Observable {
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

    size_t getBufferSize() { return commands.size(); }

  private:
    class Handler; class Plain; class Block; class Nested;
    using HandlerPtr = std::unique_ptr<Handler>;

    class Handler {
    public: virtual ~Handler() = default;

      [[ nodiscard ]]
      virtual HandlerPtr readToken(std::string const & token) = 0;
    };

    class Plain: public Handler {
    public:
      Plain(Parser &parser): parser(parser) { }

      HandlerPtr readToken(std::string const & token) override {
        if (token == "{") {
            if (parser.getBufferSize() > 0) parser.flushCommands();
            return HandlerPtr(new Block(parser));
        }
        else if (token == "}") {
          throw InvalidToken(token);
        } else {
          if (parser.getBufferSize() == 0) parser.notify();
          parser.commands.push_back(token);
          if (parser.getBufferSize() >= parser.packSize) parser.flushCommands();
        }
        return HandlerPtr(new Plain(parser));
      }

    private:
      Parser &parser;
    };

    class Block: public Handler {
    public:
      Block(Parser &parser): parser(parser) { }

      HandlerPtr readToken(std::string const & token) override {
        if (token == "{") {
          return HandlerPtr(new Nested(parser, 1));
        } else if (token == "}") {
          if (parser.getBufferSize() > 0) parser.flushCommands();
          return HandlerPtr(new Plain(parser));
      } else {
          if (parser.getBufferSize() == 0) parser.notify();
          parser.commands.push_back(token);
      }
      return HandlerPtr(new Block(parser));
    }

    private:
      Parser &parser;
    };

    class Nested: public Handler {
    public:
      Nested(Parser &parser, int level): parser(parser), level(level) { }

      HandlerPtr readToken(std::string const & token) override {
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

    private:
      Parser &parser;
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
  }

#endif
