#ifndef _IN_COMMON_PACKET_H_
#define _IN_COMMON_PACKET_H_

#include "Connection.h"

namespace IN {
namespace COMMON {

class NPacket
{
public:
  static const uint32_t MAX_PACKET_SIZE;
  NPacket();
  ~NPacket();

  //  std::pair<const uint8_t*, int size> get_buffer();
  void reset_read();

  template <typename T>
  bool put(const T& v);
  template <typename T>
  bool get(T& v);

  bool put(const char* s);
  bool put(const std::string& s);
  bool put(const uint8_t buf[], size_t len);

  bool get(std::string& s);
  int  get(uint8_t buf[], size_t len);

private:
  int m_read_idx;
  int m_size;
  uint8_t* m_buf;
  friend class Connection;

  PREVENT_COPY(NPacket);

  uint32_t read_size();
  uint32_t write_size();
 
};

template <typename T>
bool put(NPacket& p, const T& v);

template <typename T>
bool get(NPacket& p, T& v);

bool put(NPacket& p, const char* s);
bool put(NPacket& p, const std::string& s);
bool get(NPacket& p, std::string& s);

}}
#endif
