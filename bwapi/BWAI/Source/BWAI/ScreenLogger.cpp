#include "ScreenLogger.h"

#include "Globals.h"
#include <BWAPI.h>

namespace BWAI
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
      BWAPI::Broodwar->printPublic(data);
    return true;
  }
}