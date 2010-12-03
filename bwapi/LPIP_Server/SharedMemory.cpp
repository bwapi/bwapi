#include <iostream>
using namespace std;
#include "SharedMemory.h"
#include <time.h>
#define PIPE_TIMEOUT 2000
#define PIPE_SYSTEM_BUFFER_SIZE 4096
SharedMemory::SharedMemory()
{
  myIndex        = -1;
  ownsLadderGame = false;
  myPipeName[0]  ='\0';
  data           = NULL;
  mapFileHandle  = INVALID_HANDLE_VALUE;
  myPipeHandle   = INVALID_HANDLE_VALUE;
}
SharedMemory::~SharedMemory()
{
  disconnect();
}
bool SharedMemory::connectPipe()
{
  if (myPipeHandle != INVALID_HANDLE_VALUE)
  {
    CloseHandle(myPipeHandle);
    myPipeHandle = INVALID_HANDLE_VALUE;
  }
  sprintf(myPipeName, "\\\\.\\pipe\\bwapi_player_%d", GetCurrentProcessId());
  myPipeHandle = CreateNamedPipe(myPipeName,
                                           PIPE_ACCESS_DUPLEX,
                                           PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                                           PIPE_UNLIMITED_INSTANCES,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_TIMEOUT,
                                           NULL);
  if (myPipeHandle == INVALID_HANDLE_VALUE)
  {
    printf("Error: Failed to create my pipe %d\n",GetCurrentProcessId());
    return false;
  }
  COMMTIMEOUTS c;
  c.ReadIntervalTimeout = 100;
  c.ReadTotalTimeoutMultiplier = 100;
  c.ReadTotalTimeoutConstant = 2000;
  c.WriteTotalTimeoutMultiplier = 100;
  c.WriteTotalTimeoutConstant = 2000;
  SetCommTimeouts(myPipeHandle,&c);
  return true;
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
    for(int i=0;i<256;i++)
      data->gameInfo[i].lastUpdate = 0;
  }
  else
    data = (GameInfoTable*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameInfoTable));
  return true;
}
bool SharedMemory::connect()
{
  bool success = connectPipe() && connectSharedMemory();
  printf("connect() = %d, %x\n",success,data);
  return success;
}
void SharedMemory::disconnect()
{
  printf("disconnect()\n");
  disconnectFromLadderGame();
  if (data !=NULL)
  {
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
  if (isConnectedToSharedMemory())
  {
    games.clear();
    time_t now = time(NULL);
    for(int i=0;i<256;i++)
    {
      if ((now-data->gameInfo[i].lastUpdate)<(time_t)(3*60))
        games.push_back(&data->gameInfo[i]);
    }
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
  if (myIndex>=0)
  {
    if (ownsLadderGame)
      keepAliveLadderGame();
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
bool SharedMemory::isConnectedToLadderGame()
{
  return myIndex>=0;
}
bool SharedMemory::sendData(const char *buf, unsigned int len, DWORD processID)
{
  if (processID == GetCurrentProcessId()) return false;
  DWORD writtenByteCount = 0;
  //look for player with the same process id
  char pipeName[256];
  sprintf(pipeName, "\\\\.\\pipe\\bwapi_player_%d", processID);
  HANDLE pipeHandle = INVALID_HANDLE_VALUE;
  while (pipeHandle == INVALID_HANDLE_VALUE) 
  { 
    pipeHandle = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
 
    // Break if the pipe handle is valid. 
 
    if (pipeHandle != INVALID_HANDLE_VALUE) 
      break; 
 
      // Exit if an error other than ERROR_PIPE_BUSY occurs. 
 
    if (GetLastError() != ERROR_PIPE_BUSY) 
    {
      printf( "Could not open pipe. LastError = %d\n", GetLastError() ); 
      return false;
    }
 
    // All pipe instances are busy, so wait for 20 seconds. 
 
    if ( ! WaitNamedPipe(pipeName, 5000)) 
    { 
      printf("Could not open pipe: 5 second wait timed out.\n"); 
      return false;
    } 
  }
  DWORD mode = PIPE_READMODE_MESSAGE; 
  if ( ! SetNamedPipeHandleState( pipeHandle,&mode,NULL,NULL)) 
  {
    printf( "SetNamedPipeHandleState failed. LastError = %d\n", GetLastError() ); 
    return false;
  }
  int myProcID = GetCurrentProcessId();
  WriteFile(pipeHandle,&myProcID,sizeof(DWORD),&writtenByteCount,NULL); //write data to that player's pipe
  WriteFile(pipeHandle,buf,len,&writtenByteCount,NULL); //write data to that player's pipe
  CloseHandle(pipeHandle);
  return true;
}
int SharedMemory::receiveData(const char *buf, unsigned int len, DWORD& processID, bool isBlocking)
{
  processID = 0;
  memset((void*)buf,0,len);
  DWORD receivedByteCount = 0;
  while(receivedByteCount == 0)
  {
    if (isBlocking)
      update();

    BOOL success = ReadFile(myPipeHandle,(LPVOID)(&processID),sizeof(DWORD),&receivedByteCount,NULL);
    if (!success && GetLastError() == ERROR_BROKEN_PIPE)
    {
      if (!connectPipe())
      {
        printf("Error: could not reconnect broken pipe\n");
      }
    }
    if (success)
    {
      ReadFile(myPipeHandle,(LPVOID)buf,len,&receivedByteCount,NULL);
    }
    if (!success) return -1;
    if (!isBlocking) break;
    Sleep(50);
  }
  return receivedByteCount;
}