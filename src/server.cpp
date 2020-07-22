#include <server.hpp>

using namespace otus;

void Session::doRead() {
  auto self(shared_from_this());
  socket.async_read_some(
      boost::asio::buffer(data, maxLength),
      [this, self](boost::system::error_code ec, size_t length) {
        if (!ec) {
          async::receive(handle, data, length);
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
