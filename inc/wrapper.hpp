#ifndef OTUS_WRAPPER_HPP
#define OTUS_WRAPPER_HPP

#include <cstddef>
#include <memory>
#include <ostream>
#include <string>

#include "parser.hpp"
#include "tee_buffer.hpp"

namespace otus {
  class Wrapper {
    public:
    Wrapper(size_t bulk):
    stream(std::ostream(teeBuffer.get())),
    parser(bulk, stream) {
      parser.subscribe(teeBuffer);
    }

    void receive(std::string const &input) {
      ss << input;
      std::string buf { };
      while (ss) {
        ss >> buf;
        if (!buf.empty()) parser << buf;
        buf.clear();
      }
      ss.clear();
    }

    std::stringstream ss { };
    private:
    std::shared_ptr<otus::TeeBuffer> teeBuffer { std::make_shared<TeeBuffer>() };
    std::ostream stream;
    Parser parser;
  };
}

#endif
