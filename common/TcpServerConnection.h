#ifndef _IN_COMMON_TCP_SERVER_CONNECTION_H_
#define _IN_COMMON_TCP_SERVER_CONNECTION_H_
#include "ServerConnection.h"

namespace IN {
namespace COMMON {

class TcpServerConnection : public ServerConnection
{
 public:
  static constexpr int DEFAULT_TIME_OUT = 100;
  
  virtual bool is_open() override;
  virtual std::string address() override;
  virtual int get_fd();

  virtual int read(char* buf, int len) { }
  virtual int write(char* buf, int len) { }

  static TcpServerConnection* create(const Url& url, boost::asio::io_context& io_context);
  void start_accept();
private:
  TcpServerConnection(bool is_stream, boost::asio::io_context& io_context, int port);
  PREVENT_COPY(TcpServerConnection);

  boost::asio::ip::tcp::acceptor acceptor_;
  unsigned short m_port;

  void handle_client(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
  void do_close();  
};

}}
#endif
