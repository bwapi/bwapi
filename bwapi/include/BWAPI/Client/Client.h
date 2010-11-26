#pragma once
#include <windows.h>
#include "GameData.h"
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
    GameData* data;
    bool isConnected();
    bool connect();
    void disconnect();
    void update();

  private:
    HANDLE pipeObjectHandle;
    HANDLE mapFileHandle;
    bool connected;
    bool showedErrorBox;
  };
  extern Client BWAPIClient;
}
