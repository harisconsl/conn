#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

class EchoServer {
public:
  EchoServer(io_context& ios, short port)
    : acceptor_(ios, ip::tcp::endpoint(ip::tcp::v4(), port)),
      socket_(ios) {
    StartAccept();
  }

private:
  void StartAccept() {
    acceptor_.async_accept(socket_,
                           [this](boost::system::error_code ec) {
                             if (!ec) {
                               std::cout << "Client connected." << std::endl;
                               StartRead();
                             }

                             // Continue accepting more connections
                             StartAccept();
                           });
  }

  void StartRead() {
    socket_.async_read_some(buffer(data_),
                            [this](boost::system::error_code ec, std::size_t length) {
                              if (!ec) {
                                std::cout << "Received: " << data_ << std::endl;
                                StartWrite(length);
                              }
                            });
  }

  void StartWrite(std::size_t length) {
    async_write(socket_, buffer(data_, length),
                [this](boost::system::error_code ec, std::size_t /*length*/) {
                  if (!ec) {
                    std::cout << "Sent: " << data_ << std::endl;
                    StartRead();
                  }
                });
  }

private:
  ip::tcp::acceptor acceptor_;
  ip::tcp::socket socket_;
  char data_[1024];
};

int main() {
  boost::asio::io_context io_context;
  EchoServer server(io_context, 41080);

  std::thread loopThread([&io_context]() {
      while(true)
        {
          io_context.run_one();  // Run one asynchronous operation
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });
  loopThread.join();

  return 0;
}
