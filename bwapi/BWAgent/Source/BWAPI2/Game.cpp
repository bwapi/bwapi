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
/*    std::set< Unit* >& getAllUnits()
    {
      // STUB
      return std::set< Unit* >();
    }*/
    //---------------------------------------------- GET LATENCY -----------------------------------------------
    int BWAPI2_CALL getLatency()
    {
      return 0;
    }
    //--------------------------------------------- GET FRAME COUNT --------------------------------------------
    int BWAPI2_CALL getFrameCount()
    {
      return 0;
    }
    //---------------------------------------------- GET MOUSE X -----------------------------------------------
    int BWAPI2_CALL getMouseX()
    {
      return 0;
    }
    //---------------------------------------------- GET MOUSE Y -----------------------------------------------
    int BWAPI2_CALL getMouseY()
    {
      return 0;
    }
    //---------------------------------------------- GET SCREEN X ----------------------------------------------
    int BWAPI2_CALL getScreenX()
    {
      return 0;
    }
    //---------------------------------------------- GET SCREEN Y ----------------------------------------------
    int BWAPI2_CALL getScreenY()
    {
      return 0;
    }
    //----------------------------------------------- MAP WIDTH ------------------------------------------------
    int BWAPI2_CALL mapWidth()
    {
      return 0;
    }
    //----------------------------------------------- MAP HEIGHT -----------------------------------------------
    int BWAPI2_CALL mapHeight()
    {
      return 0;
    }
    //---------------------------------------------- MAP FILENAME ----------------------------------------------
    const char* BWAPI2_CALL mapFilename()
    {
      return "";
    }
    //------------------------------------------------ MAP NAME ------------------------------------------------
    const char* BWAPI2_CALL mapName()
    {
      return "";
    }
    //----------------------------------------------- GET MAP HASH ---------------------------------------------
    int BWAPI2_CALL getMapHash()
    {
      return 0;
    }

    //------------------------------------------- GET GROUND HEIGHT --------------------------------------------
    int BWAPI2_CALL getGroundHeight(int x, int y)
    {
      return 0;
    }
    //---------------------------------------------- IS WALKABLE -----------------------------------------------
    bool BWAPI2_CALL isWalkable(int x, int y)
    {
      return 0;
    }
    //--------------------------------------------- IS BUILDABLE -----------------------------------------------
    bool BWAPI2_CALL isBuildable(int x, int y)
    {
      return 0;
    }
    //---------------------------------------------- IS VISIBLE ------------------------------------------------
    bool BWAPI2_CALL isVisible(int x, int y)
    {
      return 0;
    }
    //---------------------------------------------- IS EXPLORED -----------------------------------------------
    bool BWAPI2_CALL isExplored(int x, int y)
    {
      return 0;
    }
    //----------------------------------------------- HAS CREEP ------------------------------------------------
    bool BWAPI2_CALL hasCreep(int x, int y)
    {
      return 0;
    }

    //--------------------------------------------- IS MULTIPLAYER ---------------------------------------------
    bool BWAPI2_CALL isMultiplayer()
    {
      return 0;
    }
    //----------------------------------------------- IS REPLAY ------------------------------------------------
    bool BWAPI2_CALL isReplay()
    {
      return 0;
    }
    //----------------------------------------------- IS PAUSED ------------------------------------------------
    bool BWAPI2_CALL isPaused()
    {
      return 0;
    }
    //------------------------------------------------ PRINTF --------------------------------------------------
    void BWAPI2_CALL printf(const char* text, ...)
    {
      return;
    }
    //------------------------------------------------ SEND TEXT -----------------------------------------------
    void BWAPI2_CALL sendText(const char* text, ...)
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
