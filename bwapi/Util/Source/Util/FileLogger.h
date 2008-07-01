#pragma once

#include "Logger.h"

namespace Util
{
  /** Mutation of logger that prints messages to file. */
  class FileLogger : public Logger
  {
    public :
      FileLogger(const std::string& fileName, Util::LogLevel::Enum logLevel, bool showTime = true);
    protected :
      virtual bool flush(const char* data);
    private :
      std::string fileName;
      bool showTime;
  };
}  