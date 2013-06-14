#include "Logger.h"

using namespace ReplayTool;

Logger::Logger(const char *filename)
  : file(filename == nullptr ? nullptr : fopen(filename, "a"))
{
}

Logger::~Logger()
{
  if (file != nullptr)
  {
    fclose(file);
    file = nullptr;
  }
}

void Logger::writeLine(const char *pszFormat, ...)
{
  if (file != nullptr)
  {
    va_list vl;
    va_start(vl,pszFormat);
    vfprintf_s(file, pszFormat, vl);
    fprintf_s(file, "\n");
    va_end(vl);
  }
}