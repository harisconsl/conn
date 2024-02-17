#ifndef _IN_RECURSIVELOCK_H_
#define _IN_RECURSIVELOCK_H_
#include <thread>
#include <mutex>

namespace IN {
namespace COMMON {

class RecursiveLock
{
public:
  RecursiveLock(std::recursive_mutex& lock)
    : m_lock(lock)
  {
    m_lock.lock();
  }

  ~RecursiveLock()
  {
    m_lock.unlock();
  }
private:
  std::recursive_mutex& m_lock;
};

}}
#endif
