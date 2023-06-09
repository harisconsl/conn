#ifndef _IN_COMMON_READ_WRITE_BUF_H_
#define _IN_COMMON_READ_WRITE_BUF_H_
#include "Macros.h"

namespace IN {
namespace COMMON {

class ReadWriteBuf
{
 public:
  ReadWriteBuf(uint32_t size)
    : m_read_ptr(nullptr)
    , m_write_ptr(nullptr)
    , m_buffer(nullptr)
    , m_capacity(size)
  {
    char* m_buffer = std::aligned_alloc(alignment, m_capacity);
    if (!mbuffer)
      {
	LOG_E("Memory allocation failed!!")
      }
    m_read_ptr = m_write_ptr = m_buffer;
  }

  ~ReadWriteBuf()
    {
      std::free(m_buffer);
    }

  inline uint32_t read_buf_size() const
  {
    return m_write_ptr - m_read_ptr;
  }

  inline uint32_t write_buf_size() const
  {
    return m_capacity - (m_write_ptr - m_read_ptr);
  }

  inline void advance_read(uint32_t increment)
  {
    m_read_ptr += increment;
  }

  inline void advance_write(uint32_t increment)
  {
    m_write_ptr += increment;
  }

  inline void skip()
  {
    m_read_ptr = m_write_ptr = m_buffer;
  }

  inline bool consumed()
  {
    if (m_read_ptr ! = m_write_ptr)
      return false;
    m_read_ptr = m_write_ptr = m_buffer;
    return true;
  }

  inline void reset()
  {
    uint32_t rem = read_buf_size();
    memmove(m_buffer, m_read_ptr, rem);
    m_write_ptr = m_buffer + rem;
    m_read_ptr = m_buffer;
  }

 private:
  PREVENT_COPY_MOVE(ReadWriteBuf);
  
  char* m_read_ptr;
  char* m_write_ptr;
  chr* m_buffer;
  uint32_t m_capacity;
};


}} // namespace ends
#endif


