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
    if (!s.isConnectedToLadderGame() && game!=NULL)
    {
      printf("Connecting to game %s\n",game->chGameName);
      s.connectToLadderGame(game);
      string message("Hello Server!");
      printf("Sending %s to %d\n",message.c_str(),game->playerProcessIDs[0]);
      if (s.sendData(message.c_str(),message.length(),game->playerProcessIDs[0]))
        printf("Sent\n");
      if (s.sendData(message.c_str(),message.length(),game->playerProcessIDs[0]))
        printf("Sent\n");
    }
  }
  system("pause");
  s.disconnect();
	return 0;
}