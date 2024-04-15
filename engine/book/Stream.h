#ifndef _INTRADE_STREAM_H_
#define _INTRADE_STREAM_H_

#include<set>

#include "StreamConfig.h"
#include "Book.h"
#include "AggBook.h"
#include "L2SnapshotLite.h"
#include "../../common/Utils.h"
#include "../../common/Logger.h"

namespace IN{
namespace ENGINE{


#define AVG_LEVELS_PER_VENUE 16

struct LevelRules
{
  int size;
  std::vector<EXCHANGE> m_exchs;  
};

class Stream
{
public:
  Stream(std::string name, std::string symbol, StreamConfig& cfg)
    : m_name(name)
    , m_symbol(symbol)
    , m_stream_cfg(cfg)
  { 
    m_bid_sweepable_vec.reserve(m_stream_cfg.secondary_exch_sweepable_vec.size() * AVG_LEVELS_PER_VENUE);
    m_ask_sweepable_vec.reserve(m_stream_cfg.secondary_exch_sweepable_vec.size() * AVG_LEVELS_PER_VENUE);

  }

  bool isOrderBook(EXCHANGE exch)
  {
    return true;
  }

  void verticalIntergrationPrice()
  {
    double last_price; 
    Level bid;
    if ( m_composite_order_book.m_bid_count > 0)
    {
      bid = m_composite_order_book.m_bid[0];
      last_price = bid.price;
    }
    int j = 0;
    for (int i = 1; i < m_composite_order_book.m_bid_count  ;++i)
      {
        Level l = m_composite_order_book.m_bid[i];
         if (last_price != l.price)
          {
            m_composite_price_order_book.m_bid[j] = bid;
            ++j;
            bid = l;
          }
        bid.price = (bid.price * bid.size + l.price * l.size)/(bid.size + l.size);
        bid.size += l.size; 
      }
      m_composite_price_order_book.m_bid[j] = bid;

    Level ask;
    if ( m_composite_order_book.m_ask_count > 0)
    {
      ask =  m_composite_order_book.m_ask[0];
      last_price = ask.price;
    }
    j = 0;
    for (int i = 1; i < m_composite_order_book.m_ask_count  ;++i)
      {
        Level l = m_composite_order_book.m_ask[i];
         if (last_price != l.price)
          {
            m_composite_price_order_book.m_ask[j] = ask;
            ++j;
            ask = l;
          }
        ask.price = (ask.price * ask.size + l.price * l.size)/(ask.size + l.size);
        ask.size += l.size;
      }
      m_composite_price_order_book.m_ask[j] = ask;
  }

  void verticalIntergrationLevel()
  {
    int lcount = 0;
    Level bid;
    for (int i = 0; i < m_composite_order_book.m_bid_count ; ++i)
      {
        Level l = m_composite_order_book.m_bid[i];
        if (lcount >= m_stream_cfg.level_vec.size())
          break;
        
        while ( lcount < m_stream_cfg.level_vec.size() && m_stream_cfg.level_vec[lcount] <= bid.size + l.size)
          {
            int vsiz = m_stream_cfg.level_vec[lcount] - bid.size ;
            Level new_bid;
            new_bid.size = m_stream_cfg.level_vec[lcount] ;
            new_bid.price = ((l.price * vsiz) + (bid.price * bid.size))/(bid.size+ vsiz);
            // insert in book
            m_order_book.m_bid[lcount]= new_bid;
            ++lcount;                
          }
        bid.price = ((l.price * l.size) + (bid.price * bid.size))/(bid.size + l.size);
        bid.size += l.size;
      }

    lcount = 0;
    Level ask ;
    for (int i = 0; i < m_composite_order_book.m_ask_count  ;++i)
      {
        Level l = m_composite_order_book.m_ask[i];
        if (lcount >= m_stream_cfg.level_vec.size())
          break;

        while ( lcount < m_stream_cfg.level_vec.size() && m_stream_cfg.level_vec[lcount] <= bid.size + l.size)
          {
            int vsiz = m_stream_cfg.level_vec[lcount] - ask.size ;
            Level new_ask;
            new_ask.size = m_stream_cfg.level_vec[lcount] ;
            new_ask.price = ((l.price * vsiz) + (ask.price * ask.size))/(ask.size + vsiz);
            // insert in book
            m_order_book.m_ask[lcount]= new_ask;
            ++lcount;                
          }
        ask.price = ((l.price * l.size) + (ask.price * ask.size))/(ask.size + l.size);
        ask.size += l.size;
      }
  }

