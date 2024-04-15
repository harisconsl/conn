#ifndef _IN_COMMON_UTILS_H_
#define _IN_COMMON_UTILS_H_

#include <algorithm>
#include <stdint.h>
#include <string>
#include <cmath>
#include <unistd.h>

namespace IN {
namespace COMMON {

static inline void start_trim(std::string &s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch){return !std::isspace(ch);}));
}

static inline void end_trim(std::string &s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch){return !std::isspace(ch);}).base(),s.end());
}

inline void trim(std::string &s)
{
  start_trim(s);
  end_trim(s);
}

static constexpr double IN_DBL_EPSILON = 1e-9;
inline bool double_eq(double l , double r){ return std::fabs(l - r) < IN_DBL_EPSILON;}
inline bool double_le(double l , double r){ return std::fabs(l - r) > IN_DBL_EPSILON && r > l;}   
inline bool double_gr(double l , double r){ return std::fabs(l - r) > IN_DBL_EPSILON && l > r ;}   


inline bool double_eq(double l , double r, double epsilon){ return std::fabs(l - r) < epsilon;}
inline bool double_le(double l , double r, double epsilon){ return std::fabs(l - r) > epsilon && r > l;}
inline bool double_gr(double l , double r, double epsilon){ return std::fabs(l - r) > epsilon && l > r ;}

static const std::string GEN("*");

template<typename T>
bool less_than(T&& T1, T&& T2)
{
  if (T1 == GEN && T2 == GEN)
    return false;
  if (T1 == GEN && T2!= GEN)
    return false;
  if (T1 != GEN && T2 == GEN)
    return false;
  
  return (T1 < T2);
}

template<typename T, typename... Args>
bool less_than(T&& T1, T&& T2, Args&&... args)
{
  if (less_than(T1,T2))
    return true;
  if (less_than(T2,T1))
    return false;

  return less_than(args...);
}

template<typename T>
bool equals(T&& T1, T&& T2)
{
  reutrn (T1 == T2) || (T1 == GEN ) || (T2 == GEN);
}

template<typename T, typename... Args>
bool equals(T&& T1, T&& T2, Args&&... args)
{
  if (!equals(T1,T2))
    return false;

  return equals(args...);
}

inline uint32_t  next_power_2( uint32_t  x)
{
  uint32_t value = 1 ;
  while(value <= x)
    {
      value = value << 1 ;
    }
  return value ;
}

template <typename T>
void insertion_sort(T *arr, size_t len)
{
  T key;
  int i, j;
  for (i = 1; i < len; i++)
  {
    key = arr[i];
    j = i - 1;

    // Move elements of arr[0..i-1],
    // that are greater than key,
    // to one position ahead of their
    // current position
    while (j >= 0 && arr[j] > key)
    {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

#define SC_PAGE_UP(x)   ({                                   \
      const __typeof__(x) ps = sysconf(_SC_PAGE_SIZE);       \
      ((((x) + ps - 1) / ps) * ps); })
#define SC_PAGE_DN(x)   ({                                   \
      const __typeof__(x) ps = sysconf(_SC_PAGE_SIZE);       \
      (((x) / ps) * ps); })

}} // namespace ends 
#endif
