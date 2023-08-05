#ifndef _IN_COMMON_SHM_CONNECTION_H_
#define _IN_COMMON_SHM_CONNECTION_H_

#include "Connection.h"
#include "Url.h"
namespace IN {
namespace COMMON {

class ShmConnection : public Connection
{
 public:
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();

  virtual int read(char* buf, int len) { }
  virtual int write(char* buf, int len) { }

  static ShmConnection* create(const Url& url, boost::asio::io_context& io_context);
  int open();
  void async_read();

 private:
  ShmConnection(bool is_stream, boost::asio::io_context& io_context);
  PREVENT_COPY(ShmConnection);
  
  boost::asio::posix::stream_descriptor stream_;
  void* mapped_memory;  
  int m_fd;
  void handle_connect(const boost::system::error_code& ec);
  void handle_write(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
  void do_close();
};

}}
#endif
