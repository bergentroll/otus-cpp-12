#include <boost/asio.hpp>
#include <exception>
#include <iostream>

#include "get_arg.hpp"
#include "server.hpp"

using namespace otus;
using namespace std;
namespace io = boost::asio;

int main(int argc, char const **argv) {
  if (argc != 3) {
    cerr << "two aguments expected" << endl;
    return EXIT_FAILURE;
  }

  int port, bulkSize;

  try {
    port = get_arg(argc, argv, 1);
    bulkSize = get_arg(argc, argv, 2);
  } catch (InvalidArgument const &e) {
    cerr << "Exception while arguments parsion: " << e.what() << endl;
  }

  io::io_service context;
  Server server { context, port, bulkSize };
  context.run();

  return 0;
}
