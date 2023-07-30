#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <sys/epoll.h>

constexpr int max_events = 10;

class Server
{
public:
  Server(boost::asio::io_context& io_context, int port)
    : io_context_(io_context),
      acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
      epoll_fd_(epoll_create1(0))
  {
    listen();
    run();
  }

private:
  void listen()
  {
    boost::asio::ip::tcp::socket socket(io_context_);
    acceptor_.async_accept(socket, [this, &socket](const boost::system::error_code& error) {
      if (!error) {
	// New client connection accepted
	std::cout << "New client connected: " << socket.remote_endpoint() << std::endl;
	
	// Register the socket with epoll
	boost::asio::posix::stream_descriptor descriptor(io_context_, socket.native_handle());
	struct epoll_event event;
	event.events = EPOLLIN | EPOLLET;  // Set desired event flags
	event.data.ptr = nullptr;  // Use data.ptr to store custom data if needed
	epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, descriptor.native_handle(), &event);

	// Start listening for data on the socket
	startListening(socket);
      }
      
      // Continue listening for new connections
      listen();
    });
  }

  void startListening(boost::asio::ip::tcp::socket& socket)
  {
    auto buffer = std::make_shared<boost::asio::streambuf>();
    
    boost::asio::async_read_until(socket, *buffer, '\n', [this, &socket, buffer](const boost::system::error_code& error, std::size_t bytes_transferred) {
      if (!error) {
	// Data received from the client
	std::string data(boost::asio::buffers_begin(buffer->data()), boost::asio::buffers_begin(buffer->data()) + bytes_transferred);
	std::cout << "Received from client: " << data;

	// Echo the received data back to the client
	boost::asio::async_write(socket, boost::asio::buffer(buffer->data(), bytes_transferred),
				 [this, &socket, buffer](const boost::system::error_code& error, std::size_t bytes_transferred) {
				   if (!error) {
				     // Continue listening for more data
				     startListening(socket);
				   }
				 }
				 );
      }
      else
      {
	// Error or client disconnected, close the connection
	std::cout << "Client disconnected: " << socket.remote_endpoint() << std::endl;
	socket.close();
      }
    });
  }

  void run()
  {
    struct epoll_event events[max_events];
    while (true)
      {
	int num_events = epoll_wait(epoll_fd_, events, max_events, -1);
	for (int i = 0; i < num_events; ++i) {
	  if (events[i].events & EPOLLIN)
	    {
	      auto descriptor = static_cast<boost::asio::posix::stream_descriptor*>(events[i].data.ptr);
	      boost::asio::ip::tcp::socket socket(io_context_, descriptor->native_handle());
	      
	      // Handle incoming data on the socket
	      startListening(socket);
	    }
	}
      }
    }

  boost::asio::io_context& io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  int epoll_fd_;
};

int main() {
  try {
    boost::asio::io_context io_context;
    Server server(io_context, 8080);
    io_context.run();
  }
  catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  
  return 0;
}
