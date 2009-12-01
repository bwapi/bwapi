#include "ScreenLogger.h"

#include "Engine.h"

namespace BWAPI
{
  bool ScreenLogger::shut = false;
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  ScreenLogger::ScreenLogger(Util::LogLevel::Enum logLevel)
      : Logger(logLevel)
  {
  }
  //------------------------------------------------- FLUSH --------------------------------------------------
  bool ScreenLogger::flush(const char* data)
  {
    if (!this->shut)
      BWAPI::Engine::printf(data);
    return true;
  }
};
