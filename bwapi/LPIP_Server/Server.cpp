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
  printf("Starting Server [pid=%d]...\n",GetCurrentProcessId());
  SharedMemory s;
  if (!s.connect())
    printf("Error could not connect\n");

  GameInfo myGameInfo;
  //fill out name
  strncpy(myGameInfo.chGameName,"Hello Game",128);
  //fill out stats
  strncpy(myGameInfo.chGameStats,"Some Game Stats",128);
  s.advertiseLadderGame(&myGameInfo);
  printf("Added game 'Hello Game' to shared memory\n");

  char buffer[1024];
  DWORD processID;
  while(true)
  {
    bool wantExit = false;
    s.update();
    int rVal = s.receiveData(buffer,1024,&processID,false,&wantExit);
    if (rVal>0)
      printf("Received the following data from process %d: %s\n",processID,buffer);
  }
  s.disconnect();
	return 0;
}