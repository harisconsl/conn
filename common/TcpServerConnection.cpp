#include <TcpServerConnection.h>
using namespace std;
using namespace IN::COMMON;

typedef boost::asio::ip bai;

TcpServerConnection::TcpServerConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , socket_(io_context)
{ }

TcpServerConnection* TcpServerConnection::create(const Url& url, boost::asio::io_context& io_context)
{
  //bai::tcp::endpoint ep(bai::address::from_string(127.0.0.1), 8080);
  // socket_ = std::make_shared<bai::tcp::socket>(m_io_context);
  
  std::string port = url.get_option("port");

  if (!host.size() || !port.size())
    {
      LOG_I("Invalid host and/or port");
      return nullptr;
    }

  TcpServerConnection connection = new TcpServerConnection(is_stream, io_context);

  // intialize the private memeberiof the class

  connection->m_port = port;
}

bool TcpServerConnection::open()
{
  //  if (socket_.is_open())
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(m_host), m_port);
  socket_.async_connect(endpoint, std::bind(&TcpConnection::handle_connect, this, std::placeholders::_1));
}


void TcpServerConnection::handle_connect(const boost::system::error_code& error)
{
  if (!error)
    {
      std::cout << "Connected to the server!" << std::endl;
      std::string message = "Hello, Server!";
      boost::asio::async_write(*socket_, boost::asio::buffer(message.c_str(), message.size()),
			       std::bind(&TcpConnection::handle_write, this, std::placeholders::_1, std::placeholders::_2));
    }
  else
    {
      std::cout << "Connection error: " << error.message() << std::endl;
    }
}

void TcpServerConnection::handle_write(const boost::system::error_code& error, std::size_t bytes_transferred)
{
  if (!error)
    {
      std::cout << "Message sent!" << std::endl;
      socket_->async_read_some(boost::asio::buffer(buffer), std::bind(&TcpConnection::handle_read, this, std::placeholders::_1, std::placeholders::_2));
      }
  else
    {
      std::cout << "Write error: " << error.message() << std::endl;
    }
}

void TcpServerConnection::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred)
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

void TcpServerConnection::do_close()
{
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

bool TcpServerConnection::is_open()
{
  if (socket_.is_open())
    return true;
  return false;
}

std::string TcpServerConnection::address()
{
  return m_address + ":" + m_port;
}

