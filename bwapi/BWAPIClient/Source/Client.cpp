#include "Client.h"
#include <windows.h>
#include <sstream>
#include <iostream>

namespace BWAPI
{
  Client BWAPIClient;
  Client::Client()
  {
    pipeObjectHandle    = INVALID_HANDLE_VALUE;
    mapFileHandle       = INVALID_HANDLE_VALUE;
    gameTableFileHandle = INVALID_HANDLE_VALUE;
    connected           = false;
    showedErrorBox      = false;
  }
  Client::~Client()
  {
    if (connected)
    {
      disconnect();
      connected=false;
    }
  }
  bool Client::isConnected()
  {
    return connected;
  }
  bool Client::connect()
  {
    if (connected) return true;
    int serverProcID = -1;
    int gameTableIndex = -1;
    gameTable = NULL;
    gameTableFileHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "Global\\bwapi_shared_memory_game_list" );
    if (gameTableFileHandle == INVALID_HANDLE_VALUE || gameTableFileHandle == NULL )
      return false;
    gameTable = (GameTable*) MapViewOfFile(gameTableFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameTable));

    //Find row with most recent keep alive that isn't connected
      
    time_t latest;
    for(int i = 0; i < GameTable::MAX_GAME_INSTANCES; i++)
    {
      std::cout << i << " | " << gameTable->gameInstances[i].serverProcessID << " | " << gameTable->gameInstances[i].isConnected << " | " << gameTable->gameInstances[i].lastKeepAliveTime << "\n";
      if (gameTable->gameInstances[i].serverProcessID != 0xFFFFFFFF && !gameTable->gameInstances[i].isConnected)
      {
        if (gameTableIndex == -1 || gameTable->gameInstances[i].lastKeepAliveTime < latest)
        {
          latest = gameTable->gameInstances[i].lastKeepAliveTime;
          gameTableIndex = i;
        }
      }
    }
    if (gameTableIndex != -1)
    {
      serverProcID = gameTable->gameInstances[gameTableIndex].serverProcessID;
    }

    if (serverProcID == -1)
    {
      return false;
    }
    
    std::stringstream sharedMemoryName;
    sharedMemoryName << "Global\\bwapi_shared_memory_";
    sharedMemoryName << serverProcID;

    std::stringstream communicationPipe;
    communicationPipe << "\\\\.\\pipe\\bwapi_pipe_";
    communicationPipe << serverProcID;


    pipeObjectHandle = CreateFileA(communicationPipe.str().c_str(),GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
    if (pipeObjectHandle == INVALID_HANDLE_VALUE || pipeObjectHandle == NULL)
    {
      CloseHandle(gameTableFileHandle);
      return false;
    }

    COMMTIMEOUTS c;
    c.ReadIntervalTimeout         = 100;
    c.ReadTotalTimeoutMultiplier  = 100;
    c.ReadTotalTimeoutConstant    = 2000;
    c.WriteTotalTimeoutMultiplier = 100;
    c.WriteTotalTimeoutConstant   = 2000;
    SetCommTimeouts(pipeObjectHandle,&c);

    connected=true;
    printf("Connected\n");
    mapFileHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, sharedMemoryName.str().c_str());
    if (mapFileHandle == INVALID_HANDLE_VALUE || mapFileHandle == NULL)
    {
      CloseHandle(pipeObjectHandle);
      CloseHandle(gameTableFileHandle);
      return false;
    }
    data = (GameData*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameData));
    if (BWAPI::Broodwar!=NULL)
      delete (GameImpl*)BWAPI::Broodwar;
    BWAPI::Broodwar = new GameImpl(data);
    if (BWAPI::BWAPI_getRevision() != BWAPI::Broodwar->getRevision())
    {
      //error
      printf("Error: Client and Server are not compatible!\n");
      printf("Client Revision: %d\n", BWAPI::BWAPI_getRevision());
      printf("Server Revision: %d\n", BWAPI::Broodwar->getRevision());
      disconnect();
      Sleep(2000);
      return false;
    }
    //wait for permission from server before we resume execution
    int code=1;
    while (code!=2)
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle, &code, sizeof(int), &receivedByteCount, NULL);
    }
    return true;
  }
  void Client::disconnect()
  {
    if (!connected) return;
    CloseHandle(gameTableFileHandle);
    CloseHandle(pipeObjectHandle);
    CloseHandle(mapFileHandle);
    connected = false;
    printf("Disconnected\n");
    if ( BWAPI::Broodwar != NULL )
      delete (GameImpl*)BWAPI::Broodwar;
    BWAPI::Broodwar = NULL;
  }
  void Client::update()
  {
    DWORD writtenByteCount;
    int code = 1;
    WriteFile(pipeObjectHandle, &code, sizeof(int), &writtenByteCount, NULL);

    while (code != 2)
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle,&code,sizeof(int),&receivedByteCount,NULL);
      if (!success)
      {
        disconnect();
        return;
      }
    }
    for(int i = 0; i < data->eventCount; ++i)
    {
      EventType::Enum type(data->events[i].type);

      if (type == EventType::MatchStart)
        ((GameImpl*)BWAPI::Broodwar)->onMatchStart();
      if (type == EventType::MatchFrame || type == EventType::MenuFrame)
        ((GameImpl*)BWAPI::Broodwar)->onMatchFrame();
    }
    if ( Broodwar != NULL && ((GameImpl*)BWAPI::Broodwar)->inGame && !Broodwar->isInGame() )
      ((GameImpl*)BWAPI::Broodwar)->onMatchEnd();
  }
}
