#include <iostream>
#include <server.hpp>

using namespace otus;
using namespace std;

void Session::doRead() {
  auto self(shared_from_this());
  socket.async_read_some(
      boost::asio::buffer(data, maxLength),
      [this, self](boost::system::error_code ec, size_t length) {
        if (!ec) {
          try {
            async::receive(handle, data, length);
          } catch (Parser::InvalidToken const &e) {
            cerr << "Invalid token: " << e.what() << endl;
          }
          doRead();
        }
      });
}

void Server::doAccept() {
  acceptor.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) std::make_shared<Session>(std::move(socket), bulkSize)->start();
        doAccept();
      });
}
