#pragma once
#include <windows.h>
#include "GameData.h"
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include "GameTable.h"


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
    HANDLE gameTableFileHandle;
    GameTable* gameTable;
    
    bool connected;
    bool showedErrorBox;
  };
  extern Client BWAPIClient;
}
