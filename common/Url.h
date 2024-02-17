#ifndef _IN_COMMON_URL_H_
#define _IN_COMMON_URL_H_

#include <iostream>
#include <string>
#include <map>

namespace IN{
namespace COMMON{

enum class CONN_TYPE
{
    TCP,
    MCP,
    UNIX,
    SHM,
    UDP,
    CHRONICLE,
    DEFAULT  
};

class Url
{
 public:
  std::string URL_DELIMIT = "://";
  std::string PORT_DELIMIT = ":";
  char OPT_DELIMIT = ',';

  // String need to be end truncated for space
  // examples :
  //  tcp://192.168.0.10:25001,rmax=1000
  //  mcp://233.24.10.1:19601
  //  unix://tmp/mysocket,rbuf=1024
  //  shm://agg_book,size=1024
  //  chr://tmp/chronicle-file,
  
  Url(const std::string& url);
  
  ~Url() { }

  CONN_TYPE get_type() const;

  std::string get_type_str() const;

  std::string get_address() const;

  std::string get_option(const std::string& def) const;

  friend std::ostream& operator << ( std::ostream& os, const Url& url)
  {
    switch(url.m_type)
      {
      case CONN_TYPE::TCP:
	os << "tcp" ; break;
      case CONN_TYPE::MCP:
	os << "mcp" ; break;
      case CONN_TYPE::UNIX:
	os << "unix" ; break;
      case CONN_TYPE::SHM:
	os << "shm" ; break;
      case CONN_TYPE::UDP:
	os << "udp" ; break;
      case CONN_TYPE::CHRONICLE:
	os << "chr" ; break;
      case CONN_TYPE::DEFAULT:
	os << "default" ; break;
      default:
	os << "default";
      }

    os << url.URL_DELIMIT << url.m_address;
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
