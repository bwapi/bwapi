#include <stdio.h>
#include <tchar.h>

#include <windows.h>

#include <BWAPI\all.h>

#include <string>

using namespace BWAPI;

class ShoutBox
{
private:
  Position pos;
public:
  ShoutBox(Position startpos)
  {
    pos = startpos;
  }
  Position getNext()
  {
    Position retval = pos;
    pos += Position(0, 8);
    return retval;
  }
};

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
        sprintf(buff, "acc: %d", kunit.item.isAccelerating);
        DrawText(pos, buff);

        count++;
      }
      itoa(gd->units.count, buff, 10);
      DrawTextScreen(Position(10, 10), buff);

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

      for each(BuildPosition startLocation in gd->startLocations)
      {
        DrawCircle(startLocation * 32, 5, 61, false);
      }

      ShoutBox sb(Position(10, 18));
      sprintf(buff, "%d", gd->forces.count);
      DrawTextScreen(sb.getNext(), buff);
      int i = 0;
      for each(const Force& force in gd->forces)
      {
        sprintf(buff, "force %d: %s", i++, force.name.buffer);
        DrawTextScreen(sb.getNext(), buff);
        for each(PlayerId player in force.players)
        {
          const Player &playerData = gd->players[player];
          sprintf(buff, "-player %d: %s", player, playerData.name.buffer);
          DrawTextScreen(sb.getNext(), buff);
        }
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

