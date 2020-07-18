#ifndef OTUS_WRAPPER_HPP
#define OTUS_WRAPPER_HPP

#include <cstddef>
#include <future>
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

    ~Wrapper() {
      if (asyncResult.valid()) asyncResult.wait();
    }

    void receive(std::string const &input) {
      if (asyncResult.valid()) asyncResult.wait();
      asyncResult = std::async(&Wrapper::asyncCall, this, input);
    }

    std::stringstream ss { };
    private:
    std::shared_ptr<otus::TeeBuffer> teeBuffer { std::make_shared<TeeBuffer>() };
    std::ostream stream;
    Parser parser;
    std::future<void> asyncResult { };

    void asyncCall(std::string const &input);
  };
}

#endif
