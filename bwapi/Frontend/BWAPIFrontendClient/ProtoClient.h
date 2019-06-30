#pragma once
#include <BWAPI/Client/Client.h>
#include <BWAPI/Client/GameData.h>
#include <BWAPI/Game.h>
#include <map>
#include "../Backend/BWAPIBackendCore/BWAPIProtoClient.h"

namespace BWAPI
{
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
  private:
    void queueMessage(std::unique_ptr<bwapi::message::Message> message);

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

    bool connected = false;

    std::set<PlayerData, IDCompare> players;
    std::set<UnitData, IDCompare> units;
    std::set<RegionData, IDCompare> regions;
    std::set<BulletData, IDCompare> bullets;
    std::set<ForceData, IDCompare> forces;

    BWAPIProtoClient protoClient;

  };
}
