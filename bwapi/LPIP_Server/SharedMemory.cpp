#include <iostream>
using namespace std;
#include "SharedMemory.h"
#include <time.h>
SharedMemory::SharedMemory()
{
  myIndex = -1;
}
bool SharedMemory::connect()
{
  
  mapFileHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "Global\\bwapi_local_lpip_game6");
  data = NULL;
  if (mapFileHandle == INVALID_HANDLE_VALUE || mapFileHandle == NULL)
  {
    printf("Shared memory does not exist, creating...\n");
    //cannot connect to shared memory, so try to create it
    int size = sizeof(GameInfoTable);
    mapFileHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, "Global\\bwapi_local_lpip_game6");
    if (mapFileHandle == INVALID_HANDLE_VALUE || mapFileHandle == NULL)
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
  return true;
}
void SharedMemory::disconnect()
{
  removeLadderGameAd();
  UnmapViewOfFile(mapFileHandle);
  mapFileHandle = NULL;
  data = NULL;
}
bool SharedMemory::advertiseLadderGame(GameInfo* gm)
{
  if (!data) return false;
  time_t now = time(NULL);
  if (myIndex==-1)
  {
    for(int i=0;i<256;i++)
    {
      if (now-data->gameInfoLastUpdate[i]>3*60)
      {
        myIndex = i;
        break;
      }
    }
  }
  if (myIndex>=0)
  {
    //update last update time to current time (now)
    data->gameInfoLastUpdate[myIndex] = now;

    //add game data to shared memory
    data->gameInfo[myIndex] = *gm;
  }
  return myIndex>=0;
}

void SharedMemory::removeLadderGameAd()
{
  if (data && myIndex>=0)
    data->gameInfoLastUpdate[myIndex] = 0;
}
void SharedMemory::updateGameList()
{
  games.clear();
  time_t now = time(NULL);
  for(int i=0;i<256;i++)
  {
    if ((now-data->gameInfoLastUpdate[i])<(time_t)(3*60))
      games.push_back(&data->gameInfo[i]);
  }
}