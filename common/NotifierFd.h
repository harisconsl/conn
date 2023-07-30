#ifndef _IN_COMMON_NOTFIER_FD_H_
#define _IN_COMMON_NOTIFIER_FD_H_
#include <sys/eventfd.h>
#include <stddef.h>
#include <stdint.h>

namespace IN{
namespace COMMON{

class NotifierFd
{
  public:
  NotifierFd();
  ~NotifierFd();

  int get_fd() const;
  bool notify();
  bool clear_fd();

  bool init_fd();
  void close_fd();
 private:
  int m_event_fd;
};

}}
#endif
