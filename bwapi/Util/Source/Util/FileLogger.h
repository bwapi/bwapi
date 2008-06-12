#pragma once

#include "Logger.h"

namespace Util
{
  class FileLogger : public Logger
  {
    public :
      FileLogger(const std::string& fileName, Util::LogLevel::Enum logLevel);
    protected :
      virtual bool flush(const char* data);
    private :
      std::string fileName;
  };
}  