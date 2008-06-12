#include "ScreenLogger.h"

#include "Globals.h"

namespace BWAPI
{
  //----------------------------------------- CONSTRUCTOR ----------------------------------------------------
  ScreenLogger::ScreenLogger(Util::LogLevel::Enum logLevel)
  :Logger(logLevel)
  {
  }
  //-------------------------------------------- FLUSH -------------------------------------------------------
  bool ScreenLogger::flush(const char* data)
  {
    BWAPI::Broodwar.print(data);
    return true;
  }
}