#include "ChronicleConnection.h"
#include "Logger.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace IN::COMMON;

ChronicleConnection::ChronicleConnection(bool is_stream, boost::asio::io_context& io_context)
  : Connection(is_stream)
{ }

ChronicleConnection* ChronicleConnection::create(const Url& url, boost::asio::io_context& io_context)
{ 
  std::string host = url.get_address();
  if (!host.size())
    {
      LOG_I("Invalid host : " << host);
      return nullptr;
    }

  ChronicleConnection* connection = new ChronicleConnection(true, io_context);
  // intialize the private memeberiof the class
  connection->m_address = host;
  return connection;
}

int ChronicleConnection::open()
{
  /*
  auto queue = SingleChronicleQueueBuilder::binary(PathName("test")).rollCycle(RollCycles::DAILY()).build();
  auto appender = queue->acquireAppender();
  // appender->writeText("hello world");
  auto tailer = queue->aquireTailer();
  // tailer->readText();
  */
  return 0;
}

int get_fd()
{
  return -1;
}
