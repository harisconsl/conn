#ifndef _EXCHSYMKEY_H_
#define _EXCHSYMKEY_H_

#include "Exchange.h"

namespace IN{
namespace ENGINE{

class ExchSymKey
{
private:
  std::string m_symbol;
  EXCHANGE m_exchange;
public:
  ExchSymKey(std::string symbol,EXCHANGE ex)
  {
    m_symbol = symbol;
    m_exchange = ex;
  }

  ExchSymKey() = default;
  std::string get_symbol() const
  {
    return m_symbol;
  }

  EXCHANGE get_exchange() const
  {
    return m_exchange;
  }

  bool operator ==(const ExchSymKey& rhs) const
  {
    return (m_symbol == rhs.m_symbol && m_exchange == rhs.m_exchange);
  }

  operator size_t() const
  {
    return (size_t) m_symbol.size() ^ ((size_t) m_exchange << 1);
  }

  bool operator < (const ExchSymKey& rhs) const
  {
    if (m_exchange < rhs.m_exchange)
      return true;
    else if (m_exchange > rhs.m_exchange)
      return false;
    else
      return m_symbol < rhs.m_symbol;

  }

  std::string to_string() const
  {
    std::stringstream ss;
    ss << ex2str(m_exchange) << "::" << m_symbol ;
    return ss.str();
  }

};
}}
#endif
