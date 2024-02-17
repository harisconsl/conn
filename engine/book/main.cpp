#include <iostream>
#include <vector>
#include <map>
#include "Stream.h"
#include "ConfigBook.h"
#include "ExchSymKey.h"

#include "../../common/Logger.h"

using namespace std;
using namespace IN::ENGINE;
int main(int argc, char* argv[])
{
  if (argc < 2)
    {
      LOG_E("Usage: " << argv[0] << " <config.xml>");
      return 1;
    }
  // read config
  ConfigBook cfg;
  if (cfg.load(argv[1]))
    {
      LOG_E("Unable to load config: " << argv[1] << ".");
      return 1;
    }
  // create stream objects
  // stream name, stream map
  std::map<std::string, Stream*> map_stream_ptr;
  // MktShmKey stream vector map
  std::map<ExchSymKey, std::vector<Stream*>> maps_exchange_stream_ptr;

  for (auto& it: cfg.str_cfg)
    {
      Stream* str = new Stream(it.second.stream_name, it.second.symbol);
      map_stream_ptr.emplace(std::make_pair(it.second.stream_name, str));
      
      for (auto ex : it.second.exch_vec)
        {
          ExchSymKey key(str->get_symbol(),ex);
          auto found = maps_exchange_stream_ptr.find(key);
          if (found != maps_exchange_stream_ptr.end())
            {
              found->second.push_back(str);
            }
          else
            {
              std::vector<Stream*> v;
              v.push_back(str);
              ExchSymKey key(str->get_symbol(),ex);
              maps_exchange_stream_ptr.emplace(std::make_pair(key,v));
            }
        }
    }
  // read the level2 updates from SHM/file

  // on the basis of exchnage name and symbol apply
  // Stream will do final aggregation after each update
  while(true)
    {
      L2SnapshotLite* msg = new L2SnapshotLite;
      ExchSymKey key(msg->symbol,msg->exchange);
      auto found = maps_exchange_stream_ptr.find(key);
      if ( found != maps_exchange_stream_ptr.end())
        {
          // apply the L2SnapshotLite to all the members in the vector
          for( auto& it : found->second)
            {
              it->onSnapshot(msg);
            }          
        }
    }
  
  return 0;
}


