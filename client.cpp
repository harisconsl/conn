#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <iostream>
#include <array>
#include <string>
#include <boost/asio.hpp>

std::array<char, 128> buffer;

class TcpClient
{
public:
  TcpClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
    : socket_(socket)
  { }
  
  void handleConnect(const boost::system::error_code& error)
  {
    if (!error)
      {
	std::cout << "Connected to the server!" << std::endl;
	std::string message = "Hello, Server!";
	boost::asio::async_write(*socket_, boost::asio::buffer(message.c_str(), message.size()),std::bind(&TcpClient::handleWrite, this, std::placeholders::_1, std::placeholders::_2));
      }
    else
      {
	std::cout << "Connection error: " << error.message() << std::endl;
      }
  }

  void handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred)
  {
    if (!error)
      {
	std::cout << "Message sent!" << std::endl;
	socket_->async_read_some(boost::asio::buffer(buffer), std::bind(&TcpClient::handleRead, this, std::placeholders::_1, std::placeholders::_2));
      }
    else
      {
	std::cout << "Write error: " << error.message() << std::endl;
      }
  }

  void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
  {
    if (!error)
      {
	std::cout << "Received message: " << std::string(buffer.data(), bytes_transferred) << std::endl;
	
	socket_->cancel(); // Cancel any pending asynchronous operations

	// 3. Close the socket
	boost::system::error_code ec;
	socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	if (ec) {
	  // Handle error
	  std::cout << "Socket close error 1: " << ec.message() << std::endl;
	}
	socket_->close(ec);
	if (ec) {
	  // Handle error
	  std::cout << "Socket close error 2: " << ec.message() << std::endl;

	}
      }
    else
      {
	std::cout << "Read error: " << error.message() << std::endl;
      }
  }
public:
  std::shared_ptr<boost::asio::ip::tcp::socket> socket_;

};

int main()
{
  boost::asio::io_context io_context;
  // Create a TCP socket
  std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
  // Connect to the server
  TcpClient t(socket);
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 41080);
  socket->async_connect(endpoint, std::bind(&TcpClient::handleConnect, &t,
					    std::placeholders::_1));
  // Send a message to the server
  std::string message = "Hello, Server!";
  // Run the IO service
  std::thread th([&io_context](){io_context.run();});

  // while(true)
  //   {

  //     //      t.write_msg();
  //   }

  th.join();
  return 0;
}
