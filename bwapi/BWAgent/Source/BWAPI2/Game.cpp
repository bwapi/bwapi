#include "Game.h"

#include "Unit.h"

#include <Util\Version.h>
#include <Util\Buffer.h>

#include <string>

namespace BWAPI2
{
  namespace Game
  {
    //------------------------------------------------- GET UNITS ----------------------------------------------
    std::set< Unit* >& getAllUnits()
    {
      // STUB
      return std::set< Unit* >();
    }
    //---------------------------------------------- GET LATENCY -----------------------------------------------
    int getLatency()
    {
      return 0;
    }
    //--------------------------------------------- GET FRAME COUNT --------------------------------------------
    int getFrameCount()
    {
      return 0;
    }
    //---------------------------------------------- GET MOUSE X -----------------------------------------------
    int getMouseX()
    {
      return 0;
    }
    //---------------------------------------------- GET MOUSE Y -----------------------------------------------
    int getMouseY()
    {
      return 0;
    }
    //---------------------------------------------- GET SCREEN X ----------------------------------------------
    int getScreenX()
    {
      return 0;
    }
    //---------------------------------------------- GET SCREEN Y ----------------------------------------------
    int getScreenY()
    {
      return 0;
    }
    //----------------------------------------------- MAP WIDTH ------------------------------------------------
    int mapWidth()
    {
      return 0;
    }
    //----------------------------------------------- MAP HEIGHT -----------------------------------------------
    int mapHeight()
    {
      return 0;
    }
    //---------------------------------------------- MAP FILENAME ----------------------------------------------
    std::string mapFilename()
    {
      return "";
    }
    //------------------------------------------------ MAP NAME ------------------------------------------------
    std::string mapName()
    {
      return "";
    }
    //----------------------------------------------- GET MAP HASH ---------------------------------------------
    int getMapHash()
    {
      return 0;
    }

    //------------------------------------------- GET GROUND HEIGHT --------------------------------------------
    int getGroundHeight(int x, int y)
    {
      return 0;
    }
    //---------------------------------------------- IS WALKABLE -----------------------------------------------
    bool isWalkable(int x, int y)
    {
      return 0;
    }
    //--------------------------------------------- IS BUILDABLE -----------------------------------------------
    bool isBuildable(int x, int y)
    {
      return 0;
    }
    //---------------------------------------------- IS VISIBLE ------------------------------------------------
    bool isVisible(int x, int y)
    {
      return 0;
    }
    //---------------------------------------------- IS EXPLORED -----------------------------------------------
    bool isExplored(int x, int y)
    {
      return 0;
    }
    //----------------------------------------------- HAS CREEP ------------------------------------------------
    bool hasCreep(int x, int y)
    {
      return 0;
    }

