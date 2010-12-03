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
  for(int i=0;i<256;i++)
  {
    if (s.data->gameInfoLastUpdate[i]<=now && (now-s.data->gameInfoLastUpdate[i])<(time_t)(3*60))
    {
      s.data->gameInfo[i].chGameName[127]='\0';
      printf("Game[%i] Name=%s\n",i,s.data->gameInfo[i].chGameName);
    }
  }
  system("pause");
  s.disconnect();
	return 0;
}