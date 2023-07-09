#ifndef _IN_COMMON_MCAST_CONNECTION_H_
#define _IN_COMMON_MCAST_CONNECTION_H_

#include "Connection.h"

namespace IN {
namespace COMMON {

typedef boost::asio::ip bai;
class McastConnection
{
public:
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();
  
private:
  McastConnection(bool is_stream);
  PREVENT_COPY(McastConnection);

  bai::udp::socket _socket;

  void handle_connect(const boost::system::error_code& ec);
  
  void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void do_close();
};

}}
#endif
