#ifndef _IN_COMMON_SIG_HANDLER_H_
#define _IN_COMMON_SIG_HANDLER__H_
#include <csignal>
#include <functional.h>

namespace IN {
namespace COMMON {

class SignalCatcher {
public:
  static SignalCatcher& get_instance();

  SignalCatcher();
  ~SignalCatcher();

  void handleSignal(int signal);
  void set_callback(std::funtional<void> callback);

private:
  std::function<void ()> m_callback;
};

}} // namespace IN COMMON
#endif
