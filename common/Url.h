#ifndef _IN_URL_H_
#define _IN_URL_H_
#include <iostream>
#include <string>

namespace IN{
namespace COMMON{

enum class CONN_TYPE
  {
    TCP,
    MCP,
    UNIX,
    SHM,
    UDP,
    DEFAULT  
   };

class Url
{
 public:
  constexpr std::string URL_DELIMIT = "://";
  constexpr std::string PORT_DELIMIT = ":";
  constexpr std::string OPT_DELIMIT = ",";

  // String need to be end truncated for space
  // examples
  //  tcp://192.168.0.10:25001,rmax=1000
  //  mcp://233.24.10.1:19601
  // unix://tmp/mysocket,rbuf=1024
  // shm://agg_book,size=1024
  
  Url(const std::string& url);
  
  ~Url();

  CONN_TYPE get_type() const;

  std::string get_type_str() const;

  std::string get_addresss() const;

  std::string get_option(const std::string& def) const;

  friend std::ostream& operator << ( std::ostream& os, const Url& url)
  {
    switch(url.m_type)
      {
      case TCP:
	os << "tcp" ; break;
      case MCP:
	os << "mcp" ; break;
      case UNIX:
	os << "unix" ; break;
      case SHM:
	os << "shm" ; break;
      case UDP:
	os << "udp" ; break;
      case DEFAULT:
	os << "default" ; break;
      default:
	os << "default";
      }
    os << URL_DELIMIT << url.m_address;

    for (auto& opt: url.m_options)
      os << opt.first << "=" << opt.second << ",";
    return os;
  }
  
 private:  
  bool parse(const std::string& url);

  typedef std::map<std::string, std::string> Options;

  CONN_TYPE m_type;
  std::string m_address;
  Options m_options; 
};

}}
#endif
