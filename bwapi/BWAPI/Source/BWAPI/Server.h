#pragma once
#include <windows.h>

#include <vector>
#include <map>

namespace BWAPI
{
  // Forwards
  struct GameData;
  struct GameTable;
  class Event;
  class ForceInterface;
  typedef ForceInterface *Force;
  class PlayerInterface;
  typedef PlayerInterface *Player;
  class UnitInterface;
  typedef UnitInterface *Unit;

  class Server
  {
  public:
    Server();
    ~Server();
    
    void      update();
    bool      isConnected();
    int       addEvent(BWAPI::Event e);
    int       addString(const char* text);
    void      clearAll();

    int       getForceID(Force force);
    Force     getForce(int id) const;
    
    int       getPlayerID(Player player);
    Player    getPlayer(int id) const;
    
    int       getUnitID(Unit unit);
    Unit      getUnit(int id) const;

    GameData  *data;
  private:
    void onMatchStart();
    void checkForConnections();
    void initializeSharedMemory();
    void updateSharedMemory();
    void callOnFrame();
    void processCommands();
    void setWaitForResponse(bool wait);
    HANDLE pipeObjectHandle;
    HANDLE mapFileHandle;
    HANDLE gameTableFileHandle;
    GameTable* gameTable;
    int gameTableIndex;
    bool connected;
    bool localOnly;
    std::vector<Force> forceVector;
    std::map<Force, int> forceLookup;

    std::vector<Player> playerVector;
    std::map<Player, int> playerLookup;

    std::vector<Unit> unitVector;
    std::map<Unit, int> unitLookup;

    PSID pEveryoneSID;
    PACL pACL;
    PSECURITY_DESCRIPTOR pSD;
  };
}
