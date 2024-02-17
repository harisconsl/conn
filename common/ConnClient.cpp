#include "ConnClient.h"
#include "Connection.h"
#include "RecursiveLock.h"
#include "Logger.h"
#include <memory>

using namespace std;
using namespace IN::COMMON;

ConnClient::ConnClient() 
  : m_url()
  , m_connection(nullptr)
  , m_lock()
  , m_packet()
{ }

ConnClient::~ConnClient()
{
  disconnect();
}

bool ConnClient::connect(const std::string& url, boost::asio::io_context& io_context)
{
  RecursiveLock l(m_lock);
  auto* connection = m_connection;
  if (connection)
    LOG_I("Client is already connected to '" << connection->address());

  m_connection = Connection::get_connection(url, io_context);

  if (!m_connection)
    LOG_I("Unable to create connection "<< url );

  if (!m_connection->add_listener(this) || !m_connection->is_open()) 
    {
      LOG_E("Unable to open connection '" << url);
      delete m_connection;
      m_connection = nullptr;
      return false;
    }

  m_url = url;
  return true;
}

void ConnClient::disconnect()
{
  RecursiveLock l(m_lock);
  if (m_connection) 
    {
      m_connection->close();
      m_connection = nullptr;
    }
}

Connection* ConnClient::get_connection()
{
  return m_connection;
}

bool ConnClient::is_connected()
{
  RecursiveLock l(m_lock);
  return m_connection != nullptr;
}

std::string ConnClient::url() const
{
  return m_url;
}
