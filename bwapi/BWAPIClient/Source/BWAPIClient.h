#pragma once
#include <windows.h>
#include <SharedStructures\GameData.h>
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"


namespace BWAPI
{
  class Client
  {
    public:
    Client();
    ~Client();
    BWAPIC::GameData* data;
    bool isConnected();
    bool connect();
    void disconnect();
    void update();

  private:
    HANDLE pipeObjectHandle;
    HANDLE mapFileHandle;
    bool connected;
  };
  extern Client BWAPIClient;
}
