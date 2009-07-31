#pragma once

#include <list>
#include <map>
#include <set>

#include <BWAPI/Color.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/Error.h>
#include <BWAPI/Flag.h>
#include <BWAPI/Race.h>
#include <BWAPI/Order.h>
#include <BWAPI/Latency.h>
#include <BWAPI/TilePosition.h>
namespace BWAPI
{
  class Force;
  class Player;
  class Unit;
  class Game
  {
    public :
      virtual std::set< Force* > getForces() const = 0;
      virtual std::set< Player* > getPlayers() const = 0;
      virtual std::set< Unit* > getAllUnits() const = 0;
      virtual std::set< Unit* > getMinerals() const = 0;
      virtual std::set< Unit* > getGeysers() const = 0;
      virtual std::set< Unit* > getNeutralUnits() const = 0;

      virtual Latency::Enum getLatency() = 0;
      virtual int getFrameCount() const = 0;
      virtual int getMouseX() const = 0;
      virtual int getMouseY() const = 0;
      virtual int getScreenX() const = 0;
      virtual int getScreenY() const = 0;
      virtual void setScreenPosition(int x, int y) = 0;

      virtual bool isFlagEnabled(Flag::Enum flag) const = 0;
      virtual void enableFlag(Flag::Enum flag) = 0;
      virtual std::set<Unit*> unitsOnTile(int x, int y) const = 0;
      virtual Error getLastError() const = 0;

      virtual int mapWidth() const = 0;
      virtual int mapHeight() const = 0;
      virtual std::string mapFilename() const = 0;
      virtual std::string mapName() const = 0;
      virtual bool buildable(int x, int y) const = 0;
      virtual bool walkable(int x, int y) const = 0;
      virtual bool visible(int x, int y) const = 0;
      virtual bool hasCreep(int x, int y) const = 0;
      virtual bool hasPower(int x, int y, int tileWidth, int tileHeight) const = 0;
      virtual int groundHeight(int x, int y) const = 0;
      virtual const std::set< TilePosition >& getStartLocations() const = 0;
      virtual int getMapHash() const = 0;

      virtual void printf(const char* text, ...) = 0;
      virtual void sendText(const char* text, ...) = 0;
      virtual void text(CoordinateType::Enum ctype, int x, int y, const char* text, ...) = 0;
      virtual void changeRace(Race race) = 0;
      virtual bool inReplay() const = 0;
      virtual void startGame() = 0;
      virtual void pauseGame() = 0;
      virtual void resumeGame() = 0;
      virtual const std::set<Unit*>& getSelectedUnits() const = 0;
      virtual Player* self() const = 0;
      virtual Player* enemy() const = 0;
      virtual void drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid) = 0;
      virtual void drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid) = 0;
      virtual void drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid) = 0;
      virtual void drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid) = 0;
      virtual void drawDot(CoordinateType::Enum ctype, int x, int y, Color color) = 0;
      virtual void drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color) = 0;
  };
  extern Game* Broodwar;
}
