#include "Logger.h"

#include <stdio.h>
#include <stdarg.h>

#include "FileLogger.h"

namespace Util
{
  Logger* Logger::globalLog = new FileLogger("global", LogLevel::MicroDetailed);
  char Logger::buffer[BUFFER_SIZE];
  //---------------------------- CONSTRUCTOR ------------------------------------
  Logger::Logger(LogLevel::Enum levelToLog)
  :levelToLog(levelToLog)
  {
  }
  //---------------------------- CONSTRUCTOR ------------------------------------
  Logger::~Logger()
  {
    for (std::list<Logger*>::iterator i = this->connectedLoggers.begin();
         i != this->connectedLoggers.end();
         ++i)
      delete *i;
  }
  //------------------------------- LOG -----------------------------------------
  bool Logger::log(const char* message, ...)
  {
    va_list ap;
    va_start(ap, message);
    logInternal(message, LogLevel::Normal, ap);
    va_end(ap);
    return true;
  }
  //------------------------------- LOG -----------------------------------------
  bool Logger::logDetailed(const char* message, ...)
  {
    va_list ap;
    va_start(ap, message);
    logInternal(message, LogLevel::Detailed, ap);
    va_end(ap);
    return true;
  }
  //------------------------------- LOG -----------------------------------------
  bool Logger::logCommon(const char* message, ...)
  {
    va_list ap;
    va_start(ap, message);
    logInternal(message, LogLevel::Commmon, ap);
    va_end(ap);
    return true;
  }
  //------------------------------- LOG -----------------------------------------
  bool Logger::logImportant(const char* message, ...)
  {
    va_list ap;
    va_start(ap, message);
    logInternal(message, LogLevel::Important, ap);
    va_end(ap);
    return true;
  }
  //------------------------------- LOG -----------------------------------------
  bool Logger::logCritical(const char* message, ...)
  {
    va_list ap;
    va_start(ap, message);
    logInternal(message, LogLevel::Critical, ap);
    va_end(ap);
    return true;
  }  
  //------------------------------- LOG -----------------------------------------
  bool Logger::logInternal(const char* message, LogLevel::Enum logLevel, va_list ap)
  {
    if (logLevel > this->levelToLog)
      return true;

    vsnprintf(buffer, BUFFER_SIZE, message, ap); 
    this->flushInternal(buffer);
    
    if (globalLog != NULL &&
        this != globalLog)
      globalLog->logInternal(message, logLevel, ap);
    return true;
  }
  //-------------------------------- REGISTER LOGGER ----------------------------
  void Logger::registerLogger(Logger* logger)
  {
    this->connectedLoggers.push_back(logger);
  }
  //-----------------------------------------------------------------------------
  bool Logger::flushInternal(const char* buffer)
  {
    for each (Logger* i in this->connectedLoggers)
     i->flush(buffer);
    return this->flush(buffer);
  }
  //-----------------------------------------------------------------------------
  
}
