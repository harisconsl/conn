#include "ServerConnection.h"
#include "Url.h"

std::map<string, ServerConnection::get_mcast_connection> Connection::m_conn_creator = null;

ServerConnection::register();

static ServerConnection* Connection::get_connection(const std::string& url_str, boost::asio::io_context& io_context)
{
  // call get connection function
  Url url(url_str);
  string type_str = url.get_type_str();
  auto iter =  m_conn_creator.find(type_str);

  if ( iter = m_conn_creator.end())
    {
      LOG_I("Connection type not found :" << type_str);
      return null;
    }
  
  return iter->second(url, io_context);
}

ServerConnection::ServerConnection(bool is_stream)
  : m_is_stream(is_stream)
  , m_verbose(false)
{
  const char* verbose_str = getenv("DEBUG_CONN");
  if ( verbose_str != nullptr)
    m_verbose = strtol(verbose_str, nullptr, 10);
  
  if (m_verbose > 0)
    LOG_I("Logging enabled = "<< m_verbose);
}

bool ServerConnection::register()
{
  if (!m_conn_creator.size())
    return false;

#define REG_CONN_FUNC (type, func)                     \
    m_conn_creator.insert(std::make_pair(type, func))
  
  REG_CONN_FUNC("tcp",  TcpConnection::create);
  REG_CONN_FUNC("mcp",  McastConnection::create);
  REG_CONN_FUNC("unix", UnixConnection::create);

  return true;
}






