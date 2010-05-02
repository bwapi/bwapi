#include <stdio.h>

#include <BWAPIClient\Source\BWAPIClient.h>
#include <BWAPI.h>

#include <windows.h>

#include <string>

using namespace BWAPI;

void reconnect()
{
  while(!BWAPIClient.connect())
  {
    Sleep(1000);
  }
}
int main(int argc, const char* argv[])
{
  BWAPI::BWAPI_init();
  printf("Connecting...");
  reconnect();

  while(true)
  {
    printf("waiting to enter match\n");
    while (!Broodwar->isInGame())
    {
      BWAPI::BWAPIClient.update();
      if (!BWAPI::BWAPIClient.isConnected())
      {
        printf("Reconnecting...\n");
        reconnect();
      }
    }
    printf("starting match!");
    while(Broodwar->isInGame())
    {
      std::set<Unit*> allUnits = Broodwar->getAllUnits();
      for(std::set<Unit*>::iterator u=allUnits.begin();u!=allUnits.end();u++)
      {
        if ((*u)->isMoving())
          Broodwar->drawCircleMap((*u)->getPosition().x(),(*u)->getPosition().y(),30,Colors::Green,false);
      }
      BWAPI::BWAPIClient.update();
      if (!BWAPI::BWAPIClient.isConnected())
      {
        printf("Reconnecting...\n");
        reconnect();
      }
    }
    printf("Game ended\n");
  }
  system("pause");
  return 0;
}

