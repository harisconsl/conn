#include "SignalCatcher.h"

SignalCatcher& SignalCatcher::get_instance()
{
  static SignalCatcher instance;
  return instance;
}

SignalCatcher::SignalCatcher()
{
  std::signal(SIGINT, &SignalCatcher::handleSignal);
  std::signal(SIGTERM, &SignalCatcher::handleSignal);
}

void SignalCatcher::handle_signal()
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
    m_callback();
}

void SignalCatcher::set_callback(std::funtional<void> callback)
{
  m_callback = callback;
}



