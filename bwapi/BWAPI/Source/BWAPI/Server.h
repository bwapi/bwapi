#pragma once
#include <windows.h>
#include <SharedStructures/GameData.h>
namespace BWAPI
{
  class Server
  {
    public:
    Server();
    ~Server();
    void update();
    private:
    void checkForConnections();
    void initializeSharedMemory();
    void updateSharedMemory();
    void callOnFrame();
    void processCommands();
    void setWaitForResponse(bool wait);
    HANDLE pipeObjectHandle;
    HANDLE mapFileHandle;
    BWAPIC::GameData* data;
    bool connected;
  };
}