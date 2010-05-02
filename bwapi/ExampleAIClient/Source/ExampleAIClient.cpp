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
      std::set<Unit*> myUnits = Broodwar->self()->getUnits();
      for(std::set<Unit*>::iterator u=Broodwar->getSelectedUnits().begin();u!=Broodwar->getSelectedUnits().end();u++)
      {
        int x=(*u)->getPosition().x();
        int y=(*u)->getPosition().y();
        Broodwar->drawCircleMap(x,y,20,Colors::Blue,false);
      }
      for(std::set<Unit*>::iterator u=myUnits.begin();u!=myUnits.end();u++)
      {
        std::list<UnitType> tq=(*u)->getTrainingQueue();
        int x=(*u)->getPosition().x();
        int y=(*u)->getPosition().y();
        for(std::list<UnitType>::iterator j=tq.begin();j!=tq.end();j++)
        {
          Broodwar->drawTextMap(x,y,"%s",(*j).getName().c_str());
          y+=16;
        }
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

