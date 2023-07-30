#ifndef _IN_COMMON_RING_BUFFER_H_
#define _IN_COMMON_RING_BUFFER_H_

#include <atomic>
#include <utility>
#include <sys/mman.h>
#include "NotifierFd.h"

namespace IN{
namespace COMMON{

class RingBuffer
{
 public:
  RingBuffer();
  ~RingBuffer();

  bool alloc(uint32_t capacity);

  void free();
  int fd();
  
  std::pair<uint8_t*, size_t> write_buffer();
  std::pair<uint8_t*, size_t> read_buffer();

  
  void advance_read(uint32_t increment);
  void advance_write(uint32_t increment);

  bool clear_fd();
  bool notify_fd();

  uint32_t read_index(){ return m_read_idx.load(std::memory_order_acquire);}
  uint32_t write_index(){ return m_write_idx.load(std::memory_order_acquire);}
  
 private:
  uint8_t* m_buf;
  uint32_t m_capacity;
  std::atomic<uint32_t> m_read_idx;
  std::atomic<uint32_t> m_write_idx;
  NotifierFd m_notifier;
};
 
}}
#endif
