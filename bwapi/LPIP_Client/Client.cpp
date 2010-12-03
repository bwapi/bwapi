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
  cout << "Starting Client...\n";
  SharedMemory s;
  while (!s.connect())
  {
  }

  printf("Checking shared memory for games...\n");
  time_t now = time(NULL);
  s.updateGameList();
  for each(GameInfo* g in s.games)
  {
    printf("Game\n");
    printf("chGameName=%s\n",g->chGameName);
    printf("chGameStats=%s\n",g->chGameStats);
  }
  system("pause");
  s.disconnect();
	return 0;
}