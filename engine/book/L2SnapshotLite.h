#ifndef _L2SNAPSHOT_LITE_H_
#define _L2SNAPSHOT_LITE_H_

#include "Exchange.h"
#include "Level.h"
#include <array>
#include <map>
#include <vector>

namespace IN{
namespace ENGINE{

static constexpr int MAX_DEPTH = 20;

enum class SnapshotType
{
  LATE_JOIN,
    RESET,
    PASSTHRU,
    NULL_VALUE
};

enum class BookType
{
  TRADING_HALT,
    CLOSE,
    NEW_PRICE_INDICATION,
    READY_TO_TRADE,
    NOT_AVAILABLE_FOR_TRADING,
    UNKNOWN_OR_INVALID,
    PRE_OPEN,
    PRE_CROSS,
    CROSS,
    PST_CLOSE,
    NO_CHANGE,
    SESSION_ERROR,
    SESSION_RESET,
    SESSION_GAP,
    NULL_VALUE
};

enum class FXBookType
{
  FULLAMOUNT,
    SWEEPABLE,
    CLOB,
    EDF
};


struct L2SnapshotLite
{
  std::string symbol;
  EXCHANGE exchange;
  int64_t security_id;

  double mid_price;
  double mid_price_denom;
  SnapshotType snapshot_type;
  BookType book_type;
  FXBookType fxbook_type;
  std::array<Level, MAX_DEPTH> bid;
  std::array<Level, MAX_DEPTH> ask;

  uint64_t bid_count;
  uint64_t ask_count;
  
  int64_t seq_num;
  int64_t recv_time;
  int64_t publish_time;
  std::string user_text;
};

}}
#endif
