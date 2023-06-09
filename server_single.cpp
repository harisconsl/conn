#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

std::array<char, 128> buffer;
boost::asio::io_context io_context;

class TcpServer
{
public:
  TcpServer(std::shared_ptr<boost::asio::ip::tcp::socket> socket, boost::asio::ip::tcp::acceptor& acceptor)
   :socket_(socket)
   , acceptor_(acceptor)
  { }

  void handleAccept(const boost::system::error_code& error)
  {
    if (!error)
      {
	std::cout << "Accepted a connection from a client!" << std::endl;	
	// Start reading from the socket
	// std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
	socket_->async_read_some(boost::asio::buffer(buffer), std::bind(&TcpServer::handleRead, this, std::placeholders::_1, std::placeholders::_2));
      }
    else
      {
	std::cout << "Accept error: " << error.message() << std::endl;
      }
  }

  void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
  {
    if (!error)
      {
	std::cout << "Received message from client: " << std::string(buffer.data(), bytes_transferred) << std::endl;
	// Echo the received message back to the client
	boost::asio::async_write(*socket_, boost::asio::buffer(buffer, bytes_transferred), std::bind(&TcpServer::handleWrite, this, std::placeholders::_1, std::placeholders::_2));
      }
    else
      {
	std::cout << "Read error: " << error.message() << std::endl;
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
	
	socket_ = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
	acceptor_.async_accept(*socket_,std::bind(&TcpServer::handleAccept, this, std::placeholders::_1));
      }
  }

  void handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred)
  {
    if (!error)
      {
	std::cout << "Sent response to client!" << std::endl;
	// Continue reading from the socket
	socket_->async_read_some(boost::asio::buffer(buffer), std::bind(&TcpServer::handleRead, this, std::placeholders::_1, std::placeholders::_2));
      }
    else
      {
	std::cout << "Write error: " << error.message() << std::endl;
      }
  }
 public:
  std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
  boost::asio::ip::tcp::acceptor& acceptor_;
};

int main()
{
  // Create an acceptor to listen for incoming connections
  boost::asio::ip::tcp::acceptor acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));
  std::cout << "Server started. Listening on port 8080." << std::endl;

  // Create a socket and initiate an asynchronous accept operation
  std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
  TcpServer t1(socket, acceptor);
  acceptor.async_accept(*socket, std::bind(&TcpServer::handleAccept, &t1, std::placeholders::_1));
      
  // Run the IO service to handle asynchronous operations
  io_context.run();
  
  return 0;
}




