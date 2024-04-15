#ifndef _INTRADE_STATEMACHINE_H_
#define _INTRADE_STATEMACHINE_H_

#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <iostream>


namespace msm = boost::msm;
namespace mpl = boost::mpl;

using namespace boost::msm::front::euml;

namespace IN{
namespace ENGINE{


// Events
struct Submit {};
struct PartialFill {};
struct Fill {};
struct Cancel {};


class ParentOrd
{
  enum class State
  {
    Init,
    Active,
    Canceled,
    Error,
    PError,
    Completed,
    PartialCompleted
  };
};

class ChildOrd
{
  enum class State
  {
    New,
    PendingNew,
    PartiallyFilled,
    Filled,
    Canceled,
    Replaced,
    PendingCancel,
    Rejected
  };
};

// State machine for Child Order
struct ChildOrderStateMachine_: public msm::front::state_machine_def<ChildOrderStateMachine_>
{
  // States
  struct New : public msm::front::state<> {};
  struct PartiallyFilled : public msm::front::state<> {};
  struct Filled : public msm::front::state<> {};
  struct Cancelled : public msm::front::state<> {};

  void print_pf(const PartialFill& p){ std::cout << "child order partially filled\n";}
  void print_f(const Fill& p){ std::cout << "child order  filled\n";}
  void print_c(const Cancel& p){ std::cout << "child order cancelled\n";}

  typedef ChildOrderStateMachine_ c;
  typedef New initial_state;

  struct transition_table : mpl::vector<
    //    Start              Event         Next      Action Guard
    //  +---------+-------------+---------+---------------------+----------------------+
    a_row < New             , PartialFill   , PartiallyFilled , &c::print_pf >,
    a_row < PartiallyFilled , Fill          , Filled          , &c::print_f >,
    a_row < New             , Cancel        , Cancelled       , &c::print_c >
    > {} ;
};

using ChildOrderStateMachine = msm::back::state_machine<ChildOrderStateMachine_>;

// State machine for Parent Order
struct ParentOrderStateMachine_: public msm::front::state_machine_def<ParentOrderStateMachine_>
{
  // States
  struct New : public msm::front::state<> {};
  struct Pending : public msm::front::state<> {};
  struct PartiallyFilled : public msm::front::state<> {};
  struct Filled : public msm::front::state<> {};
  struct Cancelled : public msm::front::state<> {};

  // Sub-state machine for child order
  using ChildOrderStateMachine = msm::back::state_machine<ChildOrderStateMachine_>;

  // Child order instance
  ChildOrderStateMachine childOrder;

  void sprint_ack(const Submit& p){ std::cout << "parent order acked \n";}
  void sprint_pf(const PartialFill& p){ std::cout << "parent order partial filled\n";childOrder.process_event(p);}
  void sprint_f(const Fill& p){ std::cout << "parent order filled\n"; childOrder.process_event(p);}
  void sprint_c(const Cancel& p){ std::cout << "parent order cancelled\n";childOrder.process_event(p);}
  
  typedef ParentOrderStateMachine_ p; // makes transition table cleaner
  typedef New initial_state;
  
  struct transition_table : mpl::vector<
    //    Start     Event         Next      Action Guard
    //  +---------+-------------+---------+---------------------+----------------------+
    a_row < New ,   Submit     , Pending , &p::sprint_ack      >,
    a_row < Pending , PartialFill  , PartiallyFilled   , &p::sprint_pf        >,
    //    a_row < PartiallyFilled , Fill  , Filled   , &p::sprint_pf        >,
    a_row < PartiallyFilled , Fill  , Filled   , &p::sprint_f        >,
    a_row < Pending , Fill  , Filled   , &p::sprint_f                         >,
    a_row < New , Cancel  , Cancelled   , &p::sprint_c                         >
    >{};

using ParentOrderStateMachine = msm::back::state_machine<ParentOrderStateMachine_>;


}}
#endif
