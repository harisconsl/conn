#ifndef _EXCHANGE_H_
#define _EXCHANGE_H_

#include <string>
#include <unordered_map>

namespace IN{
namespace ENGINE{

static const std::string EXCHANGE_STR_null("--");
static const std::string EXCHANGE_STR_CCIL("CCIL");
static const std::string EXCHANGE_STR_D2("D2");
static const std::string EXCHANGE_STR_NSE("NSE");
static const std::string EXCHANGE_STR_BSE("BSE");
static const std::string EXCHANGE_STR_EBS("EBS");
static const std::string EXCHANGE_STR_X360T("X360T");
static const std::string EXCHANGE_STR_X360T_F("X360T_F");
static const std::string EXCHANGE_STR_X360T_S("X360T_S");
static const std::string EXCHANGE_STR_CITI("CITI"); 
static const std::string EXCHANGE_STR_CITI_F("CITI_F"); 
static const std::string EXCHANGE_STR_CITI_S("CITI_S"); 

static const std::string EXCHANGE_STR_ANY_EXCH("ANY_EXCHANGE");

enum class EXCHANGE
{
  null,
  CCIL,
  D2,
  NSE,
  BSE,
  EBS,
  X360T,
  X360T_F,
  X360T_S,
  CITI,
  CITI_F,
  CITI_S,  
  ANY_EXCH
};

const EXCHANGE str2ex(const std::string& ex)
{
  static std::unordered_map<std::string, EXCHANGE> TO_EX = {
    {EXCHANGE_STR_CCIL, EXCHANGE::CCIL}
    , {EXCHANGE_STR_D2, EXCHANGE::D2}
    , {EXCHANGE_STR_NSE, EXCHANGE::NSE}
    , {EXCHANGE_STR_BSE, EXCHANGE::BSE}
    , {EXCHANGE_STR_EBS, EXCHANGE::EBS}
    , {EXCHANGE_STR_X360T, EXCHANGE::X360T}
    , {EXCHANGE_STR_X360T_F, EXCHANGE::X360T_F}
    , {EXCHANGE_STR_X360T_S, EXCHANGE::X360T_S}
    , {EXCHANGE_STR_CITI, EXCHANGE::CITI}
    , {EXCHANGE_STR_CITI_F, EXCHANGE::CITI_F}
    , {EXCHANGE_STR_CITI_S, EXCHANGE::CITI_S}
    , {EXCHANGE_STR_ANY_EXCH, EXCHANGE::ANY_EXCH}
  };

  auto find_it = TO_EX.find(ex);
  if (find_it != TO_EX.end())
    {
      return find_it->second;
    }
  
  return EXCHANGE::null;
}

const std::string& ex2str(EXCHANGE ex)
{
  switch(ex)
    {
    case EXCHANGE::CCIL:                    return EXCHANGE_STR_CCIL;
    case EXCHANGE::D2:                      return EXCHANGE_STR_D2;
    case EXCHANGE::NSE:                     return EXCHANGE_STR_NSE;
    case EXCHANGE::BSE:                     return EXCHANGE_STR_BSE;
    case EXCHANGE::EBS:                     return EXCHANGE_STR_EBS;
    case EXCHANGE::X360T:                   return EXCHANGE_STR_X360T;
    case EXCHANGE::X360T_F:                 return EXCHANGE_STR_X360T_F;
    case EXCHANGE::X360T_S:                 return EXCHANGE_STR_X360T_S;
    case EXCHANGE::CITI:                return EXCHANGE_STR_CITI;
    case EXCHANGE::CITI_F:              return EXCHANGE_STR_CITI_F;
    case EXCHANGE::CITI_S:              return EXCHANGE_STR_CITI_S;
    case EXCHANGE::ANY_EXCH:                return EXCHANGE_STR_ANY_EXCH;
    default:                                return EXCHANGE_STR_null;
    }
    return EXCHANGE_STR_null;
  }

}}
#endif
