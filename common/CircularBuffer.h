#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_
#include <stdlib.h>
#include <optional>
#include "Utils.h"
#include "Macros.h"
#include "Logger.h"

namespace IN {
namespace COMMON {

template <typename T >
struct SeqPacket
{
  SeqPacket(): m_ts(0), m_ptr(nullptr) { }

  SeqPacket(uint64_t ts, T* ptr) : m_ts(ts), m_ptr(ptr) { }
  // Parameterized constructor
  SeqPacket(uint64_t ts, std::unique_ptr<T>& ptr) : m_ts(ts), m_ptr(std::move(ptr)) { }

  // Copy constructor
  // SeqPacket(const SeqPacket& other) : m_ts(other.m_ts), m_ptr(other.m_ptr ? std::make_unique<T>(*other.m_ptr) : nullptr) { }

  // Move constructor
  SeqPacket(SeqPacket&& other) noexcept : m_ts(std::move(other.m_ts)), m_ptr(std::move(other.m_ptr)) { }

  // Assignment operator
  // SeqPacket& operator=(const SeqPacket& other) {
  //   if (this != &other) {
  //     // Perform member-wise assignment
  //     m_ts = other.m_ts;
  //     m_ptr = other.m_ptr;
  //   }
  //   return *this;
  // }

  // Move assignment operator
  SeqPacket& operator=(SeqPacket&& other) noexcept
  {
    if (this != &other) {
      // Perform member-wise move assignment
      m_ts = std::move(other.m_ts);
      m_ptr = std::move(other.m_ptr);
    }
    return *this;
  }

  uint64_t m_ts;
  std::unique_ptr<T> m_ptr;
};

template <typename T, std::size_t N>
class CircularBuffer
{
private:
  PREVENT_COPY(CircularBuffer);
  int32_t m_max_index;
  int32_t m_write_index;

  std::array<T, N> m_array;
public:
  CircularBuffer()
    : m_write_index(0)
  { }

  void update(T& sq)
  {
    m_array[m_write_index++] = std::move(sq);
    
    if (m_write_index == N)
      m_write_index = 0;
  }
  
  std::optional<T> find( uint64_t timestamp)
  {
    
    for (int i = 0 ;i < m_write_index ;++i)
      {
        if (timestamp == m_array[i].m_ts)
          return std::move(m_array[i]);
      }
    /*
    // search from  0 to write and read to end
    for (int i = m_write_index; i < N; ++i)
      {
        if (timestamp == m_array[i].m_ts)
          return std::make_optional<T>(m_array[i]);

        //return m_array[i];
      }
    */
    return std::nullopt;
  }
  
  uint64_t write_index() const
  {
    return m_write_index;
  }

};

}}
#endif 
