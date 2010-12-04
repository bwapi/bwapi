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
  if (!s.connect())
    printf("Error could not connect\n");


  printf("Checking shared memory for games...\n");
  int sentmsgs=0;
  while(true)
  {
    s.update();
    GameInfo* game = NULL;
    for each(GameInfo* g in s.games)
    {
      game = g;
    }
    if (game!=NULL && sentmsgs==0)
    {
      sentmsgs++;
      string message("Hello Server!");
      printf("Sending %s to %d\n",message.c_str(),game->serverProcessID);
      if (s.sendData(message.c_str(),message.length(),game->serverProcessID))
        printf("Sent\n");
    }
  }
  system("pause");
  s.disconnect();
	return 0;
}