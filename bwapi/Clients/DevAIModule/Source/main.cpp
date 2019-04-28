#include <BWAPI.h>
#include "..\Frontend\BWAPIFrontendClient\Client.h"

#include <iostream>

using namespace BWAPI;


int main()
{
  Client BWAPIClient;
  Game broodwar(BWAPIClient);
  std::cout << "Connecting..." << std::endl;
  while (!BWAPIClient.connect())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
  }
  while (true)
  {
    BWAPIClient.update(broodwar);
    std::cout << "waiting to enter match" << std::endl;
  }
  return 1;
}