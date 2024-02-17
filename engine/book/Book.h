#ifndef _INTRADE_BOOK_H_
#define _INTRADE_BOOK_H_

#include "Side.h"
#include "Level.h"

namespace IN{
namespace ENGINE{

#define BOOK_DEPTH 20

template<typename T>
class Book {
public:
  
  T m_bid[BOOK_DEPTH];
  T m_ask[BOOK_DEPTH];

  Book()
  {
    clear();
  }
  
  void clear()
  {
    for(int i = 0;i < BOOK_DEPTH; i++)   
      {
        m_bid[i].price = std::numeric_limits<double>::max();
        m_bid[i].size = 0;
        m_ask[i].price = std::numeric_limits<double>::max();
        m_ask[i].size = 0;
      }
  }

  double bid_at(int x)
  {
    return m_bid[x].price;
  }

  double bid_size_at(int x)
  {
    return m_bid[x].size;
  }
  double ask_at(int x)
  {
    return m_ask[x].price;
  }
  
  double ask_size_at(int x)
  {
    return m_ask[x].size;
  }
  
  double price_at(IN::ENGINE::Side side, int x)
  {
    return (side == IN::ENGINE::Side::Buy) ? m_bid[x].price : m_ask[x].price; 
  }

  double size_at(IN::ENGINE::Side side, int x)
  {
    return (side == IN::ENGINE::Side::Buy) ? m_bid[x].size : m_ask[x].size; 
  }

};

}}
#endif
