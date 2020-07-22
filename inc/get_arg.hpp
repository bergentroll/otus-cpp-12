#ifndef OTUS_GET_ARG_HPP
#define OTUS_GET_ARG_HPP

#include <cstring>
#include <stdexcept>

namespace otus {
  class InvalidArgument: public std::logic_error {
  public:
    explicit InvalidArgument(const std::string &input):
    std::logic_error(input) { }
  };

  int get_arg(int argc, char const **argv, int index);
}

#endif
