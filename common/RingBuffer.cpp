#include "RingBuffer.h"
#include "ScopedFd.h"
#include "Utils.h"
#include "Logger.h"
using namespace IN::COMMON;

RingBuffer::RingBuffer()
  : m_buf(nullptr)
  , m_read_idx()
  , m_write_idx()
  , m_notifier()
{ }

RingBuffer::~RingBuffer()
{
  if (m_buf)
    {
      munmap(m_buf, m_capacity << 1);
      m_buf = nullptr;
    }
}

bool RingBuffer::alloc( uint32_t capacity)
{
  // https://github.com/willemt/cbuffer/blob/master/cbuffer.c
  if (m_buf)
    return false;

  if (!m_notifier.init_fd())
    return false;

  m_capacity = SC_PAGE_UP(capacity);
  
  char path[] = "/tmp/rb-XXXXXX";
  int status;
  void *address;
  void* buf;

  ScopedFd fd = mkstemp(path);
  if (fd < 0)
    return false;

  status = unlink(path);
  if (status)
    return false;

  status = ftruncate(fd, m_capacity);
  if (status)
    return false;

  /* create the array of data */
  buf = mmap(NULL, m_capacity << 1, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE,
	     -1, 0);
  if (buf == MAP_FAILED)
    return false;

  address = mmap(buf, m_capacity, PROT_READ | PROT_WRITE,
		 MAP_FIXED | MAP_SHARED, fd, 0);
  
  if (address != buf)
    return false;

  address = mmap( buf + m_capacity, m_capacity,  PROT_READ | PROT_WRITE,
		  MAP_FIXED | MAP_SHARED, fd, 0);
  if (address != (void*)((uint8_t*)buf + m_capacity))
    return false;

  m_buf = reinterpret_cast<uint8_t*>(buf);
  LOG_I("address: " << static_cast<void*>(m_buf));
  return true;
}

void RingBuffer::free()
{
  if (m_buf)
    {
      munmap(m_buf, m_capacity << 1);
      m_buf = nullptr;
    }
  m_notifier.close_fd();
}

int RingBuffer::fd()
{
  return m_notifier.get_fd();
}

bool RingBuffer::notify_fd()
{
 return  m_notifier.notify();
}

bool RingBuffer::clear_fd()
{
  return m_notifier.clear_fd();
}

std::pair<uint8_t* , size_t> RingBuffer::read_buffer()
{
  uint32_t read_idx = m_read_idx.load(std::memory_order_acquire);
  uint32_t write_idx = m_write_idx.load(std::memory_order_acquire);
  return std::make_pair(m_buf + read_idx, write_idx - read_idx);
}

std::pair<uint8_t* , size_t> RingBuffer::write_buffer()
{
  uint32_t read_idx = m_read_idx.load(std::memory_order_acquire);
  uint32_t write_idx = m_write_idx.load(std::memory_order_acquire);

  while(write_idx - read_idx > m_capacity)
    {
      read_idx = m_read_idx.load(std::memory_order_acquire);
      write_idx = m_write_idx.load(std::memory_order_acquire);
    }
  return std::make_pair(m_buf + write_idx, m_capacity - (write_idx - read_idx));
}

void RingBuffer::advance_write( uint32_t increment)
{
  m_write_idx.fetch_add( increment, std::memory_order_release);
}

void RingBuffer::advance_read( uint32_t increment)
{
  uint32_t read_idx = m_read_idx.load(std::memory_order_acquire);
  read_idx += increment;

  if (read_idx > m_capacity)
    {
      m_read_idx.fetch_sub(m_capacity - increment, std::memory_order_release);
      m_write_idx.fetch_sub(m_capacity, std::memory_order_release);
    }
  else
    m_read_idx.fetch_add(increment);
}

