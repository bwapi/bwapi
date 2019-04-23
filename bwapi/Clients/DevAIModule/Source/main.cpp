#include <BWAPI.h>
#include "..\Frontend\BWAPIFrontendClient\Client.h"

#include <iostream>

using namespace BWAPI;

Client client;
Game broodwar;

int main()
{
  std::cout << "Connecting..." << std::endl;
  client.connect();
  while (true)
  {
    client.update(broodwar);
    std::cout << "waiting to enter match" << std::endl;
  }
  return 1;
}