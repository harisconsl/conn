#ifndef _IN_COMMON_TIMEPOINT_H_
#define _IN_COMMON_TIMEPOINT_H_

#include <chrono>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Logger.h"

namespace IN {
namespace COMMON {

using sc = std::chrono;
boost::posix_time::ptime const ptime_epoch(boost::gregorian::date(1970,1,1));

  
class Timepoint
{
 public:
  using clock = sc::system_clock;
  using time_point = sc::time_point<clock>;
  using duration = sc::duration<clock::rep, std::nana>;

 Timepoint(uint64_t ns)
   : m_nano(ns)
    { }
   
 Timepoint(const time_point& tp)
   : m_nano(sc::duration_cast<duration>(tp.time_since_epoch()).count())
    { }

 Timepoint(const boost::posix_time::ptime& t)
   : m_nano((t - ptime_epoch).total_nanoseconds())
    { }

 Timepoint(const timespec& ts):
  m_nano(uint64_t(ts.tv_sec)*1000000000UL + uint64_t(ts.tv_nsec))
    { }

  static Timepoint now()
  {
    return Timepoint(sc::duration_cast<duration>
		     (clock::now().time_since_epoch()).count());
  }

  uint64_t total_nanoseconds()
  {
    return m_nano;
  }

  uint64_t total_microseconds()
  {
    return m_nano/1000;
  }

  uint64_t total_milliseconds()
  {
    return m_nano/1000000;
  }

  uint64_t total_seconds()
  {
    return m_nano/1000000000;
  }

  uint64_t total_minutes()
  {
    return m_nano/60000000000;
  }

  uint32_t millisecond_component() const
  {
    return (m_nano/ 1000000) % 1000;
  }
  
  uint32_t microsecond_component() const
  {
    return (m_nano / 1000) % 1000;
  }

  uint32_t nanosecond_component() const
  {
    return m_nano % 1000;
  }
  
  timepoint chrono_time_point() const
  {
    return timepoint(sc::nanoseconds(m_nano));
  }

  Timepoint& operator = (uint64_t ns)
  {
    m_nano = ns;
    return *this;
  }

  Timepoint& operator = (const Timepoint& tp)
  {
    m_nano = tp.m_nano;
    return *this;
  }

  Timepoint& operator == (const Timepoint& tp)
  {
    return m_nano == tp.m_nano;
  }

  Timepoint& operator < (const Timepoint& tp)
  {
    return m_nano < tp.m_nano;
  }

  Timepoint& operator <= (const Timepoint& tp)
  {
    return m_nano <= tp.m_nano;
  }

  Timepoint& operator > (const Timepoint& tp)
  {
    return m_nano > tp.m_nano;
  }

  Timepoint& operator >= (const Timepoint& tp)
  {
    return m_nano >= tp.m_nano;
  }

  boost::posix_time::ptime to_posix_time()
  {
    return ptime_epoch + boost::posix_time::microseconds(total_microseconds());
  }

  friend std::ostream& operator << (std::ostream& os, const Timepoint& tp)
  {
    std::tm t;
    time_t time = tp.total_seconds();
    gmtime_r(&time, &t);
    
    uint32_t nano_sec = tp.nanoseconds();
    uint32_t micro_sec = nano_sec/1000; nano_sec -=micro_sec*1000;
    uint32_t milli_sec = micro_sec/1000; micro_sec -=milli_sec*1000;

    os << std::setw(4) << std::setfill('0') << (t.tm_year + 1900) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_mon + 1) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_mday) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_hour) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_min) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_sec) << '_'
       << std::setw(3) << std::setfill('0') << milli_sec
       << std::setw(3) << std::setfill('0') << micro_sec
       << std::setw(3) << std::setfill('0') << nano_sec;
    
    return os;
  }

  std::string to_FIX_time(bool local_time = false) const
  {
    std::tm t;
    time_t time = total_seconds();
    
    if (local_time)
      localtime_r(&time,&t);
    else
      gmtime_r(&time, &t);

    uint32_t nano_sec = tp.nanoseconds();
    uint32_t micro_sec = nano_sec/1000; nano_sec -=micro_sec*1000;
    uint32_t milli_sec = micro_sec/1000; micro_sec -=milli_sec*1000;

    std::stringstream os;
    os << std::setw(4) << std::setfill('0') << (t.tm_year + 1900) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_mon + 1) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_mday) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_hour) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_min) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_sec) << '_'
       << std::setw(3) << std::setfill('0') << milli_sec
       << std::setw(3) << std::setfill('0') << micro_sec;

    return os.str();    
  }

  std::string to_FIX_time_micro(bool local_time = false) const
  {
    std::tm t;
    time_t time = total_seconds();
    
    if (local_time)
      localtime_r(&time,&t);
    else
      gmtime_r(&time, &t);

    std::stringstream os;
    os << std::setw(4) << std::setfill('0') << (t.tm_year + 1900) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_mon + 1) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_mday) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_hour) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_min) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_sec) << '_'
       << std::setw(3) << std::setfill('0') << milli_seconds();

    return os.str();    
  }

  std::string to_FIX_date(bool local_time = false) const
  {
    std::tm t;
    time_t time = total_seconds();
    
    if (local_time)
      localtime_r(&time,&t);
    else
      gmtime_r(&time, &t);

    std::stringstream os;
    os << std::setw(4) << std::setfill('0') << (t.tm_year + 1900) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_mon + 1) << '_'
       << std::setw(2) << std::setfill('0') << (t.tm_mday) ;

    return os.str();    
  }

  void from_FIX_time(std::istream& is)
  {
    std::tm t;
    uint32_t milli_sec = 0;

    char ret;
    is >> t.tm_year >> ret >> t.tm_mon >> ret >> t.tm_mday >> ret
       >> t.tm_hour >> ret >> t.tm_min >> ret >> t.tm_sec >> ret >> milli;
    
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    time_t time = timegm(&t);
    m_nano = time * 1000000000LL + micro*1000000LL;
  }

  void from_FIX_time_micro(std::istream& is)
  {
    std::tm t;
    uint32_t milli_sec = 0;

    char ret;
    is >> t.tm_year >> ret >> t.tm_mon >> ret >> t.tm_mday >> ret
       >> t.tm_hour >> ret >> t.tm_min >> ret >> t.tm_sec >> ret >> micro;
    
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    time_t time = timegm(&t);
    m_nano = time * 1000000000LL + micro*1000LL;
  }

  void from_FIX_date(const std::string& date)
  {
    std::tm t;
    t.tm_year = std::stoi(date.substr(0,4)) - 1900;
    t.tm_mon = std::stoi(date.substr(4,2)) - 1;
    t.tm_mday = std::stoi(date.substr(6,2));

    return Timepoint(1000000000LL*timegm(&t));
  }
  
 private:
  uint64_t m_nano;
};

}}
#endif
