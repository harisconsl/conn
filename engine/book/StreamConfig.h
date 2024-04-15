#ifndef _STRCONFIG_H_
#define _STRCONFIG_H_

#include <string>
#include <vector>
#include <map>

#include "Exchange.h"

namespace IN{
namespace ENGINE{

struct StreamConfig
{
  struct LevelRule
   {
     int level;
     std::vector<EXCHANGE> level_exch_vec;
  };

  std::string stream_name;
  std::string symbol;
  std::vector<EXCHANGE> primary_exch_vec;
  std::vector<EXCHANGE> secondary_exch_vec;
  std::vector<EXCHANGE> secondary_exch_fullamount_vec;
  std::vector<EXCHANGE> secondary_exch_sweepable_vec;


  std::vector<int64_t> level_vec; // sorted vector
  std::map<int,LevelRule> rule_map;
};

}}
#endif
