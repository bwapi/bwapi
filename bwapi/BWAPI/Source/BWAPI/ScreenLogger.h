#include <string>

#include <Util/Logger.h>

namespace BWAPI
{
  class ScreenLogger : public Util::Logger
  {
    public :
      ScreenLogger(Util::LogLevel::Enum logLevel);
      static bool shut;
    protected :
      virtual bool flush(const char* data);
  };
}  