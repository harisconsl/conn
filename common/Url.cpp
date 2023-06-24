#include "Url.h"

Url::Url(const std::string& url)
{
  if (!parse(url))
    throw std::invalid_argument("Unable to parse " + url);
}

std::string get_option(const std::string& def) const
{
  auto iter = m_options.find(def);
  if ( iter != m_options.end())
    return iter->second;

  return "";
}

std::string get_address() const
{
  return m_address; 
}

CONN_TYPE get_type() const
{
  return m_type;
}

bool parse(const std::string& url)
{
  m_type = CONN_TYPE::DEFAULT;
  m_address = "";
  m_options.clear();

  int pos = url.find(URL_DELIMIT);
  
  if (pos == std::string::npos)
    return false;

  std::string type_str = url.substr(0, pos);
  
  if (type_str == "tcp")
    {
      m_type = CONN_TYPE::TCP;
    }
  else if (type_str == "mcp")
    {
      m_type = CONN_TYPE::MCP;      
    }
  else if (type_str == "unix")
    {
      m_type = CONN_TYPE::TCP;
    }
  else if ( type_str == "shm")
    {
      m_type = CONN_TYPE::SHM;
    }
  else
    {
      m_type = CONN_TYPE::DEFAULT;
      return false;
    }

  std::string add_substr = url.substr(pos + URL_DELIMIT.size());

  std::stringstream url_stream ;
  pos = std::string::npos;
  pos = add_substr.find(PORT_DELIMIT);
  if (pos == std::string::npos)
    {
      url_stream >> add_substr;
      if (url_stream.good())
	getline(url_stream, m_address, OPT_DELIMIT);
    }
  else
    {
      m_address = add_substr( 0, pos);
      url_stream >> add_substr( pos + PORT_DELIMIT.size()); 
    }

  bool port_identified = false;
  while( url_stream.good())
    {
      std::string option;
      std::string value;
      std::string op_val;
      getline(url_stream, op_val, ',');

      std::stringstream op_val_stream(op_val);

      getline(op_val_stream, option, '=');
      if (!op_val_stream.good() && !port_identified)
	{
	  m_options.insert(std::make_pair("port",option));
	  port_identified = true;
	}

      getline(op_val_stream, value, ',');

      if (option.length() && value.length())
	{
	  m_options.insert(std::make_pair(option, value));
	}
      else if (option.length() && !value.length())
	{
	  LOG_W("Invalid option : "<< option << " in URL : " << url << " .. ignoring!" );
	}
    }
  return (m_address.length() > 0);  
}


