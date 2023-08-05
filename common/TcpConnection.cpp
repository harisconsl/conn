#include "TcpConnection.h"
#include "Logger.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace IN::COMMON;

TcpConnection::TcpConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , socket_(io_context)
{ }

int TcpConnection::get_fd()
{
  LOG_I("TcpConnection::file descriptor");
  return socket_.native_handle();
}


TcpConnection* TcpConnection::create(const Url& url, boost::asio::io_context& io_context)
{ 
  std::string host = url.get_address();
  std::string port = url.get_option("port");

  if (!host.size() || !port.size())
    {
      LOG_I("Invalid host : " << host << "  and/or port : " << port);
      return nullptr;
    }

  TcpConnection* connection = new TcpConnection(true, io_context);

  // intialize the private memeberiof the class
  connection->m_address = host;
  try
    {
      connection->m_port = boost::lexical_cast<unsigned short>(port);
    }
  catch(const boost::bad_lexical_cast & ex)
    {
      LOG_E("bad lexical cast of port " << ex.what());
      return nullptr;
    }

  return connection;
}

bool TcpConnection::connect()
{
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_address), m_port);
  socket_.async_connect(endpoint, std::bind(&TcpConnection::handle_connect, this, std::placeholders::_1));
  LOG_D("Socket connecting to the server!" );
}

void TcpConnection::handle_connect(const boost::system::error_code& error)
{
  LOG_I("handle_connect to the server!" );  
  if (!error)
    {
      LOG_I("Connected to the server!" );
      // std::string message = "Hello, Server!";
      // boost::asio::async_write(socket_, boost::asio::buffer(message.c_str(), message.size()),
      //   		       std::bind(&TcpConnection::handle_write, this, std::placeholders::_1, std::placeholders::_2));
      //      char buffer[1024];
      socket_.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(m_read_buf.write_buffer().first), m_read_buf.write_buffer().second),
                              std::bind(&TcpConnection::handle_read, this, std::placeholders::_1, std::placeholders::_2));

    }
  else
    {
      LOG_E("Connection error: " << error.message() );
    }
}

void TcpConnection::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  char buffer[1024];
  if (!error)
    {
      LOG_I("Message sent!");
      socket_.async_read_some(boost::asio::buffer(buffer), std::bind(&TcpConnection::handle_read, this, std::placeholders::_1, std::placeholders::_2));
    }
  else
    {
      LOG_E("Write error: " << error.message() );
    }
}

void TcpConnection::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
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

void TcpConnection::do_close()
{
  socket_.cancel(); // Cancel any pending asynchronous operations
  // 3. Close the socket
  boost::system::error_code ec;
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  if (ec) {
    // Handle error
    LOG_E("Socket close error 1: " << ec.message() );
  }
  socket_.close(ec);
  if (ec) {
    // Handle error
    LOG_E("Socket close error 2: " << ec.message() );
  }
}

bool TcpConnection::is_open()
{
  if (socket_.is_open())
    return true;

  return false;
}

std::string TcpConnection::address()
{
  return m_address + ":" + to_string(m_port);
}

void TcpConnection::async_write(const char* buf, std::size_t len)
{
  boost::asio::async_write(socket_, boost::asio::buffer(buf, len),
                           std::bind(&TcpConnection::handle_write, this, std::placeholders::_1, std::placeholders::_2));
}


void TcpConnection::async_read()
{
  LOG_I("address: " << static_cast<void*>(m_read_buf.write_buffer().first));
  auto p = m_read_buf.write_buffer();
  socket_.async_read_some(boost::asio::buffer(reinterpret_cast<char*>(p.first),p.second),
                          std::bind(&TcpConnection::handle_read, this, std::placeholders::_1, std::placeholders::_2));
}
