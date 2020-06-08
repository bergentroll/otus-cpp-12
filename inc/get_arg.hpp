#ifndef GET_ARG_HPP
#define GET_ARG_HPP

#include <cstring>
#include <stdexcept>

class InvalidArgument: public std::logic_error {
public:
  explicit InvalidArgument(const std::string &input):
  std::logic_error(input) { }
};

// TODO Test.
int get_arg(int argc, char *argv[]) {
  int N;
  size_t idx;

  if (argc != 2) throw(InvalidArgument("one agument expected"));

  try {
    N = std::stoi(argv[1], &idx);
  } catch (std::invalid_argument &e) {
    throw InvalidArgument("argument must be integer");
  } catch (std::out_of_range &e) {
    throw InvalidArgument("argument is out of range");
  }

  if (idx < std::strlen(argv[1])) {
    throw InvalidArgument("argument must be integer");
  }
  if (N < 1) {
    throw InvalidArgument("argument must be greater than zero");
  }

  return N;
}

#endif
