#pragma once
#include <stdio.h>
#include <ctime>
#include <string>
/** 
 * Utility for loggin debug output.
 * Every kind of log should have it's own instance of this class, don't log different things to same log.
 * Different log instances can be later used to combine different logs with different detailLevels.
 */
class Logger
{
  public :
    /** The level of detail of the log. */
    enum LogLevel
    {
      Critical = 0,
      Important = 1,
      Normal = 2,
      Commmon = 3,
      Detailed = 4,
      MicroDetailed = 5
    };
    /** 
     * Creates new logger
     * @param name "<name>.log" is the name of the log file
     * @param levelToLog All log inputs with less importancy will be not logged in this log
     */
    Logger::Logger(const std::string& name, LogLevel levelToLog);
    /** 
     * Logs the specified message to the current log.
     * @param message Message to be written to the log file, note that the result will be "(hh:MM:ss) <message>\n"
     * @param level of detail of this message.
     */
    bool log(const std::string& message, LogLevel level = Normal);
    /** 
     * Logs the specified message using the printf notation to display the parameter.
     * Example: log("5+5=%d",5+5) will act as log("5+5=10")
     * @param message Message of the log, can include printf parameter specifiers.
     * @param parameter1 parametr for the printf parameter notation.
     * @param level of detail of this message.
     */    
    template <class T1>
    bool log(const std::string& message, const T1& parameter1, LogLevel level = Normal);
    /** 
     * Logs the specified message using the printf notation to display parameters.
     * Example: log("5+%d=%d",5,5+5) will act as log("5+5=10")
     * @param message Message of the log, can include printf parameter specifiers.
     * @param parameter1 first parametr for the printf parameter notation.
     * @param parameter2 second parametr for the printf parameter notation.
     * @param level of detail of this message.
     */    
    template <class T1, class T2>
    bool log(const std::string& message, const T1& param1, const T2& param2, LogLevel level = Normal);
  private :
    std::string name;
    LogLevel levelToLog;
  public : 
    /** Every log message will be also posted to this global log. */
    static Logger globalLog;
};

// Template member functions must be defined in header
//------------------------------- LOG -----------------------------------------
template <class T1>
bool Logger::log(const std::string& message, 
                 const T1&          parameter1, 
                       LogLevel     level)
{
  char time[9];
  _strtime(time);
  FILE *f = fopen(name.c_str(),"at");
  if (!f)
    return false;
  fprintf(f, "%s ", time);
  fprintf(f, message.c_str(), parameter1);
  fprintf(f, "\n");
  fclose(f);
  if (this != &globalLog)
    globalLog.log(message, parameter1, level);
  return true;
}
//------------------------------- LOG -----------------------------------------
template <class T1, class T2>
bool Logger::log(const std::string& message, 
                 const T1&          parameter1, 
                 const T2&          parameter2, 
                       LogLevel     level)
{
  char time[9];
  _strtime(time);
  FILE *f = fopen(name.c_str(),"at");
  if (!f)
    return false;
  fprintf(f, "%s ", time);
  fprintf(f, message.c_str(), parameter1, parameter2);
  fprintf(f, "\n");
  fclose(f);
  if (this != &globalLog)
    globalLog.log(message, parameter1, parameter2, level);
  return true;
}