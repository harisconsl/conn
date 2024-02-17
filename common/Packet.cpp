#include "Packet.h"
using namespace std;
using namespace IN::COMMON;

static const uint32_t MAX_PACKET_SIZE = 1024*1024;

NPacket::NPacket()
  : m_size(0)
  , m_read_idx(0)
  , m_buf(new uint8_t[MAX_PACKET_SIZE])
{ }


NPacket::~NPacket()
{
  if (m_buf)
    {
      m_read_idx = 0;
      delete [] (uint8_t*)m_buf;
      m_buf = nullptr;
    }
}

void NPacket::reset_read()
{
  m_size = 0;
  m_read_idx = 0;
}

uint32_t NPacket::write_size()
{
  if (m_buf)
    return MAX_PACKET_SIZE - m_size;
  return 0;
}

uint32_t NPacket::read_size()
{
  if (m_buf)
    return m_size - m_read_idx;
  return 0;
}


template <typename T> 
bool NPacket::put(const T& v)
{
  if (write_size() < sizeof(T))
    return false;
  memcpy(&m_buf[m_size], &v, sizeof(v));
  m_size += sizeof(T);
  return true;
}

template <typename T> 
bool NPacket::get(T& v)
{
  if (read_size() < sizeof(T))
    return false;
  memcpy(&v, &m_buf[m_read_idx], sizeof(v));
  m_read_idx += sizeof(T);
  
  return true;
}

bool NPacket::put(const char* s)
{
  uint16_t len = strlen(s);
  if (write_size() < (sizeof(len) + len))
    return false;

  put<uint16_t>(len);
  memcpy(&m_buf[m_size], s, len);

  m_size += len;
  return true;
}

bool NPacket::put(const std::string& s)
{
  return put(s.c_str());
}

bool NPacket::put(const uint8_t ba[], size_t len)
{
  if (write_size() < (sizeof(len) + len))
    return false;

  put<uint16_t>(len);
  memcpy(&m_buf[m_size], ba, len);

  m_size += len;
  return true;
}

bool NPacket::get(std::string& s)
{
  uint16_t slen = 0;
  if (!get<uint16_t>(slen))
    return false;

  if (read_size() < slen) {
    m_read_idx -= sizeof(slen);
    return false;
  }

  s.clear();
  s.append((char*)&m_buf[m_read_idx], slen);
  m_read_idx += slen;
  return true;
}

int NPacket::get(uint8_t buf[], size_t len)
{
  uint16_t arr_len = 0;
  if (!get<uint16_t>(arr_len))
    return -1;

  if (read_size() < arr_len) {
    m_read_idx -= sizeof(arr_len);
    return -1;
  }

  if (arr_len > len) {
    m_read_idx -= sizeof(arr_len);
    return arr_len;
  }

  memcpy(buf, &m_buf[m_read_idx], arr_len);
  m_read_idx += arr_len;
  return arr_len;
}

template <typename T> 
bool put(NPacket& p, const T& v)
{
  p.put(v);
}

template <typename T> 
bool get(NPacket& p, T& v)
{
  return p.get(v);
}

bool put(NPacket& p, const char* s)
{
  p.put(s);
}

bool put(NPacket& p, const std::string& s)
{
  p.put(s.c_str());
}

bool get(NPacket& p, std::string& s)
{
  p.get(s);
}

