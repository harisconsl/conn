#include "SignalCatcher.h"
#include "Logger.h"

using namespace IN::COMMON;

SignalCatcher& SignalCatcher::get_instance()
{
  static SignalCatcher instance;
  return instance;
}

SignalCatcher::SignalCatcher()
{
  //  std::signal(SIGINT, SignalCatcher::handle_signal);
  // std::signal(SIGTERM, SignalCatcher::handle_signal);
}

void SignalCatcher::handle_signal(int signal)
{
  if (signal == SIGINT)
    {
      LOG_I("SIGINT signal received." );
    }
  else if (signal == SIGTERM)
    {
      LOG_I("SIGTERM signal received.");
    }
  
  // Optionally, restore the default signal handler
  std::signal(signal, SIG_DFL);
  if (m_callback)
    m_callback(signal);
}

void SignalCatcher::set_callback(std::function<void(int)> callback)
{
  m_callback = callback;
}



