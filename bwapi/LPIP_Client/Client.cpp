// ClientTest.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <time.h>
using namespace std;
struct GameInfo
{
	int szSize;
	int flType;
	DWORD dwGameState;
	char chGameName[128];
	char chGameStats[128];
};
struct GameInfoTable
{
  GameInfo gameInfo[256];
  time_t gameInfoLastUpdate[256];
};
int main(int argc, char* argv[])
{
  cout << "Starting Client...\n";

  HANDLE mapFileHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "Global\\bwapi_local_lpip_games1234");
  GameInfoTable* data = NULL;
  if (mapFileHandle == INVALID_HANDLE_VALUE)
  {
    printf("Shared memory does not exist, creating...\n");
    //cannot connect to shared memory, so try to create it
    int size = sizeof(GameInfoTable);
    mapFileHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, "Global\\bwapi_local_lpip_games1234");
    if (mapFileHandle == INVALID_HANDLE_VALUE)
    {
      printf("Error: unable to make shared memory\n");
      return false;
    }
    data = (GameInfoTable*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameInfoTable));
    for(int i=0;i<256;i++)
      data->gameInfoLastUpdate[i] = 0;
  }
  else
    data = (GameInfoTable*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameInfoTable));

  printf("Checking shared memory for games...\n");
  time_t now = time(NULL);
  for(int i=0;i<256;i++)
  {
    if (data->gameInfoLastUpdate[i]<=now && (now-data->gameInfoLastUpdate[i])<(time_t)(3*60))
    {
      data->gameInfo[i].chGameName[127]='\0';
      printf("Game[%i] Name=%s",i,data->gameInfo[i].chGameName);
    }
  }
  system("pause");
	return 0;
}