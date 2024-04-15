#ifndef _INTRADE_AGGBOOK_H_
#define _INTRADE_AGGBOOK_H_

#include "Side.h"
#include "Level.h"
#include "SymbolType.h"


namespace IN{
namespace ENGINE{

#define MAX_VENUES 32

union Venues
{
  char name[8];
  uint64_t id;
};

struct VenueQty
{
  uint64_t qty[MAX_VENUES];
};

template<typename T, int DEPTH>
class AggBook
{
public:
  
  T m_bid[DEPTH];
  T m_ask[DEPTH];

  VenueQty m_bid_venue_info[DEPTH];
  VenueQty m_ask_venue_info[DEPTH];
    
  uint8_t m_dealer_count;
  Venues m_venues[MAX_VENUES];
  SymbolType m_symbol;
  int m_bid_count;
  int m_ask_count;

  AggBook()
  {
    clear();
  }
  
  void clear()
  {
    for(int i = 0;i < DEPTH; i++)   
      {
        m_bid[i].clear();
        m_ask[i].clear();
      }
    m_bid_count = 0;
    m_ask_count = 0;
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
