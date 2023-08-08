#include "UnixServerConnection.h"
#include "Logger.h"
#include <boost/lexical_cast.hpp>

using namespace IN::COMMON;

UnixServerConnection::UnixServerConnection(bool is_stream, boost::asio::io_context& io_context, std::string path)
  : ServerConnection(is_stream)
  , acceptor_(io_context, boost::asio::local::stream_protocol::endpoint(path))
    //  , m_address(path)
{ }

UnixServerConnection* UnixServerConnection::create(const Url& url, boost::asio::io_context& io_context)
{
  std::string host = url.get_address();
  if (!host.size())
    {
      LOG_I("Invalid address for unix socket");
      return nullptr;
    }

  UnixServerConnection* connection = new UnixServerConnection(true, io_context, host);
  return connection;
}

bool UnixServerConnection::start_accept()
{
  std::shared_ptr< boost::asio::local::stream_protocol::socket> socket =
    std::make_shared< boost::asio::local::stream_protocol::socket>(acceptor_.get_executor());

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

  // boost::asio::stream_protocol::endpoint(m_address);
  // socket_.async_connect(endpoint, std::bind(&UnixServerConnection::handle_connect, this, std::placeholders::_1));
}

void UnixServerConnection::handle_client(std::shared_ptr< boost::asio::local::stream_protocol::socket> socket)
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

void UnixServerConnection::do_close()
{
}

bool UnixServerConnection::is_open()
{
  return true;
}

std::string UnixServerConnection::address()
{
  return m_address;
}
