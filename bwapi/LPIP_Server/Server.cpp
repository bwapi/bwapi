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

  int freeIndex = -1;

  time_t now = time(NULL);
  for(int i=0;i<256;i++)
  {
    if (now-s.data->gameInfoLastUpdate[i]>3*60)
    {
      freeIndex = i;
      break;
    }
  }
  if (freeIndex>=0)
  {
    s.data->gameInfoLastUpdate[freeIndex] = now;
    strncpy(s.data->gameInfo[freeIndex].chGameName,"Hello Game",128);
  }
  printf("Added game 'Hello Game' to shared memory\n");
  system("pause");
  s.disconnect();
	return 0;
}