#include "Client.h"

namespace BWAPI
{
  Client BWAPIClient;
  Client::Client()
  {
    connected=false;
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
    pipeObjectHandle=CreateFileA("\\\\.\\pipe\\bwapi_pipe",GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
    COMMTIMEOUTS c;
    c.ReadIntervalTimeout = 100;
    c.ReadTotalTimeoutMultiplier = 100;
    c.ReadTotalTimeoutConstant = 2000;
    c.WriteTotalTimeoutMultiplier = 100;
    c.WriteTotalTimeoutConstant = 2000;
    if (pipeObjectHandle == INVALID_HANDLE_VALUE)
      return false;
    connected=true;
    SetCommTimeouts(pipeObjectHandle,&c);
    printf("Connected\n");
    mapFileHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "Global\\bwapi_shared_memory");
    if (mapFileHandle == INVALID_HANDLE_VALUE)
      return false;
    data = (GameData*) MapViewOfFile(mapFileHandle, FILE_MAP_ALL_ACCESS,0,0,sizeof(GameData));
    if (BWAPI::Broodwar!=NULL)
      delete (GameImpl*)BWAPI::Broodwar;
    BWAPI::Broodwar = new GameImpl(data);
    //wait for permission from server before we resume execution
    int code=1;
    while (code!=2)
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle,&code,sizeof(int),&receivedByteCount,NULL);
    }
    return true;
  }
  void Client::disconnect()
  {
    if (!connected) return;
    CloseHandle(pipeObjectHandle);
    connected=false;
    printf("Disconnected\n");
    if (BWAPI::Broodwar!=NULL)
      delete (GameImpl*)BWAPI::Broodwar;
    BWAPI::Broodwar=NULL;
  }
  void Client::update()
  {
    DWORD writtenByteCount;
    int code=1;
    WriteFile(pipeObjectHandle,&code,sizeof(int),&writtenByteCount,NULL);

    while (code!=2)
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle,&code,sizeof(int),&receivedByteCount,NULL);
      if (!success)
      {
        disconnect();
        return;
      }
    }
    for(int i=0; i<data->eventCount; i++)
    {
      Event e=((GameImpl*)BWAPI::Broodwar)->makeEvent(data->events[i]);

      if (e.type == EventType::MatchStart)
      {
        ((GameImpl*)BWAPI::Broodwar)->onMatchStart();
      }
      if (e.type == EventType::MatchEnd)
      {
        ((GameImpl*)BWAPI::Broodwar)->onMatchEnd();
      }
      if (e.type == EventType::MatchFrame)
      {
        ((GameImpl*)BWAPI::Broodwar)->onMatchFrame();
      }
    }
  }
}
