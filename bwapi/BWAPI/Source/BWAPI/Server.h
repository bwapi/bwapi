#pragma once
#include <windows.h>
#include <SharedStructures/GameData.h>
#include <SharedStructures/Event.h>
#include <BWAPI.h>
#include <vector>
#include <map>
namespace BWAPI
{
  class Server
  {
    public:
    Server();
    ~Server();
    void onMatchStart();
    void onMatchEnd();
    void update();
    int addEvent(BWAPI::Event e);
    int addString(const char* text);
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
    bool inMatch;
    std::vector<Force*> forceVector;
    std::map<Force*, int> forceLookup;
    int getForceID(Force* force);

    std::vector<Player*> playerVector;
    std::map<Player*, int> playerLookup;
    int getPlayerID(Player* player);

    std::vector<Unit*> unitVector;
    std::map<Unit*, int> unitLookup;
    int getUnitID(Unit* unit);
  };
}