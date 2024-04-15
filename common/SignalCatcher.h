#ifndef _IN_COMMON_SIG_HANDLER_H_
#define _IN_COMMON_SIG_HANDLER__H_
#include <csignal>
#include <functional>

namespace IN {
namespace COMMON {

class SignalCatcher
{
public:
  static SignalCatcher& get_instance();
  
  SignalCatcher();
  ~SignalCatcher();

  void handle_signal(int signal);
  void set_callback(std::function<void(int)> callback);
private:
  std::function<void(int)> m_callback;
};

}} // namespace IN COMMON
#endif
