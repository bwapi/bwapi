#pragma once
#include <windows.h>
#include <BWAPI/Client/GameData.h>
#include <BWAPI/Client/Event.h>
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
    void update();
    bool isConnected();
    int addEvent(BWAPI::Event e);
    int addString(const char* text);
    int getForceID(Force* force);
    Force* getForce(int id);
    int getPlayerID(Player* player);
    Player* getPlayer(int id);
    int getUnitID(Unit* unit);
    Unit* getUnit(int id);
    GameData* data;
    private:
    void clearAll();
    void onMatchStart();
    void checkForConnections();
    void initializeSharedMemory();
    void updateSharedMemory();
    void callOnFrame();
    void processCommands();
    void setWaitForResponse(bool wait);
    HANDLE pipeObjectHandle;
    HANDLE mapFileHandle;
    bool connected;
    std::vector<Force*> forceVector;
    std::map<Force*, int> forceLookup;

    std::vector<Player*> playerVector;
    std::map<Player*, int> playerLookup;

    std::vector<Unit*> unitVector;
    std::map<Unit*, int> unitLookup;
  };
}