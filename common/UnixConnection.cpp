#include "UnixConnection.h"
#include "Logger.h"

using namespace IN::COMMON;

UnixConnection::UnixConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , socket_(io_context)
{ }

int UnixConnection::get_fd()
{
  LOG_I("UnixConnection::file descriptor");
  return socket_.native_handle();
}

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

bool UnixConnection::connect()
{
  boost::asio::local::stream_protocol::endpoint endpoint(m_address);
  socket_.async_connect(endpoint, std::bind(&UnixConnection::handle_connect, this, std::placeholders::_1));
  LOG_D("Socket connecting to the server!" );
}

void UnixConnection::handle_connect(const boost::system::error_code& error)
{
  if (!error)
    {
      LOG_D("Connected to the server!");
      // std::string message = "Hello, Server!";
      // boost::asio::async_write(socket_, boost::asio::buffer(message.c_str(), message.size()),
      //   		       std::bind(&UnixConnection::handle_write, this,
      //   				 std::placeholders::_1, std::placeholders::_2));
      auto p = m_read_buf.write_buffer();
      socket_.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(p.first), p.second),
                              std::bind(&UnixConnection::handle_read, this,
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
  LOG_I("Message recieved! : " << bytes_transferred);
  if (!error)
    {
      LOG_I("Received message: " << std::string(reinterpret_cast<const char*>(m_read_buf.read_buffer().first),
                                                bytes_transferred) <<"\n");
      m_read_buf.advance_write(bytes_transferred);
      LOG_I("address: " << static_cast<void*>(m_read_buf.read_buffer().first));
      
      // char buf[128];
      // unsigned char* msg_buf = reinterpret_cast<unsigned char*>(m_read_buf.read_buffer().first);
      // for (uint16_t i = 0; i < bytes_transferred; ++i)
      //   {
      //     sprintf(&buf[2*i], "%02x", *(msg_buf+i));
      //   }
      // LOG_I("Recieved msg => " << buf );
      m_read_buf.advance_read(bytes_transferred);
      async_read();
    }
  else
    {
      LOG_E("Read error: " << error.message());
    }
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

bool UnixConnection::is_open()
{
  if (socket_.is_open())
    return true;
  
  return false;
}

std::string UnixConnection::address()
{
  return m_address;
}


void UnixConnection::async_write(const char* buf, std::size_t len)
{
  boost::asio::async_write(socket_, boost::asio::buffer(buf, len),
                           std::bind(&UnixConnection::handle_write, this, std::placeholders::_1, std::placeholders::_2));
}


void UnixConnection::async_read()
{
  LOG_I("address: " << static_cast<void*>(m_read_buf.write_buffer().first));
  auto p = m_read_buf.write_buffer();
  socket_.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(p.first),p.second),
                          std::bind(&UnixConnection::handle_read, this, std::placeholders::_1, std::placeholders::_2));
}




