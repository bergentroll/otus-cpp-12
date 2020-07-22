#include <boost/asio.hpp>
#include <exception>
#include <iostream>

#include "server.hpp"

namespace io = boost::asio;
using namespace otus;
using namespace std;

int main() {
  short port { 9000 };
  int bulkSize { 3 };
  try {
    io::io_context context;
    Server server { context, port, bulkSize };
    context.run();
  }
  catch (const exception& e) { // FIXME
    cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