  void mergeOrderBook()
  {
    // merge different book to 1 book according to price
    // band based aggregation and vwap in order book with different venues
    // copy prices in a vector and 
    int64_t bid_counter = 0;
    int64_t ask_counter = 0;
    for (auto book : m_l2Snapshots_order_book)
    {
      for (int i = 0; i < book.second->bid_count; ++i)
      {
        m_composite_order_book.m_bid[bid_counter] = book.second->bid[i];
        ++bid_counter;
      }
      m_composite_order_book.m_bid_count += book.second->bid_count;

      for (int i = 0; i < book.second->ask_count; ++i)
      {
        m_composite_order_book.m_ask[ask_counter] = book.second->ask[i];
        ++ask_counter;
      }
      m_composite_order_book.m_ask_count += book.second->ask_count;
    }

    // insertion sort for 
    IN::COMMON::insertion_sort<Level>(m_composite_order_book.m_bid, m_composite_order_book.m_bid_count);
    IN::COMMON::insertion_sort<Level>(m_composite_order_book.m_ask, m_composite_order_book.m_ask_count);

    verticalIntergrationLevel();
    verticalIntergrationPrice();
  }


  void onSnapshot(const L2SnapshotLite* msg)
  {
    // if  exchange is order book exchange
    if (isOrderBook(msg->exchange))
      mergeOrderBook();
    
    // merge the book according to rule
    m_final_book.clear();

    // 1.  for final book check the price at all the fullamount book first
    m_fullamount_book.clear();

    // According to levels, fullamount book sorting
    int fullamount_level_count = 0;
    for (auto lit : m_stream_cfg.level_vec)
    {
      for (auto it = m_l2Snapshots_fullamount_book.begin(); it != m_l2Snapshots_fullamount_book.end(); ++it)
      {
        // on first iteration copy the book
        for (int i = 0; i < it->second->bid_count; ++i)
        {
          if (lit <= it->second->bid[i].size)
          {
            if (m_fullamount_book.m_bid[fullamount_level_count].price < it->second->bid[i].price)
              m_fullamount_book.m_bid[i].price = it->second->bid[i].price;
          }
        }

        for (int i = 0; i < it->second->ask_count; ++i)
        {
          if (lit <= it->second->ask[i].size)
          {
            if (m_fullamount_book.m_ask[fullamount_level_count].price < it->second->ask[i].price)
              m_fullamount_book.m_ask[fullamount_level_count].price = it->second->ask[i].price;
          }
        }
      }
      ++fullamount_level_count;
    }

    // 2. copy fullamount book to m_final_book

    for (int j = 0; j < m_fullamount_book.m_bid_count; ++j)
    {
      m_final_book.m_bid[j] = m_fullamount_book.m_bid[j];
    }
    for (int j = 0; j < m_fullamount_book.m_ask_count; ++j)
    {
      m_final_book.m_ask[j] = m_fullamount_book.m_ask[j];
    }
    m_final_book.m_bid_count = m_fullamount_book.m_bid_count;
    m_final_book.m_ask_count = m_fullamount_book.m_ask_count;

    // 3. sweepable book
    for (auto it = m_l2Snapshots_sweepable_book.begin(); it != m_l2Snapshots_sweepable_book.end(); ++it)
    {
      for (int j = 0; j < it->second->bid_count; ++j)
      {
          m_bid_sweepable_vec.push_back(it->second->bid[j]);
      }

      for (int j = 0; j < it->second->ask_count; ++j)
      {
          m_ask_sweepable_vec.push_back(it->second->ask[j]);
      }
    }
    // we can also add exchange priority in this
    // sort according to descending price and descending qty 
    std::sort(m_bid_sweepable_vec.begin(), m_bid_sweepable_vec.end(),
            [](const Level& a, const Level& b)
            {
              if (a.price != b.price)
              {
                return b.price < a.price;
              }
              return b.size < a.size;
            });
    // ascending price and decreasing qty
    std::sort(m_ask_sweepable_vec.begin(), m_ask_sweepable_vec.end(),
            [](const Level& a, const Level& b)
            {
              if (a.price != b.price)
              {
                return a.price < b.price;
              }
              return b.size < a.size;
            });

    int level_count = 0;
    for (auto lit : m_stream_cfg.level_vec)
    {
      // check the tier which need to create
      int cum_size = 0;
      std::unordered_map<EXCHANGE, Level> res;
      for (auto vs = m_bid_sweepable_vec.begin(); vs != m_bid_sweepable_vec.end(); ++vs)
      {
        if (cum_size < lit)
        {
          // check if this price is greater the final book full amount price
          auto mit = res.find(vs->ex);
          if (mit == res.end())
          {

            if (cum_size + vs->size <= lit)
            {
              res.insert(std::pair<EXCHANGE, Level>(vs->ex, *vs));
              cum_size += vs->size;
            }
            else
            {
              Level mod_l = *vs;
              vs->size = lit - cum_size;
              res.insert(std::pair<EXCHANGE, Level>(mod_l.ex, mod_l));
            }
          }
          else
          {
            if (cum_size == lit)
              break;

            if (vs->size - res[vs->ex].size > 0)
            {
              if (cum_size + vs->size - res[vs->ex].size <= lit)
              {
                res[vs->ex].price = vs->price;
                res[vs->ex].size = vs->size;
              }
              else
              {
                res[vs->ex].price = vs->price;
                res[vs->ex].size = cum_size + vs->size - res[vs->ex].size - lit;
              }
            }
          }
        }
        else
          break;
      }

      if (cum_size < lit)
      {
        LOG_I("Not able to get the tier from existing liquidity providers");
        break;
      }
      else
      {
        // vwap calculation by iterating the vector and putting the price in final_book
        double cum_price_siz = 0.0;
        double tot_size = 0;
        for (auto r = res.begin(); r != res.end(); ++r)
        {
          // for SOR we need to keep all the venues in final book
         cum_price_siz += r->second.size * r->second.price;
         tot_size += r->second.size;
        }
        
        double cum_price = cum_price_siz/tot_size;
        if (m_final_book.m_bid[level_count].price > cum_price)
        {
          m_final_book.m_bid[level_count].price = cum_price_siz / tot_size;
          m_final_book.m_bid[level_count].size = tot_size;
        }
        cum_size = 0;
        res.clear();
      }

      // ask side
      for (auto vs = m_ask_sweepable_vec.begin(); vs != m_ask_sweepable_vec.end(); ++vs)
      {
        if (cum_size < lit)
        {
          // check if this price is greater the final book full amount price
          auto mit = res.find(vs->ex);
          if (mit == res.end())
          {

            if (cum_size + vs->size <= lit)
            {
              res.insert(std::pair<EXCHANGE, Level>(vs->ex, *vs));
              cum_size += vs->size;
            }
            else
            {
              Level mod_l = *vs;
              vs->size = lit - cum_size;
              res.insert(std::pair<EXCHANGE, Level>(mod_l.ex, mod_l));
            }
          }
          else
          {
            if (cum_size == lit)
              break;

            if (vs->size - res[vs->ex].size > 0)
            {
              if (cum_size + vs->size - res[vs->ex].size <= lit)
              {
                res[vs->ex].price = vs->price;
                res[vs->ex].size = vs->size;
              }
              else
              {
                res[vs->ex].price = vs->price;
                res[vs->ex].size = cum_size + vs->size - res[vs->ex].size - lit;
              }
            }
          }
        }
        else
          break;
      }

      if (cum_size < lit)
      {
        LOG_I("Not able to get the tier from existing liquidity providers");
        break;
      }
      else
      {
        // vwap calculation by iterating the vector and putting the price in final_book
        double cum_price_siz = 0.0;
        double tot_size = 0;
        for (auto r = res.begin(); r != res.end(); ++r)
        {
          // for SOR we need to keep all the venues in final book
         cum_price_siz += r->second.size * r->second.price;
         tot_size += r->second.size;
        }
        double cum_price = cum_price_siz/tot_size;
        if (m_final_book.m_ask[level_count].price < cum_price)
        {
          m_final_book.m_ask[level_count].price = cum_price;
          m_final_book.m_ask[level_count].size = tot_size;
        }

        cum_size = 0;
        res.clear();
      }
      ++level_count;
    }
  }

