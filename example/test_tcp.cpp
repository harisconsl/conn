#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include "../common/TcpConnection.h"
#include "../common/Logger.h"

using namespace std;
using namespace IN::COMMON;
int main()
{

  LOG_I("starting server");
  boost::asio::io_context io_context;
  std::string url = "tcp://0.0.0.0:41080";

  Connection::register_factory();
  TcpConnection* conn = static_cast<TcpConnection*>(Connection::get_connection(url, io_context));
  conn->connect();

  std::thread th([&io_context](){io_context.run();});



  std::string message = "hello";      
  while(true)
    {
      sleep(1);
      std::getline(std::cin, message);
      LOG_I("sending.. " <<  message);
      conn->async_write(message.c_str(), message.size());
    }

  std::cout << "main ends" << std::endl;
  th.join();
  return 0;
}
