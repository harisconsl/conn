#include "GWClient.h"

using namespace std;
using namespace IN::COMMON;

GWClient::GWClient() 
  : ConnClient()
  , m_logged_in(false)
  , m_count_packets(false)
  , m_packet_count(0)
{ }

void GWClient::on_packet(NPacket* p)
{
  // auto* conn = m_connection;
  // if (!conn)
  //   return;
  
  // ServiceType srvc;
  // MsgType mt = MsgType::null;
  
  // if (!get(*p, srvc))
  //   LOG_E("Cannot extract Service name in GWClient from : " << conn->address());
     

  // if (srvc != ServiceType::GWService)
  //   LOG_E("Unknown Service= " << srvc << " in GWClient expected= " << ServiceType::GWService
  //         " from: " << conn->address());
     
  // if (!get(*p, mt))
  //   LOG_E("Cannot extract msg type in GWClient from : " << conn->address());

  // auto iter = m_dispatch->find(make_pair(srvc,mt));
  // if (iter == m_dispatch->end())
  //   LOG_E("In Service[" << srvc << "] Unknown Msg Type[" << mt << "] in GWClient from : " << conn->address());
  
  // if (!iter->second(this,p))
  //   LOG_E("In Service[" << srvc << "] Cannot handle Msg Type[" << mt << "] in GWClient from : " << conn->address());

  // if (!is_connected())
  //   {
  //     return;
  //   }

  // Timepoint st;
  // if (!get(*p, st))
  //   LOG_E("Cannot extract source time in GWClient from : " << conn->address());
}

bool GWClient::is_logged_in() const
{
  return m_logged_in;
}

// bool GWClient::send(const Logon& lg)
// {
//   RecursiveLock lock(m_lock);
//   if (!is_connected())
//     return false;

//   m_packet.clear();
//   return  serialize_Packet(m_packet,lg)
//     && m_connection->send_packet(m_packet);
// }
