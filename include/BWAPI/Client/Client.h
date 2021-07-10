#pragma once

#include <string>

#include <BWAPI/CoordinateType.h>
#include <BWAPI/Color.h>
#include <BWAPI/Unitset.h>
#include <BWAPI/UnitCommand.h>

#include "ShapeType.h"

namespace BWAPI
{
  class Game;

  class Client
  {
  public:
    virtual ~Client() {}

    virtual bool isConnected() const = 0;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;

    virtual void update(Game& game) = 0;

    virtual void setScreenPosition(int x, int y) = 0;
    virtual void pingMinimap(int x, int y) = 0;
    virtual void drawShape(ShapeType::Enum shapeType, CoordinateType::Enum coordinateType, int x1, int y1, int x2, int y2, int extra1, int extra2, Color color, bool isSolid) = 0;
    virtual void drawText(CoordinateType::Enum coordinateType, const std::string &text, int x, int y, int textSize) = 0;
    virtual void sendText(const std::string &text, bool toAllies) = 0;
    virtual void printText(const std::string &text) = 0;
    virtual void pauseGame() = 0;
    virtual void resumeGame() = 0;
    virtual void leaveGame() = 0;
    virtual void restartGame() = 0;
    virtual void setLocalSpeed(int msPerFrame) = 0;
    virtual void issueCommand(const Unitset& units, UnitCommand command) = 0;
    virtual void setAlliance(int playerId, int alliance) = 0;  // 0 = enemy, 1 = allied, 2 = allied victory; @TODO add enum for this
    virtual void setVision(int playerId, bool enabled) = 0;

    virtual void createUnit(int playerId, UnitType unitType, int x, int y, int count) = 0;
    virtual void killUnits(const Unitset& units, bool removeInstantly) = 0;

    // TODO: setFrameSkip, setRevealAll, setMap, setGUI
  };
}
