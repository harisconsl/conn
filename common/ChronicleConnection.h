#ifndef _IN_COMMON_CHR_CONNECTION_H_
#define _IN_COMMON_CHR_CONNECTION_H_
#include "Connection.h"
//#include <chronicle/queue/impl/single/SingleChronicleQueue.h>
#include "Url.h"

namespace IN {
namespace COMMON {

class ChronicleConnection : public Connection
{
 public:
  static constexpr int DEFAULT_TIME_OUT = 100;
  
  virtual bool is_open() override;
  virtual std::string address() override;

  virtual int read(char* buf, int len) { }
  virtual int write(char* buf, int len) { }

  static ChronicleConnection* create(const Url& url, boost::asio::io_context& io_context);

  int open();
  void async_write(const char* buf, std::size_t len);
  void async_read();
  int get_fd();
private:
  ChronicleConnection(bool is_stream, boost::asio::io_context& io_context);
  PREVENT_COPY(ChronicleConnection);
  
  void do_close();

};                              

}}
#endif
