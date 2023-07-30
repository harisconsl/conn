#ifndef _IN_COMMON_TCP_CONNECTION_H_
#define _IN_COMMON_TCP_CONNECTION_H_
#include "Connection.h"
#include "Url.h"

namespace IN {
namespace COMMON {

class TcpConnection : public Connection
{
 public:
  static constexpr int DEFAULT_TIME_OUT = 100;
  
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();

  virtual int read(char* buf, int len) { }

  virtual int write(char* buf, int len) { }

  static TcpConnection* create(const Url& url, boost::asio::io_context& io_context);

private:
  TcpConnection(bool is_stream, boost::asio::io_context& io_context);
  PREVENT_COPY(TcpConnection);

  boost::asio::ip::tcp::socket socket_;
  unsigned short m_port;

  void handle_connect(const boost::system::error_code& ec);
  void handle_write(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
  bool open();
  void do_close();  
};

}}
#endif
