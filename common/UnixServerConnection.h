#ifndef _IN_COMMON_UNIX_SERVER_CONNECTION_H_
#define _IN_COMMON_UNIX_SERVER_CONNECTION_H_

#include "Connection.h"

namespace IN {
namespace COMMON {

class UnixServerConnection
{
 public:
  static constexpr int DEFAULT_TIME_OUT;
  
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();
  UnixServerConnection* create(const Url& url, boost::asio::io_context& io_context);
  
 private:
  UnixConnection(bool is_stream, boost::asio::io_context& io_context);
  PREVENT_COPY(UnixServerConnection);

  boost::asio::local::stream_protocol::socket socket_;
};

}}
#endif
