#ifndef _IN_COMMON_SERVER_CONNECTION_H_
#define _IN_COMMON_SERVER_CONNECTION_H_

#include <string.h>
#include <boost/asio.hpp>

namespace IN {
namespace COMMON {

class ServerConnection
{
public:
  static constexpr int MAX_PACKET_SIZE = 1024*1024*8;

  static Connection* get_connection(const std::string& url, boost::asio::io_context& io_context);
  
  virtual ~Connection();

  virtual bool is_open() = 0;

  virtual std::string address() = 0;

  virtual int get_fd() = 0;

  bool add_listener(ConnListener* cl);

  bool send_packet(Packet* p);

  bool recv_packet();

  bool flush();

  virtual int read(char* buf, int len) = 0;

  virtual int write(char* buf, int len) = 0;
  
protected:
  
  boost::asio::io_context& m_io_context;
  std::string m_url;
  std::string m_address;
  int m_fd;

  // below
  bool m_verbose;
  bool m_is_stream;
  RingBuffer m_read_buf;
  RingBuffer m_write_buf;
  
  
private:
  ServerConnection(bool is_stream, boost::asio::io_context& io_context);
  static bool register();

  static bool m_initialized;
  static std::map<std::string, Connection*> m_conn_creator;

  PREVENT_COPY(ServerConnection);
};
 
}}
#endif
