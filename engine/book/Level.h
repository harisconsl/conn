#ifndef _LEVEL_H_
#define _LEVEL_H_

#include <limits>

namespace IN{
namespace ENGINE{

struct Level
{
  int     size;
  double  price;
  
  Level()
    : size(0)
    , price(std::numeric_limits<double>::max())
  { }

  Level& operator+(const Level& other)
  {
    size += other.size;
    price +=other.price;
    return *this;
  }

  Level& operator+=(const Level& other)
  {
    size += other.size;
    price +=other.price;
    return *this;
  }

};

}}
#endif
