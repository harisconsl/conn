#ifndef _IN_COMMON_MCAST_CONNECTION_H_
#define _IN_COMMON_MCAST_CONNECTION_H_

#include "Connection.h"

namespace IN {
namespace COMMON {

class McastConnection
{
 public:
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();
  
 private:
  McastConnection(bool is_stream);
  PREVENT_COPY(McastConnection);
};

}}
#endif
