#ifndef OTUS_SERVER_HPP
#define OTUS_SERVER_HPP

#include "async.hpp"
#include <boost/asio.hpp>
#include <memory>
#include <utility>

namespace otus {
  using boost::asio::ip::tcp;

  class Session: public std::enable_shared_from_this<Session> {
  public:
    Session(tcp::socket socket, int bulkSize):
    socket(std::move(socket)), handle(async::connect(bulkSize)) { }

    ~Session() { async::disconnect(handle); }

    void start() { doRead(); }

  private:
    void doRead();

    tcp::socket socket;
    static constexpr size_t maxLength { 1024 };
    char data[maxLength];
    async::handle_t handle;
  };

  class Server {
  public:
    Server(boost::asio::io_service &context, int port, int bulkSize):
    socket(context),
    acceptor(context, tcp::endpoint(tcp::v4(), port)), bulkSize(bulkSize) {
      doAccept();
    }

  private:
    void doAccept();

    tcp::socket socket;
    tcp::acceptor acceptor;
    int bulkSize;
  };
}

#endif
