// ClientTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
using namespace std;
#include "../LPIP_Server/SharedMemory.h"
int main(int argc, char* argv[])
{
  printf("Starting Client [pid=%d]...\n",GetCurrentProcessId());
  SharedMemory s;
  while (!s.connect())
  {
  }

  printf("Checking shared memory for games...\n");
  while(true)
  {
    s.update();
    GameInfo* game = NULL;
    for each(GameInfo* g in s.games)
    {
      game = g;
    }
    if (!s.isConnectedToLadderGame())
    {
      s.connectToLadderGame(game);
    }
  }
  system("pause");
  s.disconnect();
	return 0;
}