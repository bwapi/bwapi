#pragma once
#include "BWAPI2Interface.h"

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

IP_STRIP;

namespace BWAPI2
{
  /** singleton Game class functions */
  class Unit;
  namespace Game
  {
    //--------------------------------------- QUERIES --------------------------------------
    // Map related
    BWAPI2_FUNCTION int BWAPI2_CALL mapWidth();
    BWAPI2_FUNCTION int BWAPI2_CALL mapHeight();
    BWAPI2_FUNCTION const char* BWAPI2_CALL mapFilename();
    BWAPI2_FUNCTION const char* BWAPI2_CALL mapName();
    BWAPI2_FUNCTION int BWAPI2_CALL getMapHash();
    BWAPI2_FUNCTION int BWAPI2_CALL getGroundHeight(int x, int y);
    BWAPI2_FUNCTION bool BWAPI2_CALL isWalkable(int x, int y);
    BWAPI2_FUNCTION bool BWAPI2_CALL isBuildable(int x, int y);
    BWAPI2_FUNCTION bool BWAPI2_CALL isVisible(int x, int y);
    BWAPI2_FUNCTION bool BWAPI2_CALL isExplored(int x, int y);
    BWAPI2_FUNCTION bool BWAPI2_CALL hasCreep(int x, int y);

    // Match related
    BWAPI2_FUNCTION bool BWAPI2_CALL isMultiplayer();
    BWAPI2_FUNCTION bool BWAPI2_CALL isReplay();

    // World related
//    BWAPI2_FUNCTION void BWAPI2_CALL getAllUnits(); // TODO: return type

    // GUI related
    BWAPI2_FUNCTION int BWAPI2_CALL getMouseX();
    BWAPI2_FUNCTION int BWAPI2_CALL getMouseY();
    BWAPI2_FUNCTION int BWAPI2_CALL getScreenX();
    BWAPI2_FUNCTION int BWAPI2_CALL getScreenY();
    BWAPI2_FUNCTION int BWAPI2_CALL getLatency();
    BWAPI2_FUNCTION int BWAPI2_CALL getFrameCount();
    BWAPI2_FUNCTION bool BWAPI2_CALL isPaused();



    //--------------------------------------- INSTRUCTIONS ---------------------------------
//    BWAPI2_FUNCTION void BWAPI2_CALL printf(const char* text, ...); // TODO: find better name. overwrites the printf from stdio.h
    BWAPI2_FUNCTION void BWAPI2_CALL sendText(const char* text, ...);

/* TODO: uncomment, create STUBs so it compiles right
    BWAPI2_FUNCTION std::set< Force* >&  getForces();
    BWAPI2_FUNCTION std::set< Player* >&  getPlayers();
    BWAPI2_FUNCTION std::set< Unit* >&  getAllUnits();
    BWAPI2_FUNCTION std::set< Unit* >&  getMinerals();
    BWAPI2_FUNCTION std::set< Unit* >&  getGeysers();
    BWAPI2_FUNCTION std::set< Unit* >&  getNeutralUnits();

    BWAPI2_FUNCTION std::set< Unit* >& getStaticMinerals();
    BWAPI2_FUNCTION std::set< Unit* >& getStaticGeysers();
    BWAPI2_FUNCTION std::set< Unit* >& getStaticNeutralUnits();


    BWAPI2_FUNCTION bool  isFlagEnabled(int flag);
    BWAPI2_FUNCTION void  enableFlag(int flag);
    BWAPI2_FUNCTION std::set<Unit*>& unitsOnTile(int x, int y);
    BWAPI2_FUNCTION Error  getLastError() const;


    BWAPI2_FUNCTION bool hasPower(int x, int y, int tileWidth, int tileHeight);

    BWAPI2_FUNCTION bool isBuildable(TilePosition position);
    BWAPI2_FUNCTION bool isVisible(TilePosition position);
    BWAPI2_FUNCTION bool isExplored(TilePosition position);
    BWAPI2_FUNCTION bool hasCreep(TilePosition position);
    BWAPI2_FUNCTION bool hasPower(TilePosition position, int tileWidth, int tileHeight);

    BWAPI2_FUNCTION bool canBuildHere(Unit* builder, TilePosition position, UnitType type);
    BWAPI2_FUNCTION bool canMake(Unit* builder, UnitType type);
    BWAPI2_FUNCTION bool canResearch(Unit* unit, TechType type);
    BWAPI2_FUNCTION bool canUpgrade(Unit* unit, UpgradeType type);
    BWAPI2_FUNCTION std::set< TilePosition >& getStartLocations();

    /**
     * Changes race in the pre-game lobby.
     * @param race Desired race of the slot (Zerg/Protoss/Terran/Random)
     */
 /*   BWAPI2_FUNCTION void changeRace(BWAPI::Race race);
    BWAPI2_FUNCTION bool isMultiplayer();
    BWAPI2_FUNCTION bool isPaused();
    BWAPI2_FUNCTION bool isReplay();
    /**
     * Starts the game in the pre-game lobby. Should be used only in the
     * pre-game lobby, and not during counting
     */
 /*   BWAPI2_FUNCTION void startGame();
    BWAPI2_FUNCTION void pauseGame();
    BWAPI2_FUNCTION void resumeGame();
    BWAPI2_FUNCTION void leaveGame();
    BWAPI2_FUNCTION void restartGame();
    BWAPI2_FUNCTION void setLocalSpeed(int speed = -1);
    BWAPI2_FUNCTION std::set<BWAPI::Unit*>& getSelectedUnits();
    BWAPI2_FUNCTION Player* self();
    BWAPI2_FUNCTION Player* enemy();

    BWAPI2_FUNCTION void drawText(int ctype, int x, int y, const char* text, ...);
    BWAPI2_FUNCTION void drawTextMap(int x, int y, const char* text, ...);
    BWAPI2_FUNCTION void drawTextMouse(int x, int y, const char* text, ...);
    BWAPI2_FUNCTION void drawTextScreen(int x, int y, const char* text, ...);

    BWAPI2_FUNCTION void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid = false);

    BWAPI2_FUNCTION void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid = false);

    BWAPI2_FUNCTION void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawCircleMap(int x, int y, int radius, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid = false);

    BWAPI2_FUNCTION void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);
    BWAPI2_FUNCTION void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid = false);

    BWAPI2_FUNCTION void drawDot(int ctype, int x, int y, Color color);
    BWAPI2_FUNCTION void drawDotMap(int x, int y, Color color);
    BWAPI2_FUNCTION void drawDotMouse(int x, int y, Color color);
    BWAPI2_FUNCTION void drawDotScreen(int x, int y, Color color);

    BWAPI2_FUNCTION void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color);
    BWAPI2_FUNCTION void drawLineMap(int x1, int y1, int x2, int y2, Color color);
    BWAPI2_FUNCTION void drawLineMouse(int x1, int y1, int x2, int y2, Color color);
    BWAPI2_FUNCTION void drawLineScreen(int x1, int y1, int x2, int y2, Color color);

    BWAPI2_FUNCTION bool gluMessageBox(char* message, int type);
    BWAPI2_FUNCTION bool gluEditBox(char* message, char* dest, size_t destsize, char* restricted);
    */
  }
}
