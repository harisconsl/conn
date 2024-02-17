#ifndef _IN_COMMON_LOG_H_
#define _IN_COMMON_LOG_H_

#include "log4cxx/logger.h"

#define LOG_T(msg) LOG4CXX_TRACE(log4cxx::Logger::getRootLogger(), msg)
#define LOG_D(msg) LOG4CXX_DEBUG(log4cxx::Logger::getRootLogger(), msg)
#define LOG_I(msg) LOG4CXX_INFO(log4cxx::Logger::getRootLogger(), msg)
#define LOG_W(msg) LOG4CXX_WARN(log4cxx::Logger::getRootLogger(), msg)
#define LOG_E(msg) LOG4CXX_ERROR(log4cxx::Logger::getRootLogger(), msg)
#define LOG_F(msg) LOG4CXX_FATAL(log4cxx::Logger::getRootLogger(), msg)

#endif
