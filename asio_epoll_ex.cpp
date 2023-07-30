#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <sys/epoll.h>

int main()
{
  boost::asio::io_context io_context;
  boost::asio::posix::stream_descriptor descriptor(io_context);
  int epoll_fd = epoll_create1(0);

    // Create a socket and connect it to a remote endpoint
  boost::asio::ip::tcp::socket socket(io_context);
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080);
  socket.connect(endpoint);

    // Register the socket's native descriptor with epoll
  descriptor.assign(socket.native_handle());
  struct epoll_event event;
  event.events = EPOLLIN | EPOLLET;  // Set desired event flags
  event.data.ptr = nullptr;  // Use data.ptr to store custom data if needed
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, descriptor.native_handle(), &event);
  
  constexpr int max_events = 10;
  struct epoll_event events[max_events];

  while (true)
    {
      int num_events = epoll_wait(epoll_fd, events, max_events, -1);
      for (int i = 0; i < num_events; ++i)
	{
	  if (events[i].events & EPOLLIN)
	    {
	      if (events[i].data.ptr == nullptr) {
		// Handle socket event
		std::array<char, 1024> buffer;
		boost::system::error_code error;
		size_t bytes_read = socket.read_some(boost::asio::buffer(buffer), error);
		if (!error) {
		  // Process the received data
		  std::cout << "Received: " << std::string(buffer.data(), bytes_read) << std::endl;
		}
		else
	        {
		  // Handle read error
		  std::cerr << "Read error: " << error.message() << std::endl;
		}
	      }
	      else
	      {
		// Handle other custom events if needed
		// Custom data can be accessed through events[i].data.ptr
	      }
	    }
	}
    }
  
  return 0;
}
