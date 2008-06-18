#include "FileLogger.h"

namespace Util
{
  //------------------------------------------- CONSTRUCTOR --------------------------------------------------
  FileLogger::FileLogger(const std::string& fileName, Util::LogLevel::Enum logLevel, bool showTime)
  :Logger(logLevel)
  ,fileName(fileName + ".log")
  ,showTime(showTime)
  {
  }
  //---------------------------------------------- FLUSH -----------------------------------------------------
  bool FileLogger::flush(const char* data)
  {
    FILE *f = fopen(fileName.c_str(),"at");
    if (!f)
      return false;
    if (showTime)
    {
      char time[9];
      _strtime(time);
      fprintf(f, "%s ", time);
    }
    fprintf(f, "%s \n", data);
    fclose(f);
    return true;
  }
}