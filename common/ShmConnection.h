#ifndef _IN_COMMON_SHM_CONNECTION_H_
#define _IN_COMMON_SHM_CONNECTION_H_

#include "Connection.h"

namespace IN {
namespace COMMON {

class ShmConnection
{
 public:
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();
  
 private:
  ShmConnection(bool is_stream);
  PREVENT_COPY(ShmConnection);
};

}}
#endif
