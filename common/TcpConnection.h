#ifndef _IN_COMMON_TCP_CONNECTION_H_
#define _IN_COMMON_TCP_CONNECTION_H_

#include "TcpConnection.h"

namespace IN {
namespace COMMON {

class TcpConnection
{
 public:
  static constexpr int DEFAULT_TIME_OUT;
  
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();

  Connection* create(const Url& url);
  
 private:
  TcpConnection(bool is_stream);
  PREVENT_COPY(TcpConnection);
};

}}
#endif
