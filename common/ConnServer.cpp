#include "ConnServer.h"
#include "Connection.h"
#include "RecursiveLock.h"

using namespace std;
using namespace IN::COMMON;

ConnServer::ConnServer(ConnServer::Close_cb cb, Connection* conn)
  : m_close_cb(cb)
  , m_conn(conn)
  , m_lock()
  , m_packet()
  , m_time_of_disconnect(TIME_TO_WAIT)
{
  m_conn->add_listener(this);
}

ConnServer::~ConnServer() 
{
  disconnect();
  m_close_cb = nullptr;
}

void ConnServer::disconnect()
{
  RecursiveLock lock(m_lock);
  if (m_conn)
    {
      m_conn->close();
      m_conn = nullptr;
    }
}

bool ConnServer::is_connected()
{
  RecursiveLock lock(m_lock);
  return m_conn != nullptr;
}

void ConnServer::on_disconnect()
{
  m_conn = nullptr;
  on_client_disconnect();
  m_close_cb(this);
}

