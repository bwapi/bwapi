#pragma once
#pragma once
#include <windows.h>
#include <SharedStructures\SharedStuff.h>
#include <SharedStructures\GameData.h>

namespace BWAPI
{
  class Client
  {
    public:
    static BWAPIC::GameData* gameData;
    static BWAPIC::SharedStuff sharedStuff;
    Client();
    ~Client();
    bool isConnected();
    bool connect();
    void disconnect();
    void update();

  private:
    HANDLE pipeObjectHandle;
    bool connected;
  };
  extern Client BWAPIClient;
}
