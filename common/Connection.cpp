

std::map<string, Connection::get_mcast_connection>* Connection::m_conn_creator = null;

Connection::register();

static Connection* Connection::get_connection()
{
  // call transport function
  
}



Connection::Connection(bool isstream)
{ }

bool Connection::register()
{
  if (m_conn_creator)
    return false;

#define REG_CONN_FUNC (type, func)                     \
    m_conn_creator->insert(std::make_pair(type, func))
  
  REG_CONN_FUNC("tcp", TcpConnection:get_tcp_connection);
  REG_CONN_FUNC("mcp", TcpConnection:get_mcast_connection);
  REG_CONN_FUNC("unix", TcpConnection:get_unix_connection);
  REG_CONN_FUNC("shm", TcpConnection:get_shm_connection);

  return true;
}

Connection* Connection::get_mcast_connection()
{ }

Connection* Connection::get_tcp_connection()
{ }

Connection* Connection::get_unix_connection()
{ }

Connection* Connection::get_shm_connection()
{ }






