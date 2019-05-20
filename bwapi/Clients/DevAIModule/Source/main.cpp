#include <BWAPI.h>
#include "..\Frontend\BWAPIFrontendClient\Client.h"

#include <iostream>

using namespace BWAPI;


int main()
{
  Client BWAPIClient;
  Game broodwar(BWAPIClient);
  int i = 0;
  std::cout << "Connecting..." << std::endl;
  while (!BWAPIClient.connect())
  {
  }
  BWAPIClient.update(broodwar);
  std::cout << "waiting to enter match" << std::endl;
  while (BWAPIClient.isConnected())
  {
    while (!broodwar.isInGame())
    {
      BWAPIClient.update(broodwar);
    }
    std::cout << "Starting match!" << std::endl;
    while (broodwar.isInGame())
    {
      for (auto e : broodwar.getEvents())
      {
        switch (e.getType())
        {
        default:
          break;
        }
      }
      BWAPIClient.update(broodwar);
    }
  }
  return 1;
}