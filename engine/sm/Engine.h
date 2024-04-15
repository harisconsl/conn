#ifndef _INTRADE_ENGINE_H_
#define _INTRADE_ENGINE_H_


namespace IN{
namespace ENGINE{

class Engine
{

private:
  ConfigSM config;
  // separate timer thread for taking action
  Timer   m_timer;
  
  ParentOrdMap m_po_map;
  ChildOrdMap m_child_map;

  // Manages strategy component 
  StrategyManger m_strategy_maanger;

  // read on the basis of any external signals
  //  SignalTracker m_signal_tracker;

  //Can read from mutliple clients/outbound gateway
  ClientManager m_client_manager;

  // io_loop to read market data and other events in loop
  
};

}}
#endif
