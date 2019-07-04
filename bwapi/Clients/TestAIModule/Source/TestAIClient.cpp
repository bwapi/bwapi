#include <BWAPI.h>
#include "..\Frontend\BWAPIFrontendClient\ProtoClient.h"

#include "Globals.h"
#include "TestMap1.h"

using namespace BWAPI;

ProtoClient BWAPIClient;
Game Broodwar(BWAPIClient);

void reconnect(Client &BWAPIClient)
{
  while (!BWAPIClient.connect())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
  }
}

int main(int argc, const char* argv[])
{
  TestMap1 testMap1;

  std::cout << "Connecting..." << std::endl;
  reconnect(BWAPIClient);
  while (true)
  {
    std::cout << "waiting to enter match" << std::endl;
    while (!Broodwar->isInGame())
    {
      while (!BWAPIClient.isConnected())
      {
        std::cout << "Reconnecting..." << std::endl;
        reconnect(BWAPIClient);
      }
      BWAPIClient.update(Broodwar);
    }
    while (Broodwar->isInGame())
    {
      auto map = Broodwar.mapName();
      if (map == "testmap1.scm")
      {
        testMap1.onFrame();
      }
      else
      {

      }
      BWAPIClient.update(Broodwar);
    }
  }



  return 0;
}