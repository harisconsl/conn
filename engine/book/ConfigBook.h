#ifndef _CONFIGBOOK_H_
#define _CONFIGBOOK_H_

#include <map>
#include <vector>
#include "Exchange.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include "../../common/Logger.h"

using namespace std;
using boost::property_tree::ptree;
using boost::property_tree::xml_parser::trim_whitespace;

namespace IN {
namespace ENGINE {

struct StreamConfig
{
  struct LevelRule
  {
    int level;
    std::vector<EXCHANGE> level_exch_vec;
  };

  std::string stream_name;
  std::string symbol;
  std::vector<EXCHANGE> exch_vec;
  std::vector<int64_t> level_vec;
  std::map<int,LevelRule> rule_map;
};

class ConfigBook
{
public:
  ConfigBook()
  { } 

  bool load(const char* configPath)
  {
    std::cout << "started load1"<<std::endl;
    ptree prop;
    read_xml(configPath, prop, trim_whitespace);
    std::cout << "started load"<<std::endl;
    try
      {
        auto streams = prop.get_child("IN");      
        for (auto s = streams.begin(); s != streams.end(); ++s) 
          {
            if (s->first != "stream")
              continue;
            
            StreamConfig sc;
            
            sc.stream_name = s->second.get <string> ("<xmlattr>.name");
            sc.symbol = s->second.get <string> ("<xmlattr>.symbol");
            
            auto exchanges = s->second.get<string>("exchanges");
            // split the string
            int pos = 0;
            std::size_t found = exchanges.find(',', pos);
            while (found != std::string::npos)
              {
                try
                  {
                    std::string exch_name = exchanges.substr(pos, found - pos);
                    sc.exch_vec.push_back(str2ex(exch_name));
                  }
                catch (const boost::bad_lexical_cast &e)
                  {
                    throw std::invalid_argument("exchanges array is not correct");
                  }
                pos = found + 1;
                found = exchanges.find(',', pos);
              }
            // last one without comma
            std::string exch_name = exchanges.substr(pos);
            sc.exch_vec.push_back(str2ex(exch_name));

            auto hrules = s->second.get_child("horizontal");          
            for (auto& hr : hrules)
              {
                //read individual horizontal rules
                if(hr.first != "level_rule")
                  {
                    continue;
                  }

                StreamConfig::LevelRule lr;
                lr.level = boost::lexical_cast<int>(hr.second.get<std::string>("<xmlattr>.level"));
                std::string in_exchanges = hr.second.get<std::string>("<xmlattr>.exchanges");
                int pos = 0;
                std::size_t found = in_exchanges.find(',', pos);
                while (found != std::string::npos)
                  {
                    std::string exch_name = in_exchanges.substr(pos, found - pos);
                    lr.level_exch_vec.push_back(str2ex(exch_name));

                    pos = found + 1;
                    found = in_exchanges.find(',', pos);
                  }
                std::string exch_name = in_exchanges.substr(pos);
                lr.level_exch_vec.push_back(str2ex(exch_name));
                sc.rule_map.insert(std::pair<int, StreamConfig::LevelRule>(lr.level, lr));
              }

            auto levels = s->second.get <string>("vertical.levels");
            // split the string
            pos = 0;
            found = levels.find(',', pos);
            while (found != std::string::npos)
              {
                try
                  {
                    int i = boost::lexical_cast<int>(levels.substr(pos, found - pos));
                    sc.level_vec.push_back(i);
                  }
                catch (const boost::bad_lexical_cast &e)
                  {
                    throw std::invalid_argument("vertical levels  array not correct ");
                  }
            
                pos = found + 1;
                found = levels.find(',', pos);
              }
            int i = boost::lexical_cast<int>(levels.substr(pos));
            sc.level_vec.push_back(i);

            str_cfg.insert(std::pair<std::string,StreamConfig>(sc.stream_name,sc));
          }
        
      } catch (std::exception& e)
      {
        LOG_E("Exception while parsing xml. Error: "<< e.what());
        return false;
      }
    return true;
  }

  void print() const
  {
    for (auto& it: str_cfg)
      {
        LOG_I( "name :" << it.second.stream_name);
        LOG_I( "symbol :" << it.second.symbol);
        for (auto& it_vec : it.second.exch_vec)
          {
            LOG_I("exchange :" << ex2str(it_vec));
          }
        for (auto& it_vec : it.second.level_vec)
          {
            LOG_I("size :" << it_vec);        
          }
        for (auto& it_map : it.second.rule_map)
          {
            LOG_I("level :" << it_map.second.level);
            for (auto& it_vec : it_map.second.level_exch_vec)
              {
                LOG_I("level exchange :" << ex2str(it_vec));        
              }
          }
      }
  }

  std::map<std::string, StreamConfig> str_cfg;


};

}}
#endif
