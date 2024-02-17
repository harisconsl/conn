#ifndef _IN_GWSERVER_H_
#define _IN_GWSERVER_H_
#include "ConnServer.h"
#include "RecursiveLock.h"
#include "Connection.h"
#include "Logger.h"

namespace IN {
namespace COMMON {

class GWServer : public ConnServer
{
public:
  GWServer();
  virtual ~GWServer();

  virtual void on_packet(NPacket* p) final;


  virtual void on_connect() = 0;
  virtual void on_clientDisconnect() = 0;

  bool send(const LogonResp& l);

  template <typename T>
  bool send(const T& v)
  {
    return send_packet(v);
  }

  bool send(NPacket* p)
  {
    RecursiveLock lock(m_lock);
    if (!p)
      {
        LOG_E("Invalid null packet");
        return false;
      }

    if (!is_connected())
      {
        LOG_E("Async sending failed, either not logged in or connection is null");
        return false;
      }

    if (m_connection->send_packet(*p))
      return true;
    
    return false;
  }

private:
  template <typename T> 
  bool serialize_packet(NPacket& packet, const T& obj)
  {
    return put(m_packet, ServiceType::GWService)
      && put(m_packet, obj.msg_type)
      && put(m_packet, obj)
      && put(m_packet, Timepoint::now());
  }


  template <typename... Args> 
  bool send_packet(const Args&... args)
  {
    RecursiveLock lock(m_lock);
    if (!is_connected())
      {
        LOG_E("Sending failed, either not logged in or connection is null");
        return false;
      }

    m_packet.clear();
    bool ret = serialize_packet(m_packet,args...);

    if (!ret)
      {
        LOG_E("Failed to serialize msg!");
        return false;
      }
    
    if (m_connection->send_packet(m_packet))
      return true;
  }

};

}} //namespace ends
