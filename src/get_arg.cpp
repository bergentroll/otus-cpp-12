#include "get_arg.hpp"

using namespace std;

int otus::get_arg(int argc, char const *argv[], int index) {
  int val;
  size_t idx;

  try {
    val = stoi(argv[index], &idx);
  } catch (invalid_argument &e) {
    throw InvalidArgument("argument must be integer");
  } catch (out_of_range &e) {
    throw InvalidArgument("argument is out of range");
  }

  if (idx < strlen(argv[index])) {
    throw InvalidArgument("argument must be integer");
  }
  if (val < 1) {
    throw InvalidArgument("argument must be greater than zero");
  }

  return val;
}
