#include <iostream>
#include <array>
#include <string>
#include <boost/asio.hpp>

std::array<char, 128> buffer;

void handleConnect(const boost::system::error_code& error) {
  if (!error)
    {
      std::cout << "Connected to the server!" << std::endl;
    }
  else
    {
      std::cout << "Connection error: " << error.message() << std::endl;
    }
}

void handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred) {
  if (!error)
    {
      std::cout << "Message sent!" << std::endl;
    }
  else
    {
      std::cout << "Write error: " << error.message() << std::endl;
    }
}

void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred) {
  if (!error)
    {
      std::cout << "Received message: " << std::string(buffer.data(), bytes_transferred) << std::endl;
    }
  else
    {
      std::cout << "Read error: " << error.message() << std::endl;
    }
}

int main()
{
  boost::asio::io_context io_context;
  // Create a TCP socket
  boost::asio::ip::tcp::socket socket(io_context);
  // Connect to the server
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080);
  socket.async_connect(endpoint, handleConnect);
  // Send a message to the server
  std::string message = "Hello, Server!";
  boost::asio::async_write(socket, boost::asio::buffer(message), handleWrite);
  // Receive a response from the server
  socket.async_read_some(boost::asio::buffer(buffer), handleRead);
  // Run the IO service
  io_context.run();
  return 0;
}
