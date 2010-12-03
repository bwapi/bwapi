#include <iostream>
using namespace std;
#include "SharedMemory.h"
#include <time.h>
#define PIPE_TIMEOUT 2000
#define PIPE_SYSTEM_BUFFER_SIZE 4096
SharedMemory::SharedMemory()
{
  myIndex = -1;
  ownsLadderGame = false;
  for(int i=0;i<10;i++)
  {
    pipeHandle[i] = NULL;
  }
}
SharedMemory::~SharedMemory()
{
  if (data!=NULL)
    disconnect();
}
bool SharedMemory::connect()
{
  
  mapFileHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "Global\\bwapi_local_lpip_game");
  data = NULL;
  if (mapFileHandle == INVALID_HANDLE_VALUE || mapFileHandle == NULL)
  {
    printf("Shared memory does not exist, creating...\n");
    //cannot connect to shared memory, so try to create it
    int size = sizeof(GameInfoTable);
    mapFileHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, "Global\\bwapi_local_lpip_game");
    if (mapFileHandle == INVALID_HANDLE_VALUE || mapFileHandle == NULL)
    {
      printf("Error: unable to make shared memory\n");
      return false;
    }
    data = (GameInfoTable*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameInfoTable));
    for(int i=0;i<256;i++)
      data->gameInfo[i].lastUpdate = 0;
  }
  else
    data = (GameInfoTable*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameInfoTable));
  return true;
}
void SharedMemory::disconnect()
{
  disconnectFromLadderGame();
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
      if (now-data->gameInfo[i].lastUpdate>3*60)
      {
        myIndex = i;
        break;
      }
    }
  }
  if (myIndex>=0)
  {
    //update last update time to current time (now)
    data->gameInfo[myIndex].lastUpdate = now;

    //add game data to shared memory
    data->gameInfo[myIndex] = *gm;
    data->gameInfo[myIndex].playerProcessIDs[0] = GetCurrentProcessId();
    for(int i=1;i<10;i++)
      data->gameInfo[myIndex].playerProcessIDs[i] = -1;
    data->gameInfo[myIndex].tableIndex = myIndex;
    ownsLadderGame = true;
  }
  return myIndex>=0;
}
void SharedMemory::keepAliveLadderGame()
{
  //update last update time to current time (now)
  time_t now = time(NULL);
  if (myIndex>=0)
    data->gameInfo[myIndex].lastUpdate = now;
}
void SharedMemory::removeLadderGameAd()
{
  if (data && myIndex>=0)
  {
    data->gameInfo[myIndex].lastUpdate = 0;
    myIndex = -1;
    ownsLadderGame = false;
  }
}
void SharedMemory::updateGameList()
{
  games.clear();
  time_t now = time(NULL);
  for(int i=0;i<256;i++)
  {
    if ((now-data->gameInfo[i].lastUpdate)<(time_t)(3*60))
      games.push_back(&data->gameInfo[i]);
  }
}
bool SharedMemory::connectToLadderGame(GameInfo* gm)
{
  if (!gm) return false;
  myIndex = gm->tableIndex;
  int id = GetCurrentProcessId();
  int index = -1;
  for(int i=0;i<10;i++)
  {
    if (gm->playerProcessIDs[i]==id)
      return true;
    if (gm->playerProcessIDs[i]==-1)
      index = i;
  }
  if (index==-1)
  {
    printf("Error: Cannot join because game already has 10 players\n");
    return false;
  }
  gm->playerProcessIDs[index]=id;
  update();
  return true;
}
void SharedMemory::disconnectFromLadderGame()
{
  if (ownsLadderGame)
  {
    removeLadderGameAd();
    return;
  }
  int id = GetCurrentProcessId();
  if (myIndex>=0)
  {
    GameInfo* gm = &data->gameInfo[myIndex];
    for(int i=0;i<10;i++)
    {
      if (gm->playerProcessIDs[i]==id)
        gm->playerProcessIDs[i]=-1;
    }
  }
  myIndex = -1;
  update();
}

void SharedMemory::update()
{
  updateGameList();
  int id = GetCurrentProcessId();
  if (myIndex>=0)
  {
    GameInfo* gm = &data->gameInfo[myIndex];
    if (ownsLadderGame)
      gm->lastUpdate = time(NULL);
    for(int i=0;i<10;i++)
    {
      if (gm->playerProcessIDs[i]!=id && gm->playerProcessIDs[i]!=-1 && (pipeHandle[i] == INVALID_HANDLE_VALUE || pipeHandle[i] == NULL))
      {
        int a=id;
        int b=gm->playerProcessIDs[i];
        if (a>b)
        {
          int temp=a;
          a=b;
          b=temp;
        }
        sprintf(pipeName[i], "\\\\.\\pipe\\bwapi_playerplayer_%d_%d", a,b);
        pipeHandle[i] = CreateFileA(pipeName[i],GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
        if (pipeHandle[i] == INVALID_HANDLE_VALUE || pipeHandle[i] == NULL)
        {
          //cannot connect to shared memory, so try to create it
          pipeHandle[i] = CreateNamedPipe(pipeName[i],
                                           PIPE_ACCESS_DUPLEX,
                                           PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                                           PIPE_UNLIMITED_INSTANCES,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_TIMEOUT,
                                           NULL);
          if (pipeHandle[i] == INVALID_HANDLE_VALUE || pipeHandle[i] == NULL)
            printf("Error: Failed to create pipe between process %d and process %d\n",a,b);
        }
        if (pipeHandle[i] != INVALID_HANDLE_VALUE &&  pipeHandle[i] != NULL)
          printf("Created pipe to process %d\n",gm->playerProcessIDs[i]);
      }
      if (gm->playerProcessIDs[i]==-1 && pipeHandle[i] != INVALID_HANDLE_VALUE && pipeHandle[i] != NULL)
      {
        CloseHandle(pipeHandle[i]);
        pipeHandle[i] = NULL;
        printf("Closed pipe to process %d\n",gm->playerProcessIDs[i]);
      }
    }
  }
}
bool SharedMemory::isConnectedToSharedMemory()
{
  return data!=NULL;
}
bool SharedMemory::isConnectedToLadderGame()
{
  return myIndex>=0;
}
bool SharedMemory::sendData(const char *buf, unsigned int len, int processID)
{
  if (myIndex>=0)
  {
    GameInfo* gm = &data->gameInfo[myIndex];
    DWORD writtenByteCount = 0xFFFFFFFF;
    //look for player with the same process id
    for(int i=0;i<10;i++)
      if (gm->playerProcessIDs[i] == processID) //found player
        WriteFile(pipeHandle[i],buf,len,&writtenByteCount,NULL); //write data to that player's pipe
    if (writtenByteCount==len) return true;
  }
  return false;
}