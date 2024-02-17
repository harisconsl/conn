#ifndef _IN_COMMON_CONNLISTENER_H_
#define _IN_COMMON_CONNLISTENER_H_
namespace IN {
namespace COMMON {

class NPacket;
class ConnListener {
public:
  virtual void on_connect() = 0;
  virtual void on_packet(NPacket* p) = 0;
  virtual void on_disconnect() = 0;
};

}}
#endif
