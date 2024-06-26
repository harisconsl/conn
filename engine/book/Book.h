#ifndef _INTRADE_BOOK_H_
#define _INTRADE_BOOK_H_

#include "Side.h"
#include "Level.h"
#include "SymbolType.h"

namespace IN{
namespace ENGINE{

#define BOOK_DEPTH 20


template<typename T>
class Book {
public:
  
  T m_bid[BOOK_DEPTH];
  T m_ask[BOOK_DEPTH];
  int m_bid_count;
  int m_ask_count;
  SymbolType m_symbol;

 // friend class AggBook;

  Book()
  {
    clear();
  }

  Book(const Book& other)
  {
    m_symbol = other.m_symbol;
    for(int i = 0; i < other.m_bid_count ; ++i)
    {
      m_bid[i]= other.m_bid[i];
    }
    m_bid_count = other.m_bid_count;
    for(int i = 0; i < other.m_ask_count ; ++i)
    {
      m_ask[i]= other.m_ask[i];
    }
    m_ask_count = other.m_ask_count;
  }

  Book& operator=(const Book& other)
  {
    m_symbol = other.m_symbol;
    for(int i = 0; i < other.m_bid_count ; ++i)
    {
      m_bid[i]= other.m_bid[i];
    }
    m_bid_count = other.m_bid_count;
    for(int i = 0; i < other.m_ask_count ; ++i)
    {
      m_ask[i]= other.m_ask[i];
    }
    m_ask_count = other.m_ask_count;
    return *this;
  }
  
  void clear()
  {
    for(int i = 0;i < BOOK_DEPTH; i++)   
      {
        m_bid[i].clear();
        m_ask[i].clear();
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
    return (side == IN::ENGINE::Side::Buy) ? ((x < m_bid_count) ? m_bid[x].price : std::numeric_limits<double>::max() ) :
           ((x < m_ask_count) ? m_ask[x].price: std::numeric_limits<double>::max()) ; 
  }

  double size_at(IN::ENGINE::Side side, int x)
  {
    return (side == IN::ENGINE::Side::Buy) ? ((x < m_bid_count) ? m_bid[x].size: std::numeric_limits<double>::max() ) :
           ((x < m_ask_count) ? m_ask[x].size: std::numeric_limits<double>::max()) ; 
  }

};

}}
#endif
