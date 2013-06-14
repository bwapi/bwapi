#include <Windows.h>
#include <string>

#include "config.h"
#include "common.h"

bool RunConfig()
{
  // Get the bwapi-data directory
  std::string sBWAPIDir(GetBWAPIDataDirectory());

  // Open the config file
  if ( ShellExecute(NULL, "open", (sBWAPIDir + "\\bwapi.ini").c_str(), NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32 )
    return BWAPIError("Unable to open BWAPI config file.");

  return true;
}

