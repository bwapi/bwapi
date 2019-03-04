#include <BWAPI.h>
#include "../svnrev.h"

int BWAPI::BWAPI_getRevision()
{
  return SVN_REV;
}
bool BWAPI::BWAPI_isDebug()
{
  return BUILD_DEBUG == 1;
}

