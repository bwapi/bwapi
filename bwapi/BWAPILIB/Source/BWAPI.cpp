#include <BWAPI.h>
#include "../../svnrev.h"

#include "../../Debug.h"

int BWAPI::BWAPI_getRevision()
{
  return 4708;
}
bool BWAPI::BWAPI_isDebug()
{
  return BUILD_DEBUG == 1;
}

