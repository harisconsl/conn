#ifndef _IN_COMMON_TCP_SERVER_CONNECTION_H_
#define _IN_COMMON_TCP_SERVER_CONNECTION_H_
#include "ServerConnection.h"
#include "Url.h"

namespace IN {
namespace COMMON {

  class TcpServerConnection : public ServerConnection
{
 public:
  static constexpr int DEFAULT_TIME_OUT = 100;
  
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();

  virtual int read(char* buf, int len) { }
  virtual int write(char* buf, int len) { }

  TcpServerConnection* create(const Url& url, const boost::asio::io_context& io_context);

private:
  TcpServerConnection(bool is_stream, boost::asio::io_context& io_context);
  PREVENT_COPY(TcpServerConnection);

  boost::asio::ip::tcp::socket _socket;
  unsigned short m_port;

  void handle_accept(const boost::system::error_code& ec);
  void handle_write(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void do_close();  
};

}}
#endif
