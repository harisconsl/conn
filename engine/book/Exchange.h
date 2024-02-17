#ifndef _EXCHANGE_H_
#define _EXCHANGE_H_

#include <string>
#include <unordered_map>

namespace IN{
namespace ENGINE{

static const std::string EXCHANGE_STR_null("--");
static const std::string EXCHANGE_STR_CCIL("CCIL");
static const std::string EXCHANGE_STR_D2("D2");
static const std::string EXCHANGE_STR_X360T("X360T");
static const std::string EXCHANGE_STR_ANY_EXCH("ANY_EXCHANGE");

enum class EXCHANGE
{
  null,
  CCIL,
  D2,
  X360T,
  ANY_EXCH
};

const EXCHANGE str2ex(const std::string& ex)
{
  static std::unordered_map<std::string, EXCHANGE> TO_EX = {
    {EXCHANGE_STR_CCIL, EXCHANGE::CCIL}
    , {EXCHANGE_STR_D2, EXCHANGE::D2}
    , {EXCHANGE_STR_X360T, EXCHANGE::X360T}
    , {EXCHANGE_STR_ANY_EXCH, EXCHANGE::ANY_EXCH}
  };

  auto find_it = TO_EX.find(ex);
  if(find_it != TO_EX.end())
    {
      return find_it->second;
    }

  return EXCHANGE::null;
}

const std::string& ex2str(EXCHANGE ex)
{
  switch(ex)
    {
    case EXCHANGE::CCIL:      return EXCHANGE_STR_CCIL;
    case EXCHANGE::D2:        return EXCHANGE_STR_D2;
    case EXCHANGE::X360T:        return EXCHANGE_STR_X360T;
    case EXCHANGE::ANY_EXCH:  return EXCHANGE_STR_ANY_EXCH;
    default:                  return EXCHANGE_STR_null;
    }
    return EXCHANGE_STR_null;
  }

}}
#endif
