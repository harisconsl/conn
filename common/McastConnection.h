#ifndef _IN_COMMON_MCAST_CONNECTION_H_
#define _IN_COMMON_MCAST_CONNECTION_H_

#include "Connection.h"
#include "Url.h"

namespace IN {
namespace COMMON {

class McastConnection : public Connection
{
public:
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();

  void join_group();
  void leave_group();

  static McastConnection* create(const Url& url, boost::asio::io_context& io_context);

  virtual int read(char* buf, int len) { }
  virtual int write(char* buf, int len) { }

  boost::asio::ip::tcp::socket get_socket();

  void async_read();
private:
  McastConnection(bool is_stream, boost::asio::io_context& io_context);
  PREVENT_COPY(McastConnection);

  void handle_connect(const boost::system::error_code& ec);  
  void handle_recv(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void do_close(){}

  boost::asio::ip::udp::socket socket_;
  unsigned short m_port;
  std::string m_interface_addr;
};

}}
#endif
