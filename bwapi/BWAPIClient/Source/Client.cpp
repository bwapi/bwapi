#include <BWAPI/Client/Client.h>
#include <windows.h>
#include <sstream>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

namespace BWAPI
{
  Client BWAPIClient;
  Client::Client()
    : pipeObjectHandle(INVALID_HANDLE_VALUE)
    , mapFileHandle(INVALID_HANDLE_VALUE)
    , gameTableFileHandle(INVALID_HANDLE_VALUE)
  {}
  Client::~Client()
  {
    this->disconnect();
  }
  bool Client::isConnected() const
  {
    return this->connected;
  }
  bool Client::connect()
  {
    if ( this->connected )
    {
      std::cout << "Already connected." << std::endl;
      return true;
    }

    int serverProcID    = -1;
    int gameTableIndex  = -1;

    this->gameTable = NULL;
    this->gameTableFileHandle = OpenFileMappingA(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, "Local\\bwapi_shared_memory_game_list" );
    if ( !this->gameTableFileHandle )
    {
      std::cerr << "Game table mapping not found." << std::endl;
      return false;
    }
    this->gameTable = static_cast<GameTable*>( MapViewOfFile(this->gameTableFileHandle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(GameTable)) );
    if ( !this->gameTable )
    {
      std::cerr << "Unable to map Game table." << std::endl;
      return false;
    }

    //Find row with most recent keep alive that isn't connected
    DWORD latest = 0;
    for(int i = 0; i < GameTable::MAX_GAME_INSTANCES; i++)
    {
      std::cout << i << " | " << gameTable->gameInstances[i].serverProcessID << " | " << gameTable->gameInstances[i].isConnected << " | " << gameTable->gameInstances[i].lastKeepAliveTime << std::endl;
      if (gameTable->gameInstances[i].serverProcessID != 0 && !gameTable->gameInstances[i].isConnected)
      {
        if ( gameTableIndex == -1 || latest == 0 || gameTable->gameInstances[i].lastKeepAliveTime < latest )
        {
          latest = gameTable->gameInstances[i].lastKeepAliveTime;
          gameTableIndex = i;
        }
      }
    }

    if (gameTableIndex != -1)
      serverProcID = gameTable->gameInstances[gameTableIndex].serverProcessID;

    if (serverProcID == -1)
    {
      std::cerr << "No server proc ID" << std::endl;
      return false;
    }
    
    std::stringstream sharedMemoryName;
    sharedMemoryName << "Local\\bwapi_shared_memory_";
    sharedMemoryName << serverProcID;

    std::stringstream communicationPipe;
    communicationPipe << "\\\\.\\pipe\\bwapi_pipe_";
    communicationPipe << serverProcID;

    pipeObjectHandle = CreateFileA(communicationPipe.str().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if ( pipeObjectHandle == INVALID_HANDLE_VALUE )
    {
      std::cerr << "Unable to open communications pipe: " << communicationPipe.str() << std::endl;
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

    std::cout << "Connected" << std::endl;
    mapFileHandle = OpenFileMappingA(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, sharedMemoryName.str().c_str());
    if (mapFileHandle == INVALID_HANDLE_VALUE || mapFileHandle == NULL)
    {
      std::cerr << "Unable to open shared memory mapping: " << sharedMemoryName.str() << std::endl;
      CloseHandle(pipeObjectHandle);
      CloseHandle(gameTableFileHandle);
      return false;
    }
    data = static_cast<GameData*>( MapViewOfFile(mapFileHandle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(GameData)) );
    if ( data == nullptr )
    {
      std::cerr << "Unable to map game data." << std::endl;
      return false;
    }

    // Create new instance of Game/Broodwar
    if ( BWAPI::BroodwarPtr )
      delete static_cast<GameImpl*>(BWAPI::BroodwarPtr);
    BWAPI::BroodwarPtr = new GameImpl(data);
    assert( BWAPI::BroodwarPtr != nullptr );

    if (BWAPI::BWAPI_getRevision() != BWAPI::Broodwar->getRevision())
    {
      //error
      std::cerr << "Error: Client and Server are not compatible!" << std::endl;
      std::cerr << "Client Revision: " << BWAPI::BWAPI_getRevision() << std::endl;
      std::cerr << "Server Revision: " << BWAPI::Broodwar->getRevision() << std::endl;
      disconnect();
      std::this_thread::sleep_for(std::chrono::milliseconds{ 2000 });
      return false;
    }
    //wait for permission from server before we resume execution
    int code = 1;
    while ( code != 2 )
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle, &code, sizeof(code), &receivedByteCount, NULL);
      if ( !success )
      {
        disconnect();
        std::cerr << "Unable to read pipe object." << std::endl;
        return false;
      }
    }
    
    std::cout << "Connection successful" << std::endl;
    assert( BWAPI::BroodwarPtr != nullptr);

    this->connected = true;
    return true;
  }
  void Client::disconnect()
  {
    if ( !this->connected ) return;
    
    if ( gameTableFileHandle != INVALID_HANDLE_VALUE )
      CloseHandle(gameTableFileHandle);
    gameTableFileHandle = INVALID_HANDLE_VALUE;

    if ( pipeObjectHandle != INVALID_HANDLE_VALUE )
      CloseHandle(pipeObjectHandle);
    pipeObjectHandle = INVALID_HANDLE_VALUE;
    
    if ( mapFileHandle != INVALID_HANDLE_VALUE )
      CloseHandle(mapFileHandle);
    mapFileHandle = INVALID_HANDLE_VALUE;

    this->connected = false;
    std::cout << "Disconnected" << std::endl;

    if ( BWAPI::BroodwarPtr )
      delete static_cast<GameImpl*>(BWAPI::BroodwarPtr);
    BWAPI::BroodwarPtr = nullptr;
  }
  void Client::update()
  {
    DWORD writtenByteCount;
    int code = 1;
    WriteFile(pipeObjectHandle, &code, sizeof(code), &writtenByteCount, NULL);
    //std::cout << "wrote to pipe" << std::endl;

    while (code != 2)
    {
      DWORD receivedByteCount;
      //std::cout << "reading pipe" << std::endl;
      BOOL success = ReadFile(pipeObjectHandle, &code, sizeof(code), &receivedByteCount, NULL);
      if ( !success )
      {
        std::cout << "failed, disconnecting" << std::endl;
        disconnect();
        return;
      }
    }
    //std::cout << "about to enter event loop" << std::endl;

    for(int i = 0; i < data->eventCount; ++i)
    {
      EventType::Enum type(data->events[i].type);

      if ( type == EventType::MatchStart )
        static_cast<GameImpl*>(BWAPI::BroodwarPtr)->onMatchStart();
      if ( type == EventType::MatchFrame || type == EventType::MenuFrame )
        static_cast<GameImpl*>(BWAPI::BroodwarPtr)->onMatchFrame();
    }
    if ( BWAPI::BroodwarPtr != nullptr && static_cast<GameImpl*>(BWAPI::BroodwarPtr)->inGame && !Broodwar->isInGame() )
      static_cast<GameImpl*>(BWAPI::BroodwarPtr)->onMatchEnd();
  }
}
