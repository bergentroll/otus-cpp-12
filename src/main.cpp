#include <iostream>

#include "get_arg.hpp"
#include "parser.hpp"
#include "tee_buffer.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
  int N;
  try {
    N = get_arg(argc, argv);
  } catch (InvalidArgument &e) {
    cerr << "Usage error: " << e.what() << endl;
    return EXIT_FAILURE;
  }

  TeeBuffer teeBuffer { };
  ostream stream { &teeBuffer };
  Parser parser { N, stream };
  string buf { };

  while (!cin.eof()) {
    cin >> buf;
    try {
      parser << buf;
    }
    catch (Parser::InvalidToken &e) {
      cerr << "Error occured: " << e.what() << endl;
    }
  }

  return EXIT_SUCCESS;
}
