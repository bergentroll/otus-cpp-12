#include <iostream>
#include <memory>

#include "get_arg.hpp"
#include "parser.hpp"
#include "tee_buffer.hpp"

using namespace std;
using namespace otus;

int main(int argc, char const *argv[]) {
  int N;
  try {
    N = get_arg(argc, argv);
  } catch (InvalidArgument &e) {
    cerr << "Usage error: " << e.what() << endl;
    return EXIT_FAILURE;
  }

  auto teeBuffer { make_shared<TeeBuffer>() };
  ostream stream { teeBuffer.get() };
  Parser parser { N, stream };
  parser.subscribe(teeBuffer);
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
