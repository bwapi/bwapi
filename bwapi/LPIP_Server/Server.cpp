// ClientTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
using namespace std;
#include "SharedMemory.h"
int main(int argc, char* argv[])
{
  cout << "Starting Server...\n";
  SharedMemory s;
  while (!s.connect())
  {
  }

  GameInfo myGameInfo;
  //fill out name
  strncpy(myGameInfo.chGameName,"Hello Game",128);
  //fill out stats
  strncpy(myGameInfo.chGameStats,"Some Game Stats",128);
  s.advertiseLadderGame(&myGameInfo);
  printf("Added game 'Hello Game' to shared memory\n");
  system("pause");
  s.disconnect();
	return 0;
}