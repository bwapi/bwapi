#include <Windows.h>
#include <string>

#include "config.h"
#include "common.h"

bool RunConfig()
{
  // Get the bwapi-data directory
  const std::string sBWAPIConfig{ GetBWAPIDataDirectory() + "/bwapi.ini" };

  // Open the config file
  std::system( ("start \"\" \"" + sBWAPIConfig + "\"").c_str() );
  return true;
}

