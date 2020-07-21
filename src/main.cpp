#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <utility>

#include "async.hpp"

using namespace std;
namespace io = boost::asio;
using io::ip::tcp;
using namespace otus::async;

class Session: public enable_shared_from_this<Session> {
public:
  Session(tcp::socket socket): socket(move(socket)) { }

  void start() {
    doRead();
  }

private:
  void doRead() {
    auto self(shared_from_this());
    socket.async_read_some(
        boost::asio::buffer(data, max_length),
        [this, self](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            auto c { connect(2) };
            receive(c, data, length);
            doWrite(length);
          }
        });
  }

  void doWrite(size_t length) {
    auto self(shared_from_this());
    io::async_write(
      socket,
      io::buffer(data, length),
      [this, self](boost::system::error_code ec, size_t ) { if (!ec) doRead(); });
  }

  tcp::socket socket;
  static constexpr size_t max_length { 1024 };
  char data[max_length];
};

class Server {
public:
  Server(io::io_context &context, short port):
  acceptor(context, tcp::endpoint(tcp::v4(), port)) {
    do_accept();
  }

private:
  void do_accept() {
    acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket) {
          if (!ec) make_shared<Session>(move(socket))->start();
          do_accept();
        });
  }

  tcp::acceptor acceptor;
};

int main() {
  try {
    io::io_context context;
    Server server { context, 8166 };
    context.run();
  }
  catch (const exception& e) { // FIXME
    cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
