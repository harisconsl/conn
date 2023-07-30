#include "UnixConnection.h"
#include "Logger.h"

using namespace IN::COMMON;
boost::asio::local::stream_protocol stream_protocol;
boost::asio::io_context io_context;

UnixConnection::UnixConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , socket_(io_context)
{ }

UnixConnection* UnixConnection::create(const Url& url, boost::asio::io_context& io_context)
{
  std::string host = url.get_address();
  if (!host.size())
    {
      LOG_I("Invalid address for unix socket");
      return nullptr;
    }

  UnixConnection* connection = new UnixConnection(true, io_context);
  connection->m_address = host;
  return connection;
}

bool UnixConnection::is_open()
{
  boost::asio::local::stream_protocol::endpoint endpoint(m_address);
  socket_.async_connect(endpoint, std::bind(&UnixConnection::handle_connect, this, std::placeholders::_1));
}

void UnixConnection::handle_connect(const boost::system::error_code& error)
{
  if (!error)
    {
      LOG_D("Connected to the server!");
      std::string message = "Hello, Server!";
      boost::asio::async_write(socket_, boost::asio::buffer(message.c_str(), message.size()),
			       std::bind(&UnixConnection::handle_write, this,
					 std::placeholders::_1, std::placeholders::_2));
    }
  else
    {
      LOG_E("Connection error: " << error.message() );
    }
}

void UnixConnection::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  char buffer[1024];
  if (!error)
    {
      LOG_D("Message sent!");
      socket_.async_read_some(boost::asio::buffer(buffer), std::bind(&UnixConnection::handle_read, this,
								      std::placeholders::_1, std::placeholders::_2));
    }
  else
    {
      LOG_E( "Write error: " << error.message());
    }
}

void UnixConnection::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  // char buffer[1024];
  // if (!error)
  //   {
  //     std::cout << "Received message: " << std::string(buffer.data(), bytes_transferred) << std::endl;
  //   }
  // else
  //   {
  //     std::cout << "Read error: " << error.message() << std::endl;
  //   }
}

void UnixConnection::do_close()
{
  socket_.cancel(); // Cancel any pending asynchronous operations
  // 3. Close the socket
  boost::system::error_code ec;
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  if (ec) {
    // Handle error
    LOG_E( "Socket close error 1: " << ec.message());
  }
  socket_.close(ec);
  if (ec)
    {
      // Handle error
      LOG_E("Socket close error 2: " << ec.message());
    }
}

std::string UnixConnection::address()
{
  return m_address;
}






