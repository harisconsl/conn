#ifndef _IN_COMMON_UNIX_SERVER_CONNECTION_H_
#define _IN_COMMON_UNIX_SERVER_CONNECTION_H_

#include "ServerConnection.h"

namespace IN {
namespace COMMON {

class UnixServerConnection: public ServerConnection
{
public:
  static constexpr int DEFAULT_TIME_OUT = 100;
  
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();
  static UnixServerConnection* create(const Url& url, boost::asio::io_context& io_context);
  bool start_accept();

  virtual int read(char* buf, int len) { }
  virtual int write(char* buf, int len) { }
private:
  UnixServerConnection(bool is_stream, boost::asio::io_context& io_context, std::string path);
  PREVENT_COPY(UnixServerConnection);

  boost::asio::local::stream_protocol::acceptor acceptor_;

  void handle_client(std::shared_ptr<boost::asio::local::stream_protocol::socket> socket);
  void do_close();
};

}}
#endif
