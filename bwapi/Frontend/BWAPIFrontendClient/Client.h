#pragma once
#include "GameData.h"
#include <BWAPI/Game.h>
#include "GameTable.h"
#include <map>
#include "../Backend/BWAPIBackendCore/BWAPIProtoClient.h"

#include <Windows.h>


namespace BWAPI
{
  class Client
  {
  public:
    Client();
    ~Client();

    bool isConnected() const;
    bool connect();
    void disconnect();
    void update(Game& game);

    BWAPI4::GameData* data = nullptr;
  private:
    HANDLE      pipeObjectHandle;
    HANDLE      mapFileHandle;
    HANDLE      gameTableFileHandle;
    GameTable*  gameTable = nullptr;

    void onMatchFrame(Game& game);
    void onMatchStart(Game& game);
    void onMatchEnd(Game& game);
    void clearAll();

    void initGame(Game& game);
    void updateGame(Game& game);
    void initForces(Game& game);
    void initPlayers(Game& game);
    void initInitialUnits(Game& game);
    void initRegions(Game& game);

    bool wasInGame = false;

    bool connected = false;

    std::set<PlayerData, IDCompare> players;
    std::set<UnitData, IDCompare> units;
    std::set<RegionData, IDCompare> regions;
    std::set<BulletData, IDCompare> bullets;
    std::set<ForceData, IDCompare> forces;

    BWAPIProtoClient protoClient;

  };
  //static inline Client BWAPIClient;
}
