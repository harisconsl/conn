#ifndef _IN_COMMON_TCP_CONNECTION_H_
#define _IN_COMMON_TCP_CONNECTION_H_
#include "Connection.h"

namespace IN {
namespace COMMON {

typedef boost::asio::ip bai;
class TcpConnection
{
 public:
  static constexpr int DEFAULT_TIME_OUT;
  
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();

  TcpConnection* create(const Url& url, const boost::asio::io_context& io_context);

private:
  TcpConnection(bool is_stream, boost::asio::io_context& io_context);
  PREVENT_COPY(TcpConnection);

  bai::tcp::socket _socket;

  void handle_connect(const boost::system::error_code& ec);
  void handle_write(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void do_close();  
};

}}
#endif
