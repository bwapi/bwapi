#include <stdio.h>
#include <tchar.h>

#include <BWAPI\all.h>

#include "windows.h"

BWAPI::StaticGameData *data;

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
  printf("on start match\n");
  data = BWGetStaticGameData();
}

void BWAPI_CALL onMatchFrame()
{
  static char buffer[100];
  const int green = 117;

  int count = 0;
  HANDLE h = BWAllUnitsBegin();
  for(;;)
  {
    int unitId = BWAllUnitsNext(h);
    if(unitId == -1)
      break;
    BWAPI::UnitState *unit = BWGetUnit(unitId);
    BWAPI::Position pos(unit->position);
    BWPositionMapToScreen(&pos);
    BWDrawCircle(pos.x, pos.y, 10, green, false);
    count++;
  }
  BWAllUnitsClose(h);
  sprintf(buffer, "unit count: %d", count);
  BWDrawText(10, 10, buffer);

  count = 0;
  h = BWUnitAddEventsBegin();
  for(;;)
  {
    BWAPI::UnitAddEvent *e = BWUnitAddEventsNext(h);
    if(!e)
      break;
    count++;
  }
  BWUnitAddEventsClose(h);
  if(count)
  {
    sprintf(buffer, "%d units added\n", count);
    BWPrintText(buffer);
  }

  count = 0;
  h = BWUnitRemoveEventsBegin();
  for(;;)
  {
    BWAPI::UnitRemoveEvent *e = BWUnitRemoveEventsNext(h);
    if(!e)
      break;
    count++;
  }
  BWUnitRemoveEventsClose(h);
  if(count)
  {
    sprintf(buffer, "%d units removed\n", count);
    BWPrintText(buffer);
  }

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
    printf("%s\n", BWGetLastError());
  }
  system("pause");
  return 0;
}

