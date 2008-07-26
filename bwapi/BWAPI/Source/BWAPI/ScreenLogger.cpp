#include "ScreenLogger.h"

#include "Globals.h"

namespace BWAPI
{
  bool ScreenLogger::shut = false;
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  ScreenLogger::ScreenLogger(Util::LogLevel::Enum logLevel)
  :Logger(logLevel)
  {
  }
  //------------------------------------------------- FLUSH --------------------------------------------------
  bool ScreenLogger::flush(const char* data)
  {
    if (!this->shut)
      BWAPI::Broodwar.print(data);
    return true;
  }
}