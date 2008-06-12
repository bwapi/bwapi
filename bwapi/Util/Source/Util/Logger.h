#pragma once

#include <stdio.h>
#include <ctime>
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
       * Logs the specified message to the current log.
       * @param message Message to be written to the log file, note that the
       *        result will be "(hh:MM:ss) <message>\n"
       * @param logLevel of detail of this message.
       */
      bool log(const std::string& message, LogLevel::Enum logLevel = LogLevel::Normal);
      /**
       * Logs the specified message using the printf notation to display the
       * parameter. Example: log("5+5=%d",5+5) will act as log("5+5=10")
       * @param message Message of the log, can include printf parameter
       *        specifiers.
       * @param parameter1 parametr for the printf parameter notation.
       * @param logLevel of detail of this message.
       */
      template <class T1>
      bool log(const std::string& message, const T1& parameter1, LogLevel::Enum logLevel = LogLevel::Normal);
      /**
       * Logs the specified message using the printf notation to display
       * parameters. Example: log("5+%d=%d",5,5+5) will act as log("5+5=10")
       * @param message Message of the log, can include printf parameter
       *        specifiers.
       * @param parameter1 first parametr for the printf parameter notation.
       * @param parameter2 second parameter for the printf parameter notation.
       * @param logLevel of detail of this message.
       */
      template <class T1, class T2>
      bool log(const std::string& message, const T1& parameter1, const T2& parameter2, LogLevel::Enum logLevel = LogLevel::Normal);
      template <class T1, class T2, class T3>
      bool log(const std::string& message, const T1& parameter1, const T2& parameter2, const T3& parameter3, LogLevel::Enum logLevel = LogLevel::Normal);
      template <class T1, class T2, class T3, class T4>
      bool log(const std::string& message, const T1& parameter1, const T2& parameter2, const T3& parameter3, const T4& parameter4, LogLevel::Enum logLevel = LogLevel::Normal);
      void setFileName(const std::string& name);
      void registerLogger(Logger* logger);
    private :
      std::string name;
      LogLevel::Enum levelToLog;
      static const unsigned int BUFFER_SIZE = 2048;
      static char buffer[BUFFER_SIZE];
      std::list<Logger*> connectedLoggers;
    public : 
      /** Every log message will be also posted to this global log. */
      static Logger* globalLog;
      static bool deleteLogsAtStart;
    protected :
      virtual bool flush(const char* data) = 0;
      bool flushInternal(const char* data);
  };

  // Template member functions must be defined in header
  //------------------------------- LOG -----------------------------------------
  template <class T1>
  bool Logger::log(const std::string&   message, 
                   const T1&            parameter1, 
                         LogLevel::Enum logLevel)
  {
    if (levelToLog > this->levelToLog)
      return true;

    _snprintf(buffer, BUFFER_SIZE, message.c_str(), parameter1);
    this->flushInternal(buffer);

    if (globalLog != NULL &&
        this != globalLog)
      globalLog->log(message, parameter1, logLevel);
    return true;
  }
  //------------------------------- LOG -----------------------------------------
  template <class T1, class T2>
  bool Logger::log(const std::string&   message, 
                   const T1&            parameter1, 
                   const T2&            parameter2, 
                         LogLevel::Enum logLevel)
  {
    if (levelToLog > this->levelToLog)
      return true;
    
    _snprintf(buffer, BUFFER_SIZE, message.c_str(), parameter1, parameter2);
    this->flushInternal(buffer);
    
    if (globalLog != NULL &&
        this != globalLog)
      globalLog->log(message, parameter1, parameter2, logLevel);
    return true;
  }
  //------------------------------- LOG -----------------------------------------
  template <class T1, class T2, class T3>
  bool Logger::log(const std::string&   message, 
                   const T1&            parameter1, 
                   const T2&            parameter2, 
                   const T3&            parameter3, 
                         LogLevel::Enum logLevel)
  {
    if (levelToLog > this->levelToLog)
      return true;
    
    _snprintf(buffer, BUFFER_SIZE, message.c_str(), parameter1, parameter2, parameter3);
    this->flushInternal(buffer);
    
    if (globalLog != NULL &&
        this != globalLog)
      globalLog->log(message, parameter1, parameter2, parameter3, logLevel);
    return true;
  }
  //------------------------------- LOG -----------------------------------------
  template <class T1, class T2, class T3, class T4>
  bool Logger::log(const std::string&   message, 
                   const T1&            parameter1, 
                   const T2&            parameter2, 
                   const T3&            parameter3, 
                   const T4&            parameter4, 
                         LogLevel::Enum logLevel)
  {
    if (levelToLog > this->levelToLog)
      return true;

    _snprintf(buffer, BUFFER_SIZE, message.c_str(), parameter1, parameter2, parameter3, parameter4);
    this->flushInternal(buffer);

    if (globalLog != NULL &&
        this != globalLog)
      globalLog->log(message, parameter1, parameter2, parameter3, parameter4, logLevel);
    return true;
  }  
};