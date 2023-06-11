#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <iostream>
#include <boost/asio.hpp>

class Server {
public:
  Server(boost::asio::io_context& ioContext, unsigned short port)
    : acceptor_(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
  {
    startAccept();
  }

private:
  void startAccept()
  {
    std::shared_ptr<boost::asio::ip::tcp::socket> socket =
      std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor());

    // Initiate asynchronous accept operation
    acceptor_.async_accept(*socket,
			   [this, socket](const boost::system::error_code& error) {
			     if (!error)
			       {
				 // Handle the client connection
				 handleClient(std::move(socket));
			       }
			     // Start accepting the next client
			     startAccept();
			   });
  }

  void handleClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
  {
    // Read data from the client asynchronously
    socket->async_read_some(boost::asio::buffer(buffer_),
			    [this,socket](const boost::system::error_code& error, std::size_t bytesTransferred) {
			      if (!error)
				{
				  // Process the received data
				  std::cout << "Received data from client: " << std::string(buffer_.data(), bytesTransferred) << std::endl;

				  // Echo the data back to the client
				  boost::asio::async_write(*socket, boost::asio::buffer(buffer_, bytesTransferred),
							   [this, socket](const boost::system::error_code& error, std::size_t bytesTransferred) {
							     if (!error) {
							       // Continue reading data from the client
							       handleClient(socket);
							     }
							     else
							       {
								 std::cout << "Write error " << error.message() << std::endl;
							       }
							   });

				}
			      else
				{
				  std::cout << "Read error " << error.message() << std::endl;
				}
			    });
  }

private:
  boost::asio::ip::tcp::acceptor acceptor_;
  std::array<char, 1024> buffer_;
};

int main()
{
  boost::asio::io_context ioContext;
  unsigned short port = 8080;
  try
    {
      Server server(ioContext, port);
      // Start the IO context in a separate thread
      std::thread ioThread([&ioContext]() {
	ioContext.run();
      });

      std::cout << "Server started on port " << port << std::endl;
      // Wait for the server to finish
      ioThread.join();
    }
  catch (const std::exception& e)
    {
      std::cerr << "Exception in server: " << e.what() << std::endl;
    }
  return 0;
}
