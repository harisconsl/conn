#ifndef _IN_COMMON_CONNECTION_H_
#define _IN_COMMON_CONNECTION_H_

#include <string.h>

namespace IN {
namespace COMMON {

class Connection
{
 public:
  static constexpr int MAX_PACKET_SIZE = 1024*1024*8;

  static Connection* get_connection(const std::string& url);
  
  virtual ~Connection();

  virtual bool is_open() = 0;

  virtual bool std::string address() = 0;

  bool add_listener(ConnListener* cl);

  bool send_packet(Packet* p);

  bool recv_packet();

  bool flush();
 private:
  Connection(bool isstream);
  static bool register();

  Connection* get_mcast_connection();
  Connection* get_tcp_connection();
  Connection* get_unix_connection();
  Connection* get_shm_connection();

  static bool m_initialized;
  static std::map<std::string, >

  PREVENT_COPY(Connection);
  
  std::string m_url;
  std::string m_address;
  int m_fd;
};
 
}}
#endif
