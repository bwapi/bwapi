#include "Logger.h"

#include <stdio.h>

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
  bool Logger::log(const    std::string&   message, 
                            LogLevel::Enum logLevel)
  {
    if (levelToLog > this->levelToLog)
      return true;

    this->flushInternal(message.c_str());

    if (globalLog != NULL &&
        this != globalLog)
      globalLog->log(message, levelToLog);
    return true;
  }
  //------------------------------- SET FILE NAME -------------------------------
  void Logger::setFileName(const std::string& name)
  {
    this->name = name;
  }
  //-------------------------------- REGISTER LOGGER ----------------------------
  void Logger::registerLogger(Logger* logger)
  {
    this->connectedLoggers.push_back(logger);
  }
  //-----------------------------------------------------------------------------
  bool Logger::flushInternal(const char* buffer)
  {
    for (std::list<Logger*>::iterator i = this->connectedLoggers.begin();
         i != this->connectedLoggers.end();
         ++i)
     (*i)->flush(buffer);   
    return this->flush(buffer);     
  }
  //-----------------------------------------------------------------------------
  
}
