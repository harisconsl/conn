#include "Logger.h"

#include "log4cxx/consoleappender.h"
#include "log4cxx/patternlayout.h"
#include "log4cxx/logmanager.h"
#include <stdlib.h>

using namespace log4cxx;

static LoggerPtr get_default_logger()
{
  if (!std::getenv("LOG4CXX_CONFIG") && !LogManager::getLoggerRepository()->isConfigured())
    {
      LogString pattern("%d{yyyy.MM.ddTTHH:mm:ss:SSS}{IST} [%t] %-5p: %m%n");
      LayoutPtr layout(new PatternLayout(pattern));
      ConsoleAppenderPtr appender(new ConsoleAppender());
      LoggerPtr rootLogger(Logger:: getRootLogger());

      log4cxx::helpers::Pool pool;
      appender->setTarget(LOG4CXX_STR("System.err"));
      appender->setLayout(layout);
      appender->activateOptions(pool);
      rootLogger->setLevel(Level::getInfo());
      rootLogger->addAppender(appender);
      LogManager::getLoggerRepository()->setConfigured(true);
    }
  return Logger::getRootLogger();
}

LoggerPtr logger(get_default_logger());
