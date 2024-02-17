#ifndef _IN_CONNCLIENT_H_
#define _IN_CONNCLIENT_H_

#include <string>
#include <thread>

#include "Connection.h"
#include "Packet.h"
#include "ConnListener.h"
#include "RecursiveLock.h"
#include "Logger.h"

namespace IN {
namespace COMMON {

class Connection;
class NPacket;

class ConnClient : public ConnListener 
{
public:
  ConnClient();
  virtual ~ConnClient();

  bool connect(const std::string& url, boost::asio::io_context& io_context);
  void  disconnect();

  Connection* get_connection();
  bool is_connected();
  
  virtual void on_connect() = 0;
  virtual void on_packet(NPacket* p) = 0;
  virtual void on_disconnect() {LOG_I("Not implemented");};
    
  std::string url() const;
protected:
  std::string m_url;
  Connection*  m_connection;
  std::recursive_mutex m_lock;
  NPacket*  m_packet;
};

}}
#endif
