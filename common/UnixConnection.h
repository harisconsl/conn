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
  
 private:
  UnixConnection(bool is_stream);
  PREVENT_COPY(UnixConnection);
};

}}
#endif
