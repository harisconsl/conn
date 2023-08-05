#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include "../common/UnixConnection.h"
#include "../common/Logger.h"

using namespace std;
using namespace IN::COMMON;
int main()
{

  LOG_I("starting server");
  boost::asio::io_context io_context;
  std::string url = "unix://tmp/testsocket";

  Connection::register_factory();
  UnixConnection* conn = static_cast<UnixConnection*>(Connection::get_connection(url, io_context));
  conn->connect();

  std::thread th([&io_context](){io_context.run();});

  std::string message = "hello";
      
  while(true)
    {
      LOG_I("Inside while loop");
      sleep(1);
      std::getline(std::cin, message);
      LOG_I("sending.. " <<  message);
      conn->async_write(message.c_str(), message.size());
    }

  std::cout << "main ends" << std::endl;
  th.join();
  return 0;
}
