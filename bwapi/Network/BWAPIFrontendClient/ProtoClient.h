#pragma once
#include <set>
#include <string>

#include "../BWAPINetworkCore/BWAPIProtoClient.h"
#include "../SCRAPINetworkCore/SCRAPIProtoClient.h"

#include <BWAPI/Client.h>

namespace BWAPI
{
  class Game;

  class ProtoClient : public Client
  {
  public:
    friend class Game;
    ProtoClient();
    virtual ~ProtoClient();

    bool isConnected() const override;
    bool connect() override;
    void disconnect() override;
    void update(Game& game) override;

    void setScreenPosition(int x, int y) override;
    void pingMinimap(int x, int y) override;
    void drawShape(ShapeType::Enum shapeType, CoordinateType::Enum coordinateType, int x1, int y1, int x2, int y2, int extra1, int extra2, Color color, bool isSolid) override;
    void drawText(CoordinateType::Enum coordinateType, const std::string &text, int x, int y, int textSize) override;
    void sendText(const std::string &text, bool toAllies) override;
    void printText(const std::string &text) override;
    void pauseGame() override;
    void resumeGame() override;
    void leaveGame() override;
    void restartGame() override;
    void setLocalSpeed(int msPerFrame) override;
    void issueCommand(const Unitset& units, UnitCommand command) override;
    void setAlliance(int playerId, int alliance) override;
    void setVision(int playerId, bool enabled) override;
    void createUnit(int playerId, UnitType unitType, int x, int y, int count) override;
    void killUnits(const Unitset& units, bool removeInstantly) override;
    void createGameRequest(bool battlenetMap, const std::string& map, int playerCount, bool addComputer, Race race);
    void gameInfoRequest();
    void observationRequest();
    bool isRemaster() const;
    // @TODO function for debugging, delete later.
    void loadFile(Game& Broodwar);

  private:
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
    void updatePlayerUnitCounts(Game& game);

    void transmitMessages(Game& game);

    bool connected = false;

    std::set<PlayerData, IDCompare> players;
    std::set<UnitData, IDCompare> units;
    std::set<RegionData, IDCompare> regions;
    std::set<BulletData, IDCompare> bullets;
    std::set<ForceData, IDCompare> forces;

    BWAPIProtoClient protoClientBWAPI;
    SCRAPIProtoClient protoClientSCRAPI;
  };
}
