#ifndef OTUS_WRAPPER_HPP
#define OTUS_WRAPPER_HPP

#include <cstddef>
#include <memory>
#include <ostream>

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
  
    otus::Parser &getParser() { return parser; }
  
    private:
    std::shared_ptr<otus::TeeBuffer> teeBuffer { std::make_shared<TeeBuffer>() };
    std::ostream stream;
    Parser parser;
  };
}

#endif
