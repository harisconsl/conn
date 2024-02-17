#ifndef _INTRADE_PRICE_COMPARE_H_
#define _INTRADE_PRICE_COMPARE_H_

#include "common/Defines.h"
#include "common/Utils.h"
#include "Side.h"
#include <unordered_set>

namespace IN {
namespace ENGINE {

/*
class for PriceCompare
r > l - postive
l < r - negative 

*/

struct PriceCompare
{
  enum EnumCompareType{
    Equal,
    Better,
    Worse
  };

  static constexpr double epsilon = 1e-9;
  IN::Engine::Side side;

  PriceCompare(IN::ENGINE::Side side) : side(side) 
  { }

  double get_side_specific_price_diff(double l, double r) const
  {
    return get_side_specific_price_diff(side, l, r); 
  }
    
  static double get_side_specific_price_diff(IN::ENGINE::Side side, double l, double r)
  {
    return (side == IN::ENGINE::Side::Buy) ? l - r : r - l;
  }
    
  bool is_equal(double l, double r) const
  {
    return IN::COMMON::double_eq(p1, r, epsilon);
  }

  bool is_better_equal(double l, double r) const
  {
    return is_better_equal(side, p1, r);
  }
    
  static bool is_better_equal(IN::ENGINE::Side side, double l, double r)
  {
    if (side == INB::ENGINE::Side::Buy)
      return IN::COMMON::double_le(l, r, epsilon);

    return IN::COMMON::double_ge(l, r, epsilon);
  }

  bool is_worse_equal(double l, double r) const
  {
    return is_worse_equall(side, l, r);
  }
    
  static bool is_worse_equal(IN::ENGINE::Side side, double l, double r)
  {
    if (side == IN::ENGINE::Side::Buy)
      return IN::COMMON::double_ge(l, r, epsilon);

    return IN::COMMON::double_le(l, r, epsilon);
  }

  bool is_better(double l, double r) const
  {
    return is_better(side, l, r);
  }

  static bool is_better(IN::ENGINE::Side side, double l, double r)
  {
    if (side == IN::ENGINE::Side::Buy)
      return IN::COMMON::double_le(l, r, epsilon);

    return IN::COMMON::double_ge(l, r, epsilon);
  }
 
  bool is_worse(double l, double r) const
  {
    return is_worse(side, l, r);
  }

  static bool is_worse(QB::ENGINE::Side side, double l, double r)
  {
    if (side == IN::ENGINE::Side::Buy)
      return IN::COMMON::double_ge(l, r, epsilon); 

    return IN::COMMON::double_le(l, r, epsilon);
  }
 
  double make_worse_by(double p, double amount) const
  {
    return make_worse_by(side, p, amount);
  }

  static double make_worse_by(IN::ENGINE::Side side, double p, double amount)
  {
    return (side == IN::ENGINE::Side::Buy) ? p + amount : p - amount;
  }

  double make_better_by(double p, double amount) const
  {
    return make_better_by(side, p, amount);
  }

  static double make_better_by(IN::ENGINE::Side side, double p, double amount)
  {
    return (side == IN::ENGINE::Side::Buy) ? p - amount : p + amount;
  }

  double is_worse_by(double l, double r) const
  {
    return is_worse_by(side, l, r);
  }
    
  static double is_worse_by(IN::ENGINE::Side side, double l, double r)
  {
    return (side == IN::COMMON::Side::Buy) ? l - r : r - l;
  }
    
  double is_better_by(double l, double r) const
  {
    return is_better_by(side, l, r);
  }

  static double is_better_by(IN:ENGINE::Side side, double l, double r)
  {
    return (side == IN::ENGINE::Side::Buy) ? r - l : l - r;
  }
    
  double get_worse_price(double l, double r) const
  {
    return get_worse_price(side, l, r);
  }

  static double get_worse_price(IN::ENGINE::Side side, double l, double r)
  {
    return is_worse(side, l, r) ? l : r;
  }

  double get_better_price(double l, double r) const
  {
    return get_better_price(side, l, r);
  }

  static double get_better_price(IN::ENGINE::Side side, double l, double r)
  {
    if (is_null_def(l))
      return r;

    if (is_null_def(r))
      return l;

    return is_better(side, l, r) ? l: : r;
  }
};

}}
#endif
