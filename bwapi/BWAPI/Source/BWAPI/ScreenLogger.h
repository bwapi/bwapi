#pragma once
#include <string>

#include <Util/Logger.h>

namespace BWAPI
{
  /** Logger mutation that writes all messges directly to bw screen - works only ingame. */
  class ScreenLogger : public Util::Logger
  {
    public :
      ScreenLogger(Util::LogLevel::Enum logLevel = Util::LogLevel::Normal);
      static bool shut;
    protected :
      virtual bool flush(const char* data);
  };
}  