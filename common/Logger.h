#ifndef _IN_COMMON_LOG_H_
#define _IN_COMMON_LOG_H_
#include "log4cxx/logger.h"

#define LOG_T(msg) do {LOG4CXX_TRACE}{log4cxx::Logger::getRootLogger(), msg);} while(false)
#define LOG_D(msg) do {LOG4CXX_DEBUG}{log4cxx::Logger::getRootLogger(), msg);} while(false)
#define LOG_I(msg) do {LOG4CXX_INFO}{log4cxx::Logger::getRootLogger(), msg);} while(false)
#define LOG_W(msg) do {LOG4CXX_WARN}{log4cxx::Logger::getRootLogger(), msg);} while(false)
#define LOG_F(msg) do {LOG4CXX_FATAL}{log4cxx::Logger::getRootLogger(), msg);} while(false)

#endif
