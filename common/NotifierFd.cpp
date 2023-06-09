#include "NotifierFd.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "Logger.h"

NotifierFd::NotifierFd()
  : m_event_fd(-1)
{ }

NotifierFd::~NotifierFd()
{
  close_fd();
}

bool NotifierFd::init_fd()
{
  if (m_event_fd < 0)
    {
      m_event_fd = eventfd(0, O_NONBLOCK);
      if ( eventfd < 0)
	{
 	  LOG_E("Failed to create eventfd err no :" << strerror(errno) << ": " << errno);
	  return false;
	}
    }
  return true;
}

bool close_fd()
{
  if (m_event_fd > 0)
    ::close (m_event_fd);
  m_event_fd = -1;
}

int NotifierFd::get_fd() const
{
  return m_event_fd;
}

bool NotifierFd::notify()
{
  if (m_event_fd < 0)
    return false;

  eventfd_t ev = 1;
  int res = eventfd_write(m_event_fd, ev);
  if (res < 0)
    {
      LOG_E("Failed to write eventfd " << res << " err no :" << strerror(errno) << ": " << errno);
      return false;
    }

  return res == 0;
}

bool Notifier::clear_fd()
{
  if (m_event_fd < 0)
    return false;

  eventfd_t ev = 0;
  int res = eventfd_read(m_event_fd. &ev);
  if ( res < 0 && errno != EAGAIN)
    LOG_E("Failed to read eventfd " << res << " err no :" << strerror(errno) << ": " << errno);

  return (res == 0 || (res < 0 && errno == EAGAIN));
}

