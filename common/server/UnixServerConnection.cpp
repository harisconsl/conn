#include "UnixServerConnection.h"

using namespace IN::COMMON;
boost::asio::local::stream_protocol;
boost::asio::io_context io_context

UnixServerConnection::UnixServerConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , socket_(io_context)
{ }

UnixServerConnection* UnixServerConnection::create(bool is_stream, boost::asio::io_context& io_context)
{
  std::string host = url.get_address();
  if (!host.size())
    {
      LOG_I("Invalid address for unix socket");
      return nullptr;
    }

  UnixServerConnection* connection = new UnixServerConnection(is_stream, io_context);
  connection->m_host = host;
  return connection;
}

UnixServerConnection::open()
{
  boost::asio::stream_protocol::endpoint(m_host);
  socket_.async_connect(endpoint, std::bind(&UnixConnection::handle_connect, this, std::placeholders::_1));
}

void UnixServerConnection::handle_connect(const boost::system::error_code& error)
{
  if (!error)
    {
      LOG_E("Connected to the server!" );
      std::string message = "Hello, Server!";
      boost::asio::async_write(*socket_, boost::asio::buffer(message.c_str(), message.size()),
			       std::bind(&UnixConnection::handle_write, this,
					 std::placeholders::_1, std::placeholders::_2));
    }
  else
    {
      LOG_E( "Connection error: " << error.message() );
    }
}

void UnixServerConnection::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  if (!error)
    {
      LOG_I("Message sent!");
      socket_->async_read_some(boost::asio::buffer(buffer), std::bind(&UnixConnection::handle_read, this,
								      std::placeholders::_1, std::placeholders::_2));
    }
  else
    {
      LOG_E("Write error: " << error.message() );
    }
}

void UnixServerConnection::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  if (!error)
    {
      std::cout << "Received message: " << std::string(buffer.data(), bytes_transferred) << std::endl;
    }
  else
    {
      std::cout << "Read error: " << error.message() << std::endl;
    }
}

void UnixServerConnection::do_close()
{
}

bool UnixServerConnection::is_open()
{
  if (socket_.is_open())
    return true;
  return false;
}

std::string UnixServerConnection::address()
{
  return m_address;
}






