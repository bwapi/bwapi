#include <stdio.h>
#include <tchar.h>

#include <windows.h>

#include <BWAPI\all.h>

#include <string>

using namespace BWAPI;

int _tmain(int argc, _TCHAR* argv[])
{
  try
  {
    const StaticGameData *gd;
    if(!Connect())
    {
      printf("trying to connect...\n");
      while(!Connect())
      {
        Sleep(1000);
      }
    }
    printf("connected\n");

    char buff[100];
    gd = GetStaticGameData();

    CallTypeId call = WaitForEvent();
    while(call != CallTypeIds::OnDisconnect)
    {
      int count = 0;
      for each(Util::Indexed<const KnownUnit&> kunit in gd->units)
      {
        if(kunit.item.type == UnitTypeIds::Terran_SCV)
          OrderStop(kunit.index);


        Position pos = kunit.item.position;
        pos -= gd->screenPosition;
        sprintf(buff, "HP: %d", kunit.item.hitPoints);
        DrawText(pos, buff);

        count++;
      }
      itoa(count, buff, 10);
      DrawText(Position(10, 10), buff);

      const DynamicGameData *dd = GetDynamicGameData();

      int size;
      size = dd->unitAddEvents.size();
      if(size)
      {
        sprintf(buff, "units added: %d", size);
        PrintText(buff);
      }

      size = dd->unitRemoveEvents.size();
      if(size)
      {
        sprintf(buff, "units removed: %d", size);
        PrintText(buff);
      }
      call = WaitForEvent();
    }
  }
  catch(...)
  {
    printf("exception\n\n");
  }
  system("pause");
  return 0;
}

