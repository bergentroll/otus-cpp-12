#include <iostream>
#include <stdexcept>
#include <cstring>

#include "parser.hpp"

using namespace std;

class InvalidArgument: public std::logic_error
{
public:
  explicit InvalidArgument(const std::string &input):
  std::logic_error(input) { }
};


// TODO Separate.
// TODO Test.
int get_arg(int argc, char *argv[]) {
  int N;
  size_t idx;

  if (argc != 2) throw(InvalidArgument("one agument expected"));

  try {
    N = stoi(argv[1], &idx);
  } catch (invalid_argument &e) {
    throw InvalidArgument("argument must be integer");
  } catch (out_of_range &e) {
    throw InvalidArgument("argument is out of range");
  }

  if (idx < strlen(argv[1])) {
    throw InvalidArgument("argument must be integer");
  }
  if (N < 1) {
    throw InvalidArgument("argument must be greater than zero");
  }

  return N;
}

int main(int argc, char *argv[]) {
  int N;
  try {
    N = get_arg(argc, argv);
  } catch (InvalidArgument &e) {
    cerr << "Usage error: " << e.what() << endl;
    return EXIT_FAILURE;
  }

  Parser parser { N };
  string buf { };

  while (true) {
    cin >> buf;
    parser << buf;
  }

  return EXIT_SUCCESS;
}
