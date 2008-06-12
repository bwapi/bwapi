#include "FileLogger.h"

namespace Util
{
  //------------------------------------------- CONSTRUCTOR --------------------------------------------------
  FileLogger::FileLogger(const std::string& fileName, Util::LogLevel::Enum logLevel)
  :Logger(logLevel)
  ,fileName(fileName + ".log")
  {
  }
  //---------------------------------------------- FLUSH -----------------------------------------------------
  bool FileLogger::flush(const char* data)
  {
    FILE *f = fopen(fileName.c_str(),"at");
    if (!f)
      return false;
    char time[9];
    _strtime(time);
     
    fprintf(f, "%s ", time);
    fprintf(f, "%s \n", data);
    fclose(f);
    return true;
  }
}