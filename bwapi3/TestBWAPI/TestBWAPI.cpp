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

void testAction(Position pos)
{
  static int done = 0;
  if(done++ == 500)
  {
    pingMinimap(pos);
    done = 0;
  }
}

int _tmain(int argc, _TCHAR* argv[])
{
  try
  {
    const StaticGameData *gd;
    if(!connect())
    {
      printf("trying to connect...\n");
      while(!connect())
      {
        Sleep(1000);
      }
    }
    printf("connected\n");

    char buff[100];
    gd = getStaticGameData();

    CallTypeId call = waitForEvent();
    while(call != CallTypeIds::OnDisconnect)
    {
      int count = 0;
      for each(Util::Indexed<const KnownUnit&> kunit in gd->units)
      {
        if(kunit.item.type == UnitTypeIds::Terran_SCV)
        {
          testAction(kunit.item.position);
        }

        Position pos = kunit.item.position;
        sprintf(buff, "acc: %d", kunit.item.isAccelerating);
        drawText(pos, buff);

        count++;
      }
      itoa(gd->units.count, buff, 10);
      drawTextScreen(Position(10, 10), buff);

      const DynamicGameData *dd = getDynamicGameData();

      int size;
      size = dd->unitAddEvents.size();
      if(size)
      {
        sprintf(buff, "units added: %d", size);
        printText(buff);
      }

      size = dd->unitRemoveEvents.size();
      if(size)
      {
        sprintf(buff, "units removed: %d", size);
        printText(buff);
      }

      for each(BuildPosition startLocation in gd->startLocations)
      {
        drawCircle(startLocation * 32, 5, 61, false);
      }

      ShoutBox sb(Position(10, 18));
      sprintf(buff, "Force count: %d", gd->forces.count);
      drawTextScreen(sb.getNext(), buff);
      int forceId = 0;
      for each(const Force& force in gd->forces)
      {
        sprintf(buff, "force %d: %s", forceId, force.name.buffer);
        drawTextScreen(sb.getNext(), buff);
        for each(PlayerId player in gd->players)
        {
          const Player &playerData = gd->players[player];
          if(playerData.force != forceId)
            continue;
          sprintf(buff, "-player %d: %s", player, playerData.name.buffer);
          drawTextScreen(sb.getNext(), buff);
        }
        forceId++;
      }

      call = waitForEvent();
    }
  }
  catch(...)
  {
    printf("exception\n\n");
  }
  system("pause");
  return 0;
}

