#ifndef _IN_COMMON_UNIX_CONNECTION_H_
#define _IN_COMMON_UNIX_CONNECTION_H_

#include "Connection.h"

namespace IN {
namespace COMMON {

class UnixConnection
{
 public:
  static constexpr int DEFAULT_TIME_OUT;
  
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();
  UnixConnection* create(const Url& url, boost::asio::io_context& io_context);
  
 private:
  UnixConnection(bool is_stream, boost::asio::io_context& io_context);
  PREVENT_COPY(UnixConnection);

  boost::asio::local::stream_protocol::socket socket_;
};

}}
#endif
