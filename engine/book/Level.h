#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <limits>

namespace IN{
namespace ENGINE{

struct Level
{
  EXCHANGE ex;
  int     size;
  double  price;

  Level()
    : ex(EXCHANGE::null)
    , size(0)
    , price(std::numeric_limits<double>::max())
  { }

 void clear()
  {
    ex = EXCHANGE::null;
    size = 0;
    price = std::numeric_limits<double>::max();
  }

  Level& operator+(const Level& other)
  {
    ex = other.ex;
    size = other.size;
    price = other.price;
    return *this;
  }

  Level& operator+=(const Level& other)
  {
    ex = other.ex;
    size = other.size;
    price = other.price;
    return *this;
  }

  bool operator<(const Level& other)
  {
    return price < other.price;
  }

   bool operator>(const Level& other)
  {
    return price > other.price;
  }

};

}}
#endif
