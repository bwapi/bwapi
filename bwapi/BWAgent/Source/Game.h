#pragma once
#include "BWAgentInterface.h"

namespace Util  { class Logger; }
namespace BWAPI { class Player; }
namespace BWAPI { class PlayerImpl; }
namespace BWAPI { class Unit; }
namespace BWAPI { class UnitImpl; }
namespace BWAPI { class Command; }
namespace BWAPI { class AIModule; }


#include <vector>
#include <list>
#include <map>
#include <set>
#include <windows.h>

namespace BWAgent
{
  /** singleton Game class functions */
  namespace Game
  {
    AGENT_API int getMouseX();
    AGENT_API int getMouseY();
/*    AGENT_API std::set< Force* >&  getForces();
    AGENT_API std::set< Player* >&  getPlayers();
    AGENT_API std::set< Unit* >&  getAllUnits();
    AGENT_API std::set< Unit* >&  getMinerals();
    AGENT_API std::set< Unit* >&  getGeysers();
    AGENT_API std::set< Unit* >&  getNeutralUnits();

    AGENT_API std::set< Unit* >& getStaticMinerals();
    AGENT_API std::set< Unit* >& getStaticGeysers();
    AGENT_API std::set< Unit* >& getStaticNeutralUnits();

    AGENT_API int getLatency();
    AGENT_API int getFrameCount();
    AGENT_API int getMouseX();
    AGENT_API int getMouseY();
    AGENT_API int getScreenX();
    AGENT_API int getScreenY();

    AGENT_API bool  isFlagEnabled(int flag);
    AGENT_API void  enableFlag(int flag);
    AGENT_API std::set<Unit*>& unitsOnTile(int x, int y);
    AGENT_API Error  getLastError() const;

    AGENT_API int  mapWidth();
    AGENT_API int  mapHeight();
    AGENT_API std::string  mapFilename();
    AGENT_API std::string  mapName();
    AGENT_API int getMapHash();

    AGENT_API int  getGroundHeight(int x, int y);
    AGENT_API bool isWalkable(int x, int y);
    AGENT_API bool isBuildable(int x, int y);
    AGENT_API bool isVisible(int x, int y);
    AGENT_API bool isExplored(int x, int y);
    AGENT_API bool hasCreep(int x, int y);
    AGENT_API bool hasPower(int x, int y, int tileWidth, int tileHeight);


    AGENT_API bool isBuildable(TilePosition position);
    AGENT_API bool isVisible(TilePosition position);
    AGENT_API bool isExplored(TilePosition position);
    AGENT_API bool hasCreep(TilePosition position);
    AGENT_API bool hasPower(TilePosition position, int tileWidth, int tileHeight);

    AGENT_API bool canBuildHere(Unit* builder, TilePosition position, UnitType type);
    AGENT_API bool canMake(Unit* builder, UnitType type);
    AGENT_API bool canResearch(Unit* unit, TechType type);
    AGENT_API bool canUpgrade(Unit* unit, UpgradeType type);
    AGENT_API std::set< TilePosition >& getStartLocations();
    /**
     * Prints text in game (only local)
     * @param text Text to be written
     */
/*    AGENT_API void printf(const char* text, ...);
    AGENT_API void sendText(const char* text, ...);

    /**
     * Changes race in the pre-game lobby.
     * @param race Desired race of the slot (Zerg/Protoss/Terran/Random)
     */
 /*   AGENT_API void changeRace(BWAPI::Race race);
    AGENT_API bool isMultiplayer();
    AGENT_API bool isPaused();
    AGENT_API bool isReplay();
    /**
     * Starts the game in the pre-game lobby. Should be used only in the
     * pre-game lobby, and not during counting
     */
 /*   AGENT_API void startGame();
    AGENT_API void pauseGame();
    AGENT_API void resumeGame();
    AGENT_API void leaveGame();
    AGENT_API void restartGame();
    AGENT_API void setLocalSpeed(int speed = -1);
    AGENT_API std::set<BWAPI::Unit*>& getSelectedUnits();
    AGENT_API Player* self();
    AGENT_API Player* enemy();

    AGENT_API void drawText(int ctype, int x, int y, const char* text, ...);
    AGENT_API void drawTextMap(int x, int y, const char* text, ...);
    AGENT_API void drawTextMouse(int x, int y, const char* text, ...);
    AGENT_API void drawTextScreen(int x, int y, const char* text, ...);

    AGENT_API void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
    AGENT_API void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    AGENT_API void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    AGENT_API void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);

    AGENT_API void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    AGENT_API void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    AGENT_API void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    AGENT_API void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);

    AGENT_API void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid = false);
    AGENT_API void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
    AGENT_API void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
    AGENT_API void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);

    AGENT_API void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    AGENT_API void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    AGENT_API void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    AGENT_API void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);

    AGENT_API void drawDot(int ctype, int x, int y, Color color);
    AGENT_API void drawDotMap(int x, int y, Color color);
    AGENT_API void drawDotMouse(int x, int y, Color color);
    AGENT_API void drawDotScreen(int x, int y, Color color);

    AGENT_API void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color);
    AGENT_API void drawLineMap(int x1, int y1, int x2, int y2, Color color);
    AGENT_API void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
    AGENT_API void drawLineScreen(int x1, int y1, int x2, int y2, Color color);

    AGENT_API bool gluMessageBox(char* message, int type);
    AGENT_API bool gluEditBox(char* message, char* dest, size_t destsize, char* restricted);
    */
  }
}
