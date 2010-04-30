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
  printf("Connecting...");
  reconnect();
  while(true)
  {
    bool first=true;
    while(true)
    {
      if (first)
      {
        printf("in game!");
        first=false;
      }
      printf("update\n");
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

