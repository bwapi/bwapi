#pragma once
#include <windows.h>
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
    void updateSharedMemory();
    void callOnFrame();
    void processCommands();
    void setWaitForResponse(bool wait);
    HANDLE pipeObjectHandle;
    bool connected;
  };
}