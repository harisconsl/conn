#ifndef _IN_COMMON_CONNECTION_H_
#define _IN_COMMON_CONNECTION_H_

#include <functional>
#include <string.h>
#include <boost/asio.hpp>
#include "Macros.h"
#include "Url.h"
#include "RingBuffer.h"


namespace IN {
namespace COMMON {

class ConnListener;
class Packet;

class Connection
{
public:
  static constexpr int MAX_PACKET_SIZE = 1024*1024*8;

  static Connection* get_connection(const std::string& url, boost::asio::io_context& io_context);
  
  virtual ~Connection(){}

  virtual bool is_open() = 0;

  virtual std::string address() = 0;

  virtual int get_fd() = 0;

  virtual void close() final;
 
  bool add_listener(ConnListener* cl);

  bool send_packet(Packet* p);

  bool recv_packet();

  bool flush();

  virtual int read(char* buf, int len) = 0;

  virtual int write(char* buf, int len) = 0;

  static bool register_factory();

  
protected:
  
  std::string m_url;
  std::string m_address;
  int m_fd;

  // below
  bool m_verbose;
  bool m_is_stream;
  RingBuffer m_read_buf;
  RingBuffer m_write_buf;
  
  Connection(bool is_stream);
private:
  virtual void do_close() = 0;

  static bool m_initialized;
  static std::map<const std::string, std::function<Connection* (const Url& url, boost::asio::io_context& io_context)>> m_conn_creator;

  PREVENT_COPY(Connection);
};
 
}}
#endif
