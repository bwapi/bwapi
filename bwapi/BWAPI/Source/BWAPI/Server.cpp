#include "Server.h"
#include <Util/Logger.h>
#include <SharedStructures/GameData.h>
namespace BWAPI
{
  #define PIPE_TIMEOUT 2000
  #define PIPE_SYSTEM_BUFFER_SIZE 4096
  Server::Server()
  {
    pipeObjectHandle=CreateNamedPipe("\\\\.\\pipe\\bwapi_pipe",
                                           PIPE_ACCESS_DUPLEX,
                                           PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                                           PIPE_UNLIMITED_INSTANCES,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_SYSTEM_BUFFER_SIZE,
                                           PIPE_TIMEOUT,
                                           NULL);
    if (pipeObjectHandle == INVALID_HANDLE_VALUE)
      Util::Logger::globalLog->log("Error: unable to make pipe");
    COMMTIMEOUTS c;
    c.ReadIntervalTimeout = 100;
    c.ReadTotalTimeoutMultiplier = 100;
    c.ReadTotalTimeoutConstant = 2000;
    c.WriteTotalTimeoutMultiplier = 100;
    c.WriteTotalTimeoutConstant = 2000;
    SetCommTimeouts(pipeObjectHandle,&c);
    int size=sizeof(BWAPIC::GameData);
    mapFileHandle = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, "Global\\bwapi_shared_memory");
    if (mapFileHandle == INVALID_HANDLE_VALUE)
      Util::Logger::globalLog->log("Error: unable to make shared memory");
    data = (BWAPIC::GameData*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, size);
    initializeSharedMemory();
    connected=false;
  }
  Server::~Server()
  {
    DisconnectNamedPipe(pipeObjectHandle);
  }
  void Server::update()
  {
    if (connected)
    {
      updateSharedMemory();
      callOnFrame();
      processCommands();
    }
    else
    {
      checkForConnections();
    }
  }
  void Server::setWaitForResponse(bool wait)
  {
    DWORD dwMode = PIPE_READMODE_MESSAGE;
    if (wait)
      dwMode |= PIPE_WAIT;
    else
      dwMode |= PIPE_NOWAIT;
    SetNamedPipeHandleState(pipeObjectHandle,&dwMode,NULL,NULL);
  }
  void Server::checkForConnections()
  {
    if (connected) return;
    BOOL success = ConnectNamedPipe(pipeObjectHandle, NULL);
    if (!success && GetLastError() != ERROR_PIPE_CONNECTED) return;
    if (GetLastError() == ERROR_PIPE_CONNECTED)
      connected = true;
    if (!connected) return;
    setWaitForResponse(true);
  }
  void Server::initializeSharedMemory()
  {
    //called once when Starcraft starts. Not at the start of every match.
    strncpy(data->mapFilename,"",260);
    strncpy(data->mapName,"",32);
  }
  void Server::updateSharedMemory()
  {
    data->frameCount =Broodwar->getFrameCount();
    data->mouseX     =Broodwar->getMouseX();
    data->mouseY     =Broodwar->getMouseY();
    data->isInGame   =Broodwar->isInGame();
    if (Broodwar->isInGame())
    {
      data->screenX    =Broodwar->getScreenX();
      data->screenY    =Broodwar->getScreenY();
      //figure out what needs to be loaded once later
      data->mapWidth   =Broodwar->mapWidth();
      data->mapHeight  =Broodwar->mapHeight();

      strncpy(data->mapFilename,Broodwar->mapFilename().c_str(),260);
      strncpy(data->mapName,Broodwar->mapName().c_str(),32);
      data->mapHash=Broodwar->getMapHash();
      //worry about optimization later
      for(int x=0;x<Broodwar->mapWidth()*4;x++)
      {
        for(int y=0;y<Broodwar->mapHeight()*4;y++)
        {
          data->getGroundHeight[x][y]=Broodwar->getGroundHeight(x,y);
          data->isWalkable[x][y]=Broodwar->isWalkable(x,y);
        }
      }
      /*
      for(int x=0;x<Broodwar->mapWidth();x++)
      {
        for(int y=0;y<Broodwar->mapHeight();y++)
        {
          data->isBuildable[x][y]=Broodwar->isBuildable(x,y);
          data->isVisible[x][y]  =Broodwar->isVisible(x,y);
          data->isExplored[x][y] =Broodwar->isExplored(x,y);
          data->hasCreep[x][y]   =Broodwar->hasCreep(x,y);
        }
      }
      */
      data->isMultiplayer=Broodwar->isMultiplayer();
      data->isReplay     =Broodwar->isReplay();
      data->isPaused     =Broodwar->isPaused();
      data->latency      =Broodwar->getLatency();
    }
  }
  void Server::callOnFrame()
  { 
    DWORD writtenByteCount;
    int code=3;
    WriteFile(pipeObjectHandle,&code,sizeof(int),&writtenByteCount,NULL);
    while (code!=4)
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle,&code,sizeof(int),&receivedByteCount,NULL);
      if (!success)
      {
        DisconnectNamedPipe(pipeObjectHandle);
        connected=false;
        setWaitForResponse(false);
        break;
      }
    }
  }
  void Server::processCommands()
  {
  }
}