  std::string get_symbol() const
  {
    return m_symbol;
  }

  const std::vector<EXCHANGE>& get_primary_exchanges() const
  {
    return m_primary_exch_vec;
  }

private:
  std::string m_name;
  std::string m_symbol;
  StreamConfig m_stream_cfg;

  std::vector<EXCHANGE> m_primary_exch_vec;
  std::vector<EXCHANGE> m_secondary_exch_vec;
  std::vector<EXCHANGE> m_secondary_exch_fullamount_vec;
  std::vector<EXCHANGE> m_secondary_exch_sweepable_vec;

  std::map<EXCHANGE, const L2SnapshotLite*> m_l2Snapshots_fullamount_book;
  std::map<EXCHANGE, const L2SnapshotLite*> m_l2Snapshots_sweepable_book;
  std::map<EXCHANGE, const L2SnapshotLite*> m_l2Snapshots_order_book;

  
  // with vertical integration with best prices
  Book<Level> m_order_book;
  AggBook<Level, 10> m_fullamount_book;
 

  Book<Level> m_final_book;
  Book<Level> m_visual_book;

  // Derived book
  AggBook<Level, 100> m_composite_order_book;
  AggBook<Level, 100> m_composite_price_order_book;

  std::vector<Level> m_bid_sweepable_vec;
  std::vector<Level> m_ask_sweepable_vec;
  std::vector<int> m_bid_sweepable_size_vec;
  std::vector<int> m_ask_sweepable_size_vec;

  // vertical integration book need to be knowing what composition of book in a level 
  // final book should keep record from which book this has come (primary, secondary, fullamount, sweepable)
  // on main book rules are applied to create maker book
  // on taker  book we apply rules for priority

};

}}
#endif
