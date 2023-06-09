#ifndef _IN_COMMON_SCOPED_FD_H_
#define _IN_COMMON_SCOPED_FD_H_
#include <unistd.h>

class ScopedFd
{
 public:
 ScopedFd(int fd)
   : m_fd(fd)
    { }

 ~ScopedFd
  {
    if (m_fd > 0)
      close(m_fd);
    m_fd = -1;
  }

 operator int()
 {
   return m_fd;
 }

 ScopedFd& operator = (int fd)
   {
     m_fd = fd;
     return *this;
   }
 
 private:
  int m_fd;
}
#endif
