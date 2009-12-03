//#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "Game.h"
#include "BridgeClient.h"
#include "Mappings.h"

#include <string>

#include "Util\Version.h"


namespace BWAgent
{
  namespace Game
  {
    //------------------------------------------------- GET UNITS ----------------------------------------------
    std::set< Unit* >& getAllUnits()
    {
      return BWAgent::allUnits;
    }
    //---------------------------------------------- GET LATENCY -----------------------------------------------
    int getLatency()
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      return BridgeClient::sharedStaticData->getLatency;
    }
    //--------------------------------------------- GET FRAME COUNT --------------------------------------------
    int getFrameCount()
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      return BridgeClient::sharedStaticData->frameCount;
    }
    //---------------------------------------------- GET MOUSE X -----------------------------------------------
    int getMouseX()
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      return BridgeClient::sharedStaticData->getMouseX;
    }
    //---------------------------------------------- GET MOUSE Y -----------------------------------------------
    int getMouseY()
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      return BridgeClient::sharedStaticData->getMouseY;
    }
    //---------------------------------------------- GET SCREEN X ----------------------------------------------
    int getScreenX()
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      return BridgeClient::sharedStaticData->getScreenX;
    }
    //---------------------------------------------- GET SCREEN Y ----------------------------------------------
    int getScreenY()
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      return BridgeClient::sharedStaticData->getScreenY;
    }
    //----------------------------------------------- MAP WIDTH ------------------------------------------------
    int mapWidth()
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      return BridgeClient::sharedStaticData->mapWidth;
    }
    //----------------------------------------------- MAP HEIGHT -----------------------------------------------
    int mapHeight()
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      return BridgeClient::sharedStaticData->mapHeight;
    }
    //---------------------------------------------- MAP FILENAME ----------------------------------------------
    std::string mapFilename()
    {
      if(!BridgeClient::sharedStaticData)
        return "";
      return BridgeClient::sharedStaticData->mapFilename;
    }
    //------------------------------------------------ MAP NAME ------------------------------------------------
    std::string mapName()
    {
      if(!BridgeClient::sharedStaticData)
        return "";
      return BridgeClient::sharedStaticData->mapName;
    }
    //----------------------------------------------- GET MAP HASH ---------------------------------------------
    int getMapHash()
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      return BridgeClient::sharedStaticData->mapHash;
    }

    //------------------------------------------- GET GROUND HEIGHT --------------------------------------------
    int getGroundHeight(int x, int y)
    {
      if(!BridgeClient::sharedStaticData)
        return -1;
      if (x<0 || y<0 || x>=BridgeClient::sharedStaticData->mapWidth*4 || y>=BridgeClient::sharedStaticData->mapHeight*4)
        return 0;
      return BridgeClient::sharedStaticData->getGroundHeight[x][y];
    }
    //---------------------------------------------- IS WALKABLE -----------------------------------------------
    bool isWalkable(int x, int y)
    {
      if(!BridgeClient::sharedStaticData)
        return false;
      if (x<0 || y<0 || x>=BridgeClient::sharedStaticData->mapWidth*4 || y>=BridgeClient::sharedStaticData->mapHeight*4)
        return false;
      return BridgeClient::sharedStaticData->isWalkable[x][y];
    }
    //--------------------------------------------- IS BUILDABLE -----------------------------------------------
    bool isBuildable(int x, int y)
    {
      if(!BridgeClient::sharedStaticData)
        return false;
      if (x<0 || y<0 || x>=BridgeClient::sharedStaticData->mapWidth || y>=BridgeClient::sharedStaticData->mapHeight)
        return false;
      return BridgeClient::sharedStaticData->isBuildable[x][y];
    }
    //---------------------------------------------- IS VISIBLE ------------------------------------------------
    bool isVisible(int x, int y)
    {
      if(!BridgeClient::sharedStaticData)
        return false;
      if (x<0 || y<0 || x>=BridgeClient::sharedStaticData->mapWidth || y>=BridgeClient::sharedStaticData->mapHeight)
        return false;
      return BridgeClient::sharedStaticData->isVisible[x][y];
    }
    //---------------------------------------------- IS EXPLORED -----------------------------------------------
    bool isExplored(int x, int y)
    {
      if(!BridgeClient::sharedStaticData)
        return false;
      if (x<0 || y<0 || x>=BridgeClient::sharedStaticData->mapWidth || y>=BridgeClient::sharedStaticData->mapHeight)
        return false;
      return BridgeClient::sharedStaticData->isExplored[x][y];
    }
    //----------------------------------------------- HAS CREEP ------------------------------------------------
    bool hasCreep(int x, int y)
    {
      if(!BridgeClient::sharedStaticData)
        return false;
      if (x<0 || y<0 || x>=BridgeClient::sharedStaticData->mapWidth || y>=BridgeClient::sharedStaticData->mapHeight)
        return false;
      return BridgeClient::sharedStaticData->hasCreep[x][y];
    }

    //--------------------------------------------- IS MULTIPLAYER ---------------------------------------------
    bool isMultiplayer()
    {
      if(!BridgeClient::sharedStaticData)
        return false;
      return BridgeClient::sharedStaticData->isMultiplayer;
    }
    //----------------------------------------------- IS REPLAY ------------------------------------------------
    bool isReplay()
    {
      if(!BridgeClient::sharedStaticData)
        return false;
      return BridgeClient::sharedStaticData->isReplay;
    }
    //----------------------------------------------- IS PAUSED ------------------------------------------------
    bool isPaused()
    {
      if(!BridgeClient::sharedStaticData)
        return false;
      return BridgeClient::sharedStaticData->isPaused;
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

    //------------------------------------------------- PRINTF -------------------------------------------------
    void  printf(const char* text, ...)
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
