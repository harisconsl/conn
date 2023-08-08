#include "TcpServerConnection.h"
#include "Logger.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace IN::COMMON;

TcpServerConnection::TcpServerConnection(bool is_stream, boost::asio::io_context& io_context, int port)
  : ServerConnection(is_stream)
  , m_port(port)
  , acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{ }

TcpServerConnection* TcpServerConnection::create(const Url& url, boost::asio::io_context& io_context)
{
  std::string host = url.get_address();
  std::string port = url.get_option("port");

  if (!host.size() || !port.size())
    {
      LOG_I("Invalid host and/or port");
      return nullptr;
    }

  // intialize the private memeberiof the class
  int uport = -1;
  try
    {
      int uport = boost::lexical_cast<unsigned short>(port);
    }
  catch(const boost::bad_lexical_cast & ex)
    {
      LOG_E("bad lexical cast of port " << ex.what());
      return nullptr;
    }

  TcpServerConnection* connection = new TcpServerConnection(true, io_context, uport);  
  return connection;
}

void TcpServerConnection::start_accept()
{
    std::shared_ptr<boost::asio::ip::tcp::socket> socket =
      std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor());

    // Initiate asynchronous accept operation
    acceptor_.async_accept(*socket,
			   [this, socket](const boost::system::error_code& error) {
			     if (!error)
			       {
				 // Handle the client connection
				 handle_client(std::move(socket));
			       }
			     // Start accepting the next client
			     start_accept();
			   });
}


void TcpServerConnection::handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
  std::array<char,128> buffer_;
  socket->async_read_some(boost::asio::buffer(buffer_),
                          [this,socket, buffer_](const boost::system::error_code& error, std::size_t bytesTransferred) {
                            if (!error)
                              {
                                // Process the received data
                                LOG_I("Received data : " << std::string(buffer_.data(), bytesTransferred));
                                
                                // Echo the data back to the client
                                boost::asio::async_write(*socket, boost::asio::buffer(buffer_, bytesTransferred),
                                                         [this, socket](const boost::system::error_code& error, std::size_t bytesTransferred) {
                                                           if (!error) {
                                                             // Continue reading data from the client
                                                             handle_client(socket);
                                                           }
                                                           else
                                                             {
                                                               LOG_E("Write error " << error.message());
                                                             }
                                                         });
                                
                              }
                            else
                              {
                                LOG_E("Read error " << error.message());
                              }
                          });

}

void TcpServerConnection::do_close()
{
  // socket_.cancel(); // Cancel any pending asynchronous operations
  // // 3. Close the socket
  // boost::system::error_code ec;
  // socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  // if (ec) {
  //   // Handle error
  //   LOG_E( "Socket close error 1: " << ec.message());
  // }
  // socket_.close(ec);
  // if (ec) {
  //   // Handle error
  //   LOG_E("Socket close error 2: " << ec.message());
  // }
}

bool TcpServerConnection::is_open()
{
  return true;
}

std::string TcpServerConnection::address()
{
  return m_address + ":" + to_string(m_port);
}

