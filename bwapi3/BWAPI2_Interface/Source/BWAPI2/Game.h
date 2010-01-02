#pragma once

#include "BWAPI2Interface.h"

#include <list>
#include <map>
#include <set>

#include "Color.h"
#include "CoordinateType.h"
#include "Error.h"
#include "Flag.h"
#include "Race.h"
#include "Order.h"
#include "Latency.h"
#include "TilePosition.h"
#include "UnitType.h"
#include "TechType.h"
#include "UpgradeType.h"
#include "Input.h"
namespace BWAPI2
{
  class Force;
  class Player;
  class Unit;
  namespace Game
  {
    //public :
      BWAPI2_EXPORT std::set< Force* >& getForces();
      BWAPI2_EXPORT std::set< Player* >& getPlayers();
      BWAPI2_EXPORT std::set< Unit* >& getAllUnits();
      BWAPI2_EXPORT std::set< Unit* >& getMinerals();
      BWAPI2_EXPORT std::set< Unit* >& getGeysers();
      BWAPI2_EXPORT std::set< Unit* >& getNeutralUnits();

      BWAPI2_EXPORT std::set< Unit* >& getStaticMinerals();
      BWAPI2_EXPORT std::set< Unit* >& getStaticGeysers();
      BWAPI2_EXPORT std::set< Unit* >& getStaticNeutralUnits();

      BWAPI2_EXPORT int getLatency();
      BWAPI2_EXPORT int getFrameCount();
      BWAPI2_EXPORT int getMouseX();
      BWAPI2_EXPORT int getMouseY();
      BWAPI2_EXPORT Position getMousePosition();
      BWAPI2_EXPORT bool getMouseState(MouseButton button);
      BWAPI2_EXPORT bool getMouseState(int button);
      BWAPI2_EXPORT bool getKeyState(Key key);
      BWAPI2_EXPORT bool getKeyState(int key);
      BWAPI2_EXPORT int getScreenX();
      BWAPI2_EXPORT int getScreenY();
      BWAPI2_EXPORT Position getScreenPosition();
      BWAPI2_EXPORT void setScreenPosition(int x, int y);
      BWAPI2_EXPORT void setScreenPosition(Position p);
      BWAPI2_EXPORT void pingMinimap(int x, int y);
      BWAPI2_EXPORT void pingMinimap(Position p);

      BWAPI2_EXPORT bool isFlagEnabled(int flag);
      BWAPI2_EXPORT void enableFlag(int flag);
      BWAPI2_EXPORT std::set<Unit*>& unitsOnTile(int x, int y);

      BWAPI2_EXPORT int mapWidth();
      BWAPI2_EXPORT int mapHeight();
      BWAPI2_EXPORT std::string mapFilename();
      BWAPI2_EXPORT std::string mapName();
      BWAPI2_EXPORT int getMapHash();

      BWAPI2_EXPORT int  getGroundHeight(int x, int y);
      BWAPI2_EXPORT bool isWalkable(int x, int y);
      BWAPI2_EXPORT bool isBuildable(int x, int y);
      BWAPI2_EXPORT bool isVisible(int x, int y);
      BWAPI2_EXPORT bool isExplored(int x, int y);
      BWAPI2_EXPORT bool hasCreep(int x, int y);
      BWAPI2_EXPORT bool hasPower(int x, int y, int tileWidth, int tileHeight);

      BWAPI2_EXPORT bool isBuildable(TilePosition position);
      BWAPI2_EXPORT bool isVisible(TilePosition position);
      BWAPI2_EXPORT bool isExplored(TilePosition position);
      BWAPI2_EXPORT bool hasCreep(TilePosition position);
      BWAPI2_EXPORT bool hasPower(TilePosition position, int tileWidth, int tileHeight);

      BWAPI2_EXPORT bool canBuildHere(Unit* builder, TilePosition position, UnitType type);
      BWAPI2_EXPORT bool canMake(Unit* builder, UnitType type);
      BWAPI2_EXPORT bool canResearch(Unit* unit, TechType type);
      BWAPI2_EXPORT bool canUpgrade(Unit* unit, UpgradeType type);
      BWAPI2_EXPORT std::set< TilePosition >& getStartLocations();

      BWAPI2_EXPORT void printf(const char* text, ...);
      BWAPI2_EXPORT void sendText(const char* text, ...);
      BWAPI2_EXPORT void changeRace(Race race);
      BWAPI2_EXPORT bool isMultiplayer();
      BWAPI2_EXPORT bool isPaused();
      BWAPI2_EXPORT bool isReplay();
      BWAPI2_EXPORT void startGame();
      BWAPI2_EXPORT void pauseGame();
      BWAPI2_EXPORT void resumeGame();
      BWAPI2_EXPORT void leaveGame();
      BWAPI2_EXPORT void restartGame();
      BWAPI2_EXPORT void setLocalSpeed(int speed = -1);
      BWAPI2_EXPORT std::set<Unit*>& getSelectedUnits();
      BWAPI2_EXPORT Player* self();
      BWAPI2_EXPORT Player* enemy();

      BWAPI2_EXPORT void drawText(int ctype, int x, int y, const char* text, ...);
      BWAPI2_EXPORT void drawTextMap(int x, int y, const char* text, ...);
      BWAPI2_EXPORT void drawTextMouse(int x, int y, const char* text, ...);
      BWAPI2_EXPORT void drawTextScreen(int x, int y, const char* text, ...);

      BWAPI2_EXPORT void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);

      BWAPI2_EXPORT void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);

      BWAPI2_EXPORT void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);

      BWAPI2_EXPORT void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      BWAPI2_EXPORT void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);

      BWAPI2_EXPORT void drawDot(int ctype, int x, int y, Color color);
      BWAPI2_EXPORT void drawDotMap(int x, int y, Color color);
      BWAPI2_EXPORT void drawDotMouse(int x, int y, Color color);
      BWAPI2_EXPORT void drawDotScreen(int x, int y, Color color);

      BWAPI2_EXPORT void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color);
      BWAPI2_EXPORT void drawLineMap(int x1, int y1, int x2, int y2, Color color);
      BWAPI2_EXPORT void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
      BWAPI2_EXPORT void drawLineScreen(int x1, int y1, int x2, int y2, Color color);
  };
}
