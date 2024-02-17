X#ifndef _INTRADE_STREAM_H_
#define _INTRADE_STREAM_H_
#include "Exchange.h"
#include "Book.h"
#include "L2SnapshotLite.h"

namespace IN{
namespace ENGINE{

struct LevelRules
{
  int size;
  std::vector<EXCHANGE> m_exchs;
  
};

class Stream
{
public:
  Stream(std::string name, std::string symbol)
    : m_name(name)
    , m_symbol(symbol)
  {

  }

  void onSnapshot(const L2SnapshotLite* msg)
  {
    // merge the book according to rule
    // we need to merge the book again
    m_book.clear();
    for (auto it: m_snapshots)
      {
        int i = 0;
        for (auto l : msg->bid)
          {
            m_book.m_bid[i] += l; ++i;
          }
        i = 0;
        for (auto l : msg->ask)
          {
            m_book.m_ask[i] += l; ++i;
          }
      }
  }

  std::string get_symbol() const
  {
    return m_symbol;
  }

  const std::vector<EXCHANGE>& get_exchanges() const
  {
    return m_exchs;
  }
private:
  std::string m_name;
  std::string m_symbol;
  // last updated time
  std::vector<EXCHANGE> m_exchs;
  std::vector<size_t> m_levels;
  // level rules
  std::map<size_t, LevelRules> m_map_level_rules;
  std::map<EXCHANGE, const L2SnapshotLite*> m_snapshots;
  Book<Level> m_book;
};

}}
#endif
