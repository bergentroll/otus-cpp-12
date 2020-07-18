#ifndef OTUS_PARSER_HPP
#define OTUS_PARSER_HPP

#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "observer.hpp"

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

    ~Parser() {
      if (getBufferSize() > 0) flushCommands();
    }

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

      HandlerPtr readToken(std::string const & token) override;

    private:
      Parser &parser;
    };

    class Block: public Handler {
    public:
      Block(Parser &parser): parser(parser) { }

      HandlerPtr readToken(std::string const & token) override;

    private:
      Parser &parser;
    };

    class Nested: public Handler {
    public:
      Nested(Parser &parser, int level): parser(parser), level(level) { }

      HandlerPtr readToken(std::string const & token) override;

    private:
      Parser &parser;
      int level;
    };

    HandlerPtr handler { new Plain(*this) };
    std::size_t const packSize;
    std::ostream &stream;
    std::vector<std::string> commands;

    void flushCommands();
  };
}

#endif
