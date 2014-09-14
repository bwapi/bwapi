#include <Windows.h>
#include <string>

#include "config.h"
#include "common.h"

bool RunConfig()
{
  // Get the bwapi-data directory
  const std::string sBWAPIConfig{ GetBWAPIDataDirectory() + "/bwapi.ini" };

  // Open the config file
  if (reinterpret_cast<int>(ShellExecuteA(NULL, "open", sBWAPIConfig.c_str(), NULL, NULL, SW_SHOWNORMAL)) <= 32)
    return BWAPIError("Unable to open BWAPI config file.");

  return true;
}

