#include <iostream>
using namespace std;
#include "SharedMemory.h"
#include <time.h>
#define PIPE_TIMEOUT 2000
#define PIPE_SYSTEM_BUFFER_SIZE 4096
SharedMemory::SharedMemory()
{
  id             = GetCurrentProcessId();
  gameIndex      = -1;
  playerIndex    = -1;
  ownsLadderGame = false;
  data           = NULL;
  mapFileHandle  = INVALID_HANDLE_VALUE;
  for(int i=0;i<PLAYER_MAX;i++)
  {
    pipeHandle[i] = INVALID_HANDLE_VALUE;
    pipeName[i][0]='\0';
  }
}
SharedMemory::~SharedMemory()
{
  disconnect();
}
bool SharedMemory::playerExists(int i)
{
  if (!data) return false;
  return (time(NULL)-data->playerInfo[i].lastUpdate)<(time_t)(5);
}
bool SharedMemory::gameExists(int i)
{
  if (!data) return false;
  return (time(NULL)-data->gameInfo[i].lastUpdate)<(time_t)(5);
}

void SharedMemory::initGameInfoTable()
{
  if (!data) return;
  for(int i=0;i<GAME_MAX;i++)
  {
    data->gameInfo[i].lastUpdate = 0;
    data->gameInfo[i].tableIndex = i;
  }
  for(int i=1;i<PLAYER_MAX;i++)
  {
    data->playerInfo[i].lastUpdate = 0;
    data->playerInfo[i].tableIndex = i;
  }
}
bool SharedMemory::connectSharedMemory()
{
  if (mapFileHandle != INVALID_HANDLE_VALUE)
  {
    UnmapViewOfFile(data);
    CloseHandle(mapFileHandle);
    mapFileHandle = INVALID_HANDLE_VALUE;
  }
  mapFileHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "Global\\bwapi_local_lpip_game");
  data = NULL;
  if (mapFileHandle == INVALID_HANDLE_VALUE || mapFileHandle == NULL)
  {
    //cannot connect to shared memory, so try to create it
    int size = sizeof(GameInfoTable);
    mapFileHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, "Global\\bwapi_local_lpip_game");
    if (mapFileHandle == INVALID_HANDLE_VALUE)
    {
      printf("Error: unable to make shared memory\n");
      return false;
    }
    data = (GameInfoTable*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameInfoTable));
    initGameInfoTable();
  }
  else
    data = (GameInfoTable*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameInfoTable));

  playerIndex = -1;
  for(int i=0;i<PLAYER_MAX;i++)
  {
    if (!playerExists(i) || data->playerInfo[i].procID==id)
    {
      playerIndex = i;
      break;
    }
  }
  if (playerIndex == -1)
  {
    printf("Error: Cannot join because local host has 100 instances of starcraft running D:\n");
    return false;
  }
  data->playerInfo[playerIndex].procID     = id;
  data->playerInfo[playerIndex].lastUpdate = time(NULL);
  printf("My player index is %d\n",playerIndex);
  update();
  return true;
}
bool SharedMemory::connect()
{
  bool success = connectSharedMemory();
  printf("connect() = %d, %x\n",success,data);
  return success;
}
void SharedMemory::disconnect()
{
  printf("disconnect()\n");
  if (data !=NULL)
  {
    removeLadderGameAd();
    data->playerInfo[playerIndex].lastUpdate = 0;
    UnmapViewOfFile(data);
    data = NULL;
  }
  if (mapFileHandle != INVALID_HANDLE_VALUE)
  {
    UnmapViewOfFile(data);
    CloseHandle(mapFileHandle);
    mapFileHandle = INVALID_HANDLE_VALUE;
  }
  if (myPipeHandle != INVALID_HANDLE_VALUE)
  {
    CloseHandle(myPipeHandle);
    myPipeHandle = INVALID_HANDLE_VALUE;
  }
}
bool SharedMemory::advertiseLadderGame(GameInfo* gm)
{
  if (data == NULL)
  {
    printf("Error: Not connected to shared memory\n");
    return false;
  }
  time_t now = time(NULL);
  if (gameIndex==-1)
  {
    for(int i=0;i<GAME_MAX;i++)
    {
      if (gameExists(i)==false)
      {
        gameIndex = i;
        break;
      }
    }
  }
  if (gameIndex>=0)
  {
    //update last update time to current time (now)
    data->gameInfo[gameIndex].lastUpdate = now;

    //add game data to shared memory
    data->gameInfo[gameIndex] = *gm;
    data->gameInfo[gameIndex].tableIndex = gameIndex;
    ownsLadderGame = true;
    data->gameInfo[gameIndex].serverProcessID = id;
  }
  return gameIndex>=0;
}
void SharedMemory::removeLadderGameAd()
{
  if (data && gameIndex>=0 && ownsLadderGame)
  {
    data->gameInfo[gameIndex].lastUpdate = 0;
    gameIndex = -1;
    ownsLadderGame = false;
  }
}
void SharedMemory::updateGameList()
{
  if (isConnectedToSharedMemory())
  {
    games.clear();
    time_t now = time(NULL);
    for(int i=0;i<GAME_MAX;i++)
    {
      if ((now-data->gameInfo[i].lastUpdate)<(time_t)(3*60))
        games.push_back(&data->gameInfo[i]);
    }
  }
}
void SharedMemory::update()
{
  updateGameList();
  if (isConnectedToSharedMemory())
    data->playerInfo[playerIndex].lastUpdate = time(NULL);

  if (gameIndex>=0 && ownsLadderGame)
    data->gameInfo[gameIndex].lastUpdate = time(NULL);

  for(int i=0;i<PLAYER_MAX;i++)
  {
    if (playerExists(i) && data->playerInfo[i].procID!=id && (pipeHandle[i] == INVALID_HANDLE_VALUE || pipeHandle[i] == NULL))
    {
      int a=id;
      int b=data->playerInfo[i].procID;
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
        printf("Created pipe to process %d\n",data->playerInfo[i].procID);
    }
    if (!playerExists(i) && pipeHandle[i] != INVALID_HANDLE_VALUE && pipeHandle[i] != NULL)
    {
      CloseHandle(pipeHandle[i]);
      pipeHandle[i] = NULL;
      printf("Closed pipe to process %d\n",data->playerInfo[i].procID);
    }
  }
}
bool SharedMemory::isConnectedToPipe()
{
  return myPipeHandle != INVALID_HANDLE_VALUE;
}
bool SharedMemory::isConnectedToSharedMemory()
{
  return data != NULL;
}
bool SharedMemory::sendData(const char *buf, unsigned int len, DWORD processID)
{
  if (processID == id) return false;
  DWORD writtenByteCount = 0;
  //look for player with the same process id
  for(int i=0;i<PLAYER_MAX;i++)
    if (data->playerInfo[i].procID == processID) //found player
      WriteFile(pipeHandle[i],buf,len,&writtenByteCount,NULL); //write data to that player's pipe
  if (writtenByteCount==len) return true;
  return false;

}
int SharedMemory::receiveData(const char *buf, unsigned int len, DWORD *processID, bool isBlocking, bool* wantExit)
{
  if ( !processID || !buf || len <= 0 )
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return -1;
  }

  *processID = 0;
  memset((void*)buf,0,len);
  DWORD receivedByteCount = 0;
  while(receivedByteCount == 0)
  {
    if (isBlocking)
      update();
    //look for player with the same process id
    for(int i=0;i<PLAYER_MAX;i++)
      if (pipeHandle[i] != INVALID_HANDLE_VALUE && pipeHandle[i] != NULL) //found player
      {
        BOOL success = ReadFile(pipeHandle[i],(LPVOID)buf,len,&receivedByteCount,NULL);
        if (success)
        {
          *processID = data->playerInfo[i].procID;
          break;
        }
      }
    //if (!success) return -1;
    if (!isBlocking) break;
    if (*wantExit) break;
    Sleep(50);
  }
  return receivedByteCount;
}