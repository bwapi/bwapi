#include <iostream>
using namespace std;
#include "SharedMemory.h"
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
  UnmapViewOfFile(mapFileHandle);
  mapFileHandle = NULL;
  data = NULL;
}