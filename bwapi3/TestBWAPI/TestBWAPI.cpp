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
char* clearanceNames[] = {"None", "Noticed", "Visible", "Full"};

void testAction(UnitId unit)
{
  static int done = 0;
  if(done++ == 100)
  {
    const StaticGameData *gd = getStaticGameData();
    for each(Util::Indexed<const KnownUnit&> kunit in gd->units)
    {
      if(kunit.item.type == UnitTypeIds::Zerg_Zergling)
      {
        orderAttackUnit(unit, kunit.index);
        drawCircleMap(kunit.item.position, 5, 61, false);
      }
    }
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

    CallTypeId call;

    call = waitForEvent();

    // on start code here
    enableFlag(Flags::UserInput);

    while(call != CallTypeIds::OnDisconnect)
    {
      int count = 0;
      for each(UnitId unitId in gd->selectedUnits)
      {
        const UnitState& unit = gd->units[unitId];

        Position pos = unit.position;
        sprintf(buff, "action: %d", unit.action.type);
        drawTextMap(pos, buff);

        drawLineMap(unit.action.targetPosition, unit.position, 61);
        drawLineMap(unit.movePosition, unit.position, 128);

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

      ShoutBox sb(Position(10, 18));

      for each(const Player& player in gd->players)
      {
        sprintf(buff, "player \"%s\": %d minerals, %d gas", player.name.buffer, player.minerals, player.gas);
        drawTextScreen(sb.getNext(), buff);
      }

      if(!gd->userInput.isEmpty())
      {
        sendText(gd->userInput.c_str());
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