    //--------------------------------------------- IS MULTIPLAYER ---------------------------------------------
    bool isMultiplayer()
    {
      return 0;
    }
    //----------------------------------------------- IS REPLAY ------------------------------------------------
    bool isReplay()
    {
      return 0;
    }
    //----------------------------------------------- IS PAUSED ------------------------------------------------
    bool isPaused()
    {
      return 0;
    }
    //------------------------------------------------ PRINTF --------------------------------------------------
    void printf(const char* text, ...)
    {
      return;
    }
    //------------------------------------------------ SEND TEXT -----------------------------------------------
    void sendText(const char* text, ...)
    {
      return;
    }
    /*
    //------------------------------------------------ BUILDABLE -----------------------------------------------
    bool  isBuildable(TilePosition position)
    {
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool  isVisible(TilePosition position)
    {
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool  isExplored(TilePosition position)
    {
    }
    //------------------------------------------------ HAS CREEP -----------------------------------------------
    bool  hasCreep(TilePosition position)
    {
    }
    //------------------------------------------------ HAS POWER -----------------------------------------------
    bool  hasPower(int x, int y, int tileWidth, int tileHeight)
    {
    }
    //------------------------------------------------ HAS POWER -----------------------------------------------
    bool  hasPower(TilePosition position, int tileWidth, int tileHeight)
    {
    }
    //---------------------------------------------- CAN BUILD HERE --------------------------------------------
    bool  canBuildHere(Unit* builder, TilePosition position, UnitType type)
    {
    }
    //------------------------------------------------- CAN MAKE -----------------------------------------------
    bool  canMake(Unit* builder, UnitType type)
    {
    }
    //----------------------------------------------- CAN RESEARCH ---------------------------------------------
    bool  canResearch(Unit* unit, TechType type)
    {
    }
    //----------------------------------------------- CAN UPGRADE ----------------------------------------------
    bool  canUpgrade(Unit* unit, UpgradeType type)
    {
    }
    //--------------------------------------------- GET START LOCATIONS ----------------------------------------
    std::set< TilePosition >& getStartLocations()
    {
    }
    //----------------------------------------------- GET FORCES -----------------------------------------------
    std::set< Force* >& getForces()
    {
    }
    //----------------------------------------------- GET PLAYERS ----------------------------------------------
    std::set< Player* >& getPlayers()
    {
    }
    //---------------------------------------------- GET MINERALS ----------------------------------------------
    std::set< Unit* >& getMinerals()
    {
    }
    //---------------------------------------------- GET GEYSERS -----------------------------------------------
    std::set< Unit* >& getGeysers()
    {
    }
    //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
    std::set< Unit* >& getNeutralUnits()
    {
    }
    //---------------------------------------------- GET MINERALS ----------------------------------------------
    std::set< Unit* >& getStaticMinerals()
    {
    }
    //---------------------------------------------- GET GEYSERS -----------------------------------------------
    std::set< Unit* >& getStaticGeysers()
    {
    }
    //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
    std::set< Unit* >& getStaticNeutralUnits()
    {
    }
    //------------------------------------------------ IN GAME -------------------------------------------------
    bool inGame() const
    {
    }
    //---------------------------------------------- CHANGE SLOT -----------------------------------------------
    void changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
    {
    }
    //---------------------------------------------- CHANGE RACE -----------------------------------------------
    void  changeRace(BWAPI::Race race)
    {
    }
    //----------------------------------------------- START GAME -----------------------------------------------
    void  startGame()
    {
    }
    //----------------------------------------------- PAUSE GAME -----------------------------------------------
    void  pauseGame()
    {
    }
    //---------------------------------------------- RESUME GAME -----------------------------------------------
    void  resumeGame()
    {
    }
    //---------------------------------------------- LEAVE GAME ------------------------------------------------
    void  leaveGame()
    {
    }
    //--------------------------------------------- RESTART GAME -----------------------------------------------
    void  restartGame()
    {
    }
    //------------------------------------------- SET SCREEN POSITION ------------------------------------------
    void setScreenPosition(int x, int y)
    {
    }
    //------------------------------------------ GET SELECTED UNITS --------------------------------------------
    std::set<BWAPI::Unit*>& getSelectedUnits()
    {
    }
    //--------------------------------------------- UNITS ON TILE ----------------------------------------------
    std::set<Unit*>& unitsOnTile(int x, int y)
    {
    }
    //--------------------------------------------- GET LAST ERROR ---------------------------------------------
    Error  getLastError() const
    {
    }
    //--------------------------------------------- IS FLAG ENABLED --------------------------------------------
    bool  isFlagEnabled(int flag)
    {
    }
    //----------------------------------------------- ENABLE FLAG ----------------------------------------------
    void  enableFlag(int flag)
    {
    }
    //----------------------------------------------------- SELF -----------------------------------------------
    Player*  self()
    {
    }
    //----------------------------------------------------- ENEMY ----------------------------------------------
    Player*  enemy()
    {
    }
    //----------------------------------------------------- DRAW -----------------------------------------------
    void  drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
    {
    }
    void  drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
    }
    void  drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
    }
    void  drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
    }

    void  drawDot(int ctype, int x, int y, Color color)
    {
    }
    void  drawDotMap(int x, int y, Color color)
    {
    }
    void  drawDotMouse(int x, int y, Color color)
    {
    }
    void  drawDotScreen(int x, int y, Color color)
    {
    }

    void  drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
    {
    }
    void  drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
    {
    }
    void  drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
    {
    }
    void  drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
    {
    }

    void  drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
    }
    void  drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
    }
    void  drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
    }
    void  drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
    }

    void  drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
    {
    }
    void  drawLineMap(int x1, int y1, int x2, int y2, Color color)
    {
    }
    void  drawLineMouse(int x1, int y1, int x2, int y2, Color color)
    {
    }
    void  drawLineScreen(int x1, int y1, int x2, int y2, Color color)
    {
    }

    void  drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
    }
    void  drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
    }
    void  drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
    }
    void  drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
    }

    void  drawText(int ctype, int x, int y, const char* text, ...)
    {
    }
    void  drawTextMap(int x, int y, const char* text, ...)
    {
    }
    void  drawTextMouse(int x, int y, const char* text, ...)
    {
    }
    void  drawTextScreen(int x, int y, const char* text, ...)
    {
    }

    //--------------------------------------------------- GAME SPEED -------------------------------------------
    void  setLocalSpeed(int speed)
    {
    }
    */
  };
};
