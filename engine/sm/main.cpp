#include <vector>
#include <map>


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
  ConfigSM cfg;
  if (cfg.load(argv[1]))
    {
      LOG_E("Unable to load config: " << argv[1] << ".");
      return 1;
    }
  // create stream objects
  return 0;
}
