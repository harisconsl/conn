#ifndef _IN_COMMON_UNIX_CONNECTION_H_
#define _IN_COMMON_UNIX_CONNECTION_H_
#include "Connection.h"
#include "Url.h"

namespace IN {
namespace COMMON {

class UnixConnection : public Connection 
{
 public:
  static constexpr int DEFAULT_TIME_OUT = 100;
  
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();
  
  static UnixConnection* create(const Url& url, boost::asio::io_context& io_context);

  virtual int read(char* buf, int len) { }

  virtual int write(char* buf, int len) { }
  
 private:
  UnixConnection(bool is_stream, boost::asio::io_context& io_context);

  PREVENT_COPY(UnixConnection);

  boost::asio::local::stream_protocol::socket socket_;

  void handle_connect(const boost::system::error_code& ec);
  void handle_write(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void do_close();
};

}}
#endif
