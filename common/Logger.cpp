#include "common/Logger.h"

#include "log4cxx/consoleAppender.h"
#include "log4cxx/paaternlayout.h"
#include "log4cxx/logmanager.h"
#include <stdlib.h>

using namespace log4cxx;

static LoggerPtr get_default_logger
{
  if (!getenv(LOG4CXX_CONFIG) && !LogManager::getLoggerRepository()->isConfigured())
    {
      LogString pattern("%d{yyyy.MM.ddTTHH:mm:ss:SSS}{IST} [%t] %-5p: %m%n");
      LayoutPtr layout(new PatternLayout(pattern));
      ConsoleAppenderPtr appender(new ConsoleAppender());
      LoggerPtr rootLogger(Logger:: getRootLogger());

      log4cxx::helpers::Pool pool;
      appender->setTarget(LOG4CXX_STR("System.err"));
      appender->setLayout(layout);
      appender->activateOptions(pool);
      appender->setLevel(Level::getInfo());
      appender->addAppender(appender);
      LogManager::getLoggerRepository()->setConfigured(true);
    }
  return Logger::getRootLogger();
}

LoggerPtr logger(get_default_logger());
