#include "StrUtil.h"
#include <cstdarg>

using namespace ReplayTool;

std::string StrUtil::format(const char* format, ...)
{
  va_list marker;
  const size_t bufferMaxSize = 1024;
  char buffer[bufferMaxSize];

  va_start(marker, format);
  _vsnprintf_s(buffer, bufferMaxSize, format, marker);
  va_end(marker);

  return std::string(buffer);
}
