#include "McastConnection.h"
#include <boost/lexical_cast.hpp>
#include "Logger.h"

using namespace std;
using namespace IN::COMMON;

McastConnection::McastConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
  , socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0))
{ }

int McastConnection::get_fd()
{
  LOG_I("McastConnection::file descriptor");
  return socket_.native_handle();
}

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

  //  char buffer[1024];
  boost::asio::ip::udp::endpoint sender_ep;
  auto p = m_read_buf.write_buffer();
  socket_.async_receive_from(boost::asio::buffer(reinterpret_cast<char*>(p.first),p.second), sender_ep,
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
  LOG_I("Message recieved! : " << bytes_transferred);
  if (!ec)
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
      LOG_E("Read error: " << ec.message());
    }
}

bool McastConnection::is_open()
{
  return true;
}

std::string McastConnection::address()
{
  return m_address + ":" + to_string(m_port);
}

void McastConnection::async_read()
{
  LOG_I("address: " << static_cast<void*>(m_read_buf.write_buffer().first));
  auto p = m_read_buf.write_buffer();
  boost::asio::ip::udp::endpoint sender_ep;
  socket_.async_receive_from(boost::asio::buffer(reinterpret_cast<char*>(p.first),p.second), sender_ep,
                             std::bind(&McastConnection::handle_recv, this,
                                       std::placeholders::_1, std::placeholders::_2));
}
