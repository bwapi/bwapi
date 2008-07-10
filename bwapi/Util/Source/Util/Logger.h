#pragma once

#include <stdio.h>
#include <string>
#include <list>

#include "LogLevel.h"

namespace Util
{
  /** 
  * Utility for loggin debug output.
  * Every kind of log should have it's own instance of this class, don't log different things to same log.
  * Different log instances can be later used to combine different logs with different detailLevels.
  */
  class Logger
  {
    public :
      /**
       * Creates new logger.
       * @param name "<name>.log" is the name of the log file
       * @param levelToLog All log inputs with less importancy will be not
       *        logged in this log
       */
      Logger(LogLevel::Enum levelToLog);
      virtual ~Logger();
      /**
       * Logs the message using printf formatting style.
       * This function use Normal Log level.
       * @param message message to be logged.
       * @param other Parameters of the printf style format.
       */
      bool log         (const char* message, ...);
      bool logDetailed (const char* message, ...);
      bool logCommon   (const char* message, ...);
      bool logImportant(const char* message, ...);
      bool logCritical (const char* message, ...);      
      void registerLogger(Logger* logger);
      /** Every log message will be also posted to this global log. */
      static Logger* globalLog;
      static bool deleteLogsAtStart;
    protected :
      virtual bool flush(const char* data) = 0;
      bool flushInternal(const char* data);      
    private :
      bool logInternal(const char* message, LogLevel::Enum, va_list ap);
      LogLevel::Enum levelToLog;
      static const unsigned int BUFFER_SIZE = 2048;
      static char buffer[BUFFER_SIZE];
      std::list<Logger*> connectedLoggers;
  };
};