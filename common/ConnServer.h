#ifndef _CONN_SERVER_H_
#define _CONN_SERVER_H_

#include <functional>
#include <mutex>
#include <atomic>

#include "Timepoint.h"
#include "ConnListener.h"
#include "Packet.h"

namespace IN {
namespace COMMON {
    
class Connection;
class ConnServer : public ConnListener
{
public:
  static constexpr int64_t TIME_TO_WAIT = 5000000000LL; 

  typedef std::function<void(ConnServer*)> Close_cb;

  ConnServer(Close_cb cb, Connection* conn);  
  virtual ~ConnServer();
  
  void disconnect();
  bool is_connected();
  
  virtual void on_disconnect() final;

  virtual void on_connect() = 0;
  virtual void on_packet(NPacket* p) = 0;
  virtual void on_client_disconnect() = 0;

protected:
  Close_cb m_close_cb;
  Connection* m_conn;
  std::recursive_mutex m_lock;
  NPacket m_packet;
  Timepoint m_time_of_disconnect;
};

}}
#endif
