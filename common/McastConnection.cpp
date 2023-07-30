#include "McastConnection.h"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "Logger.h"

using namespace std;
using namespace IN::COMMON;

McastConnection::McastConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0))
{ }

McastConnection* McastConnection::create(const Url& url, boost::asio::io_context& io_context)
{
  std::string host = url.get_address();
  std::string port = url.get_option("port");
  std::string interface_addr = url.get_option("interface_addr");

  if (!host.size() || !port.size())
    {
      LOG_I("Invalid host and/or port");
      return nullptr;
    }

  McastConnection* connection = new McastConnection(false, io_context);

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
  connection->m_interface_addr = interface_addr;

  return connection;
}

void McastConnection::join_group()
{
  boost::asio::ip::address multicast_addr = boost::asio::ip::address::from_string(m_address);
  socket_.set_option(boost::asio::ip::multicast::join_group(multicast_addr));
  socket_.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(m_interface_addr), 0));

  char buffer[1024];
  boost::asio::ip::udp::endpoint sender_ep;
  socket_.async_receive_from(boost::asio::buffer(buffer), sender_ep,
                             std::bind(&McastConnection::handle_recv, this,
                                       std::placeholders::_1, std::placeholders::_2));

}

void McastConnection::leave_group()
{
  boost::asio::ip::address multicastAddr = socket_.local_endpoint().address();
  socket_.set_option(boost::asio::ip::multicast::leave_group(multicastAddr));
}

void McastConnection::handle_recv(const boost::system::error_code& ec, std::size_t bytes_transferred)
{

}
