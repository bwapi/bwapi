#include <stdio.h>
#include <tchar.h>

#include <BWAPI\all.h>

#include "windows.h"

void BWAPI_CALL onUnitAdd(BWAPI::UnitState *)
{
}

void BWAPI_CALL onUnitRemove(BWAPI::UnitState *)
{
}

void BWAPI_CALL onSendText(const char* text)
{
}

void BWAPI_CALL onMatchStart(bool fromBeginning)
{
}

void BWAPI_CALL onMatchFrame()
{
  BWDrawText(10, 10, "AI drawn text");
  printf("onframe\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
  if (!BWConnect())
  {
    printf("waiting for server to start up\n");
    while(!BWConnect())
    {
      Sleep(200);
    }
  }
  printf("connected\n");
  if(!BWTakeover(onMatchFrame, onMatchStart, onUnitAdd, onUnitRemove, onSendText))
  {
    printf(BWGetLastError());
  }
  system("pause");
  return 0;
}

