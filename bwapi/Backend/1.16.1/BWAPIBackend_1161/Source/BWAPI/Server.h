#pragma once

#include "../../BWAPIBackendCore/BWAPIProtoClient.h"

#include <vector>
#include <unordered_map>

namespace BWAPI
{
  // Forwards
  struct GameData;
  struct GameTable;
  class Event;
  class ForceInterface;
  typedef ForceInterface* Force;
  class PlayerInterface;
  typedef PlayerInterface* Player;
  class UnitInterface;
  typedef UnitInterface* Unit;

  class Server
  {
  public:
    Server();
    ~Server();

    Server(const Server &other) = delete;
    Server(Server &&other) = delete;
    
    void      update();
    bool      isConnected() const;
    void       addEvent(const BWAPI::Event& e);
    int       addString(const char* text);
    void      clearAll();

    int       getForceID(Force force);
    Force     getForce(int id) const;
    
    int       getPlayerID(Player player);
    Player    getPlayer(int id) const;
    
    int       getUnitID(Unit unit);
    Unit      getUnit(int id) const;

    //std::unique_ptr<GameData> data;
    GameData* data;
    GameData* oldData;
  private:
    void onMatchStart();
    void checkForConnections();
    void initializeGameData();
    void updateGameData();
    void callOnFrame();
    void processMessages();
    void setWaitForResponse(bool wait);
    void receiveData();
    void sendData();
    //std::unique_ptr<bwapi::data::GameData> makeGameDataMessage();

    GameTable* gameTable = nullptr;
    int gameTableIndex = -1;
    bool connected = false;
    bool listening = false;
    bool localOnly = false;

    std::vector<Force> forceVector;
    std::unordered_map<Force, int> forceLookup;

    std::vector<Player> playerVector;
    std::unordered_map<Player, int> playerLookup;

    std::vector<Unit> unitVector;
    std::unordered_map<Unit, int> unitLookup;


    BWAPIProtoClient protoClient;
  };
}
