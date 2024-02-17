#include<iostream>
#include "../engine/book/ConfigBook.h"
#include "../common/Logger.h"


int main(int argc, char* argv[])
{
  if (argc < 2)
    {
      LOG_I("Usage: " << argv[0] << " <book_config.xml>");
      return 1;
    }

  std::cout << "config test case" << '\n';
  IN::ENGINE::ConfigBook cfg;
  cfg.load(argv[1]);
  cfg.print();
  return 0;
}

