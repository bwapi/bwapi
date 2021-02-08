#include <BWAPI.h>

int BWAPI::BWAPI_getRevision()
{
  return 9000;
}
bool BWAPI::BWAPI_isDebug()
{
#ifdef _DEBUG
  return true;
#else
  return false;
#endif
}

