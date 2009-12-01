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
#include <BWAPI/UnitType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UpgradeType.h>
namespace BWAPI
{
  class Force;
  class Player;
  class Unit;
  class Game
  {
    public :
      virtual std::set< Force* >& getForces() = 0;
      virtual std::set< Player* >& getPlayers() = 0;
      virtual std::set< Unit* >& getAllUnits() = 0;
      virtual std::set< Unit* >& getMinerals() = 0;
      virtual std::set< Unit* >& getGeysers() = 0;
      virtual std::set< Unit* >& getNeutralUnits() = 0;

      virtual std::set< Unit* >& getStaticMinerals() = 0;
      virtual std::set< Unit* >& getStaticGeysers() = 0;
      virtual std::set< Unit* >& getStaticNeutralUnits() = 0;

      virtual int getLatency() = 0;
      virtual int getFrameCount() = 0;
      virtual int getMouseX() = 0;
      virtual int getMouseY() = 0;
      virtual int getScreenX() = 0;
      virtual int getScreenY() = 0;

      virtual bool isFlagEnabled(int flag) = 0;
      virtual void enableFlag(int flag) = 0;
      virtual std::set<Unit*>& unitsOnTile(int x, int y) = 0;
      virtual Error getLastError() const = 0;

      virtual int mapWidth() = 0;
      virtual int mapHeight() = 0;
      virtual std::string mapFilename() = 0;
      virtual std::string mapName() = 0;
      virtual int getMapHash() = 0;

      virtual int  getGroundHeight(int x, int y) = 0;
      virtual bool isWalkable(int x, int y) = 0;
      virtual bool isBuildable(int x, int y) = 0;
      virtual bool isVisible(int x, int y) = 0;
      virtual bool isExplored(int x, int y) = 0;
      virtual bool hasCreep(int x, int y) = 0;
      virtual bool hasPower(int x, int y, int tileWidth, int tileHeight) = 0;

      virtual bool isBuildable(TilePosition position) = 0;
      virtual bool isVisible(TilePosition position) = 0;
      virtual bool isExplored(TilePosition position) = 0;
      virtual bool hasCreep(TilePosition position) = 0;
      virtual bool hasPower(TilePosition position, int tileWidth, int tileHeight) = 0;

      virtual bool canBuildHere(Unit* builder, TilePosition position, UnitType type) = 0;
      virtual bool canMake(Unit* builder, UnitType type) = 0;
      virtual bool canResearch(Unit* unit, TechType type) = 0;
      virtual bool canUpgrade(Unit* unit, UpgradeType type) = 0;
      virtual std::set< TilePosition >& getStartLocations() = 0;

      virtual void printf(const char* text, ...) = 0;
      virtual void sendText(const char* text, ...) = 0;
      virtual void changeRace(Race race) = 0;
      virtual bool isMultiplayer() = 0;
      virtual bool isPaused() = 0;
      virtual bool isReplay() = 0;
      virtual void startGame() = 0;
      virtual void pauseGame() = 0;
      virtual void resumeGame() = 0;
      virtual void leaveGame() = 0;
      virtual void restartGame() = 0;
      virtual void setLocalSpeed(int speed = -1) = 0;
      virtual std::set<Unit*>& getSelectedUnits() = 0;
      virtual Player* self() = 0;
      virtual Player* enemy() = 0;

      virtual void drawText(int ctype, int x, int y, const char* text, ...) = 0;
      virtual void drawTextMap(int x, int y, const char* text, ...) = 0;
      virtual void drawTextMouse(int x, int y, const char* text, ...) = 0;
      virtual void drawTextScreen(int x, int y, const char* text, ...) = 0;

      virtual void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false) = 0;
      virtual void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false) = 0;
      virtual void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false) = 0;
      virtual void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false) = 0;

      virtual void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) = 0;
      virtual void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) = 0;
      virtual void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) = 0;
      virtual void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false) = 0;

      virtual void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid = false) = 0;
      virtual void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false) = 0;
      virtual void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false) = 0;
      virtual void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false) = 0;

      virtual void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false) = 0;
      virtual void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false) = 0;
      virtual void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false) = 0;
      virtual void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false) = 0;

      virtual void drawDot(int ctype, int x, int y, Color color) = 0;
      virtual void drawDotMap(int x, int y, Color color) = 0;
      virtual void drawDotMouse(int x, int y, Color color) = 0;
      virtual void drawDotScreen(int x, int y, Color color) = 0;

      virtual void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color) = 0;
      virtual void drawLineMap(int x1, int y1, int x2, int y2, Color color) = 0;
      virtual void drawLineMouse(int x1, int y1, int x2, int y2, Color color) = 0;
      virtual void drawLineScreen(int x1, int y1, int x2, int y2, Color color) = 0;
  };
}
