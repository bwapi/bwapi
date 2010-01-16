#pragma once

#include "BWSLInterface.h"
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

#include <list>
#include <map>
#include <set>

namespace BWSL
{
  class Force;
  class Player;
  class Unit;
  namespace Game
  {
    //private :
      void onStart();
      void onFrame();
      std::set<Unit*>& getPlayerUnits(const Player* player);
    //public :
      BWSL_EXPORT std::set< Force* >& getForces();
      BWSL_EXPORT std::set< Player* >& getPlayers();
      BWSL_EXPORT std::set< Unit* >& getAllUnits();
      BWSL_EXPORT std::set< Unit* >& getMinerals();
      BWSL_EXPORT std::set< Unit* >& getGeysers();
      BWSL_EXPORT std::set< Unit* >& getNeutralUnits();

      BWSL_EXPORT std::set< Unit* >& getStaticMinerals();
      BWSL_EXPORT std::set< Unit* >& getStaticGeysers();
      BWSL_EXPORT std::set< Unit* >& getStaticNeutralUnits();

      BWSL_EXPORT Force* getForce(int forceId);
      BWSL_EXPORT Player* getPlayer(int playerId);
      BWSL_EXPORT Unit* getUnit(int unitId);

      BWSL_EXPORT int getLatency();
      BWSL_EXPORT int getFrameCount();
      BWSL_EXPORT int getMouseX();
      BWSL_EXPORT int getMouseY();
      BWSL_EXPORT Position getMousePosition();
      BWSL_EXPORT bool getMouseState(MouseButton button);
      BWSL_EXPORT bool getMouseState(int button);
      BWSL_EXPORT bool getKeyState(Key key);
      BWSL_EXPORT bool getKeyState(int key);
      BWSL_EXPORT int getScreenX();
      BWSL_EXPORT int getScreenY();
      BWSL_EXPORT Position getScreenPosition();
      BWSL_EXPORT void setScreenPosition(int x, int y);
      BWSL_EXPORT void setScreenPosition(Position p);
      BWSL_EXPORT void pingMinimap(int x, int y);
      BWSL_EXPORT void pingMinimap(Position p);

      BWSL_EXPORT bool isFlagEnabled(int flag);
      BWSL_EXPORT void enableFlag(int flag);
      BWSL_EXPORT std::set<Unit*>& unitsOnTile(int x, int y);

      BWSL_EXPORT int mapWidth();
      BWSL_EXPORT int mapHeight();
      BWSL_EXPORT std::string mapFilename();
      BWSL_EXPORT std::string mapName();
      BWSL_EXPORT int getMapHash();

      BWSL_EXPORT int  getGroundHeight(int x, int y);
      BWSL_EXPORT bool isWalkable(int x, int y);
      BWSL_EXPORT bool isBuildable(int x, int y);
      BWSL_EXPORT bool isVisible(int x, int y);
      BWSL_EXPORT bool isExplored(int x, int y);
      BWSL_EXPORT bool hasCreep(int x, int y);
      BWSL_EXPORT bool hasPower(int x, int y, int tileWidth, int tileHeight);

      BWSL_EXPORT bool isBuildable(TilePosition position);
      BWSL_EXPORT bool isVisible(TilePosition position);
      BWSL_EXPORT bool isExplored(TilePosition position);
      BWSL_EXPORT bool hasCreep(TilePosition position);
      BWSL_EXPORT bool hasPower(TilePosition position, int tileWidth, int tileHeight);

      BWSL_EXPORT bool canBuildHere(Unit* builder, TilePosition position, UnitType type);
      BWSL_EXPORT bool canMake(Unit* builder, UnitType type);
      BWSL_EXPORT bool canResearch(Unit* unit, TechType type);
      BWSL_EXPORT bool canUpgrade(Unit* unit, UpgradeType type);
      BWSL_EXPORT std::set< TilePosition >& getStartLocations();

      BWSL_EXPORT void printf(const char* text, ...);
      BWSL_EXPORT void sendText(const char* text, ...);
      BWSL_EXPORT void changeRace(Race race);
      BWSL_EXPORT bool isMultiplayer();
      BWSL_EXPORT bool isPaused();
      BWSL_EXPORT bool isReplay();
      BWSL_EXPORT void startGame();
      BWSL_EXPORT void pauseGame();
      BWSL_EXPORT void resumeGame();
      BWSL_EXPORT void leaveGame();
      BWSL_EXPORT void restartGame();
      BWSL_EXPORT void setLocalSpeed(int speed = -1);
      BWSL_EXPORT std::set<Unit*>& getSelectedUnits();
      BWSL_EXPORT Player* self();
      BWSL_EXPORT Player* enemy();

      BWSL_EXPORT void drawText(int ctype, int x, int y, const char* text, ...);
      BWSL_EXPORT void drawTextMap(int x, int y, const char* text, ...);
      BWSL_EXPORT void drawTextMouse(int x, int y, const char* text, ...);
      BWSL_EXPORT void drawTextScreen(int x, int y, const char* text, ...);

      BWSL_EXPORT void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
      BWSL_EXPORT void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      BWSL_EXPORT void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
      BWSL_EXPORT void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);

      BWSL_EXPORT void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      BWSL_EXPORT void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      BWSL_EXPORT void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
      BWSL_EXPORT void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);

      BWSL_EXPORT void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid = false);
      BWSL_EXPORT void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
      BWSL_EXPORT void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
      BWSL_EXPORT void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);

      BWSL_EXPORT void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      BWSL_EXPORT void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      BWSL_EXPORT void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
      BWSL_EXPORT void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);

      BWSL_EXPORT void drawDot(int ctype, int x, int y, Color color);
      BWSL_EXPORT void drawDotMap(int x, int y, Color color);
      BWSL_EXPORT void drawDotMouse(int x, int y, Color color);
      BWSL_EXPORT void drawDotScreen(int x, int y, Color color);

      BWSL_EXPORT void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color);
      BWSL_EXPORT void drawLineMap(int x1, int y1, int x2, int y2, Color color);
      BWSL_EXPORT void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
      BWSL_EXPORT void drawLineScreen(int x1, int y1, int x2, int y2, Color color);
  };
}
