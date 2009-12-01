#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "Util\Version.h"
#include "Game.h"

#include <string>

#include <Util/FileLogger.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

namespace BWAgent
{
  namespace Game
  {/*
    //----------------------------------------------- MAP WIDTH ------------------------------------------------
    int  mapWidth()
    {
    }
    //----------------------------------------------- MAP HEIGHT -----------------------------------------------
    int  mapHeight()
    {
    }
    //---------------------------------------------- MAP FILENAME ----------------------------------------------
    std::string  mapFilename()
    {
    }
    //------------------------------------------------ MAP NAME ------------------------------------------------
    std::string  mapName()
    {
    }
    //---------------------------------------------- GROUND HEIGHT ---------------------------------------------
    int  getGroundHeight(int x, int y)
    {
    }
    //------------------------------------------------ BUILDABLE -----------------------------------------------
    bool  isBuildable(int x, int y)
    {
    }
    //------------------------------------------------ BUILDABLE -----------------------------------------------
    bool  isBuildable(TilePosition position)
    {
    }
    //------------------------------------------------ WALKABLE ------------------------------------------------
    bool  isWalkable(int x, int y)
    {
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool  isVisible(int x, int y)
    {
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool  isVisible(TilePosition position)
    {
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool  isExplored(int x, int y)
    {
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool  isExplored(TilePosition position)
    {
    }
    //------------------------------------------------ HAS CREEP -----------------------------------------------
    bool  hasCreep(int x, int y)
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
    //----------------------------------------------- GET MAP HASH ---------------------------------------------
    int  getMapHash()
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
    //------------------------------------------------- GET UNITS ----------------------------------------------
    std::set< Unit* >& getAllUnits()
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
    //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
    void IssueCommand(PBYTE pbBuffer, u32 iSize)
    {
    }
    //------------------------------------------------- UPDATE -------------------------------------------------
    void update()
    {
    }
    //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
    void refreshSelectionStates()
    {
    }
    //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
    bool isMultiplayer()
    {
    }
    //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
    bool _isSinglePlayer() const
    {
    }
    //------------------------------------------- IS ON START CALLED -------------------------------------------
    bool isOnStartCalled() const
    {
    }
    //------------------------------------------ SET ON START CALLED -------------------------------------------
    void setOnStartCalled(bool onStartCalled)
    {
    }
    //------------------------------------------------ IN GAME -------------------------------------------------
    bool inGame() const
    {
    }
    //----------------------------------------------- IS PAUSED ------------------------------------------------
    bool isPaused()
    {
    }
    //----------------------------------------------- IN REPLAY ------------------------------------------------
    bool  isReplay()
    {
    }
    //----------------------------------------------- IN REPLAY ------------------------------------------------
    bool  _isReplay() const
    {
    }
    const int BUFFER_SIZE = 2048;
    char buffer[BUFFER_SIZE];

    //---------------------------------------------- PRINT WITH PLAYER ID --------------------------------------
    void printEx(s32 pID, const char* text, ...)
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
    //----------------------------------------- ADD TO COMMAND BUFFER ------------------------------------------
    void addToCommandBuffer(Command* command)
    {
    }
    //--------------------------------------------- ON GAME START ----------------------------------------------
    void onGameStart()
    {
    }

    //---------------------------------------------- ON SEND TEXT ----------------------------------------------
    bool onSendText(const char* text)
    {
    }
    //----------------------------------------------- PARSE TEXT -----------------------------------------------
    bool parseText(const char* text)
    {
    }
    //---------------------------------------------- ON GAME END -----------------------------------------------
    void onGameEnd()
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
    //---------------------------------------------- GET MOUSE X -----------------------------------------------
    int  getMouseX()
    {
    }
    //---------------------------------------------- GET MOUSE Y -----------------------------------------------
    int  getMouseY()
    {
    }
    //---------------------------------------------- GET SCREEN X ----------------------------------------------
    int  getScreenX()
    {
    }
    //---------------------------------------------- GET SCREEN Y ----------------------------------------------
    int  getScreenY()
    {
    }
    //------------------------------------------- SET SCREEN POSITION ------------------------------------------
    void setScreenPosition(int x, int y)
    {
    }
    //----------------------------------------------------------------------------------------------------------
    void refresh()
    {
    }
    //----------------------------------------------------------------------------------------------------------
    UnitImpl* getUnit(int index)
    {
    }
    //--------------------------------------------- SAVE SELECTED ----------------------------------------------
    void saveSelected()
    {
    }
    //--------------------------------------------- LOAD SELECTED ----------------------------------------------
    void loadSelected()
    {
    }
    //------------------------------------------ GET SELECTED UNITS --------------------------------------------
    std::set<BWAPI::Unit*>& getSelectedUnits()
    {
    }
    //--------------------------------------------- ON REMOVE UNIT ---------------------------------------------
    void onUnitDeath(BW::Unit* unit)
    {
    }
    //---------------------------------------------- ON ADD UNIT -----------------------------------------------
    void onAddUnit(BWAPI::Unit* unit)
    {
    }
    //----------------------------------------------- GET FIRST ------------------------------------------------
    UnitImpl* getFirst()
    {
    }
    //---------------------------------------------- GET LATENCY -----------------------------------------------
    int getLatency()
    {
    }
    //------------------------------------------ UPDATE UNITS ON TILE ------------------------------------------
    void updateUnits()
    {
    }
    //--------------------------------------------- GET FRAME COUNT --------------------------------------------
    int  getFrameCount()
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
    //--------------------------------------------- SET LAST ERROR ---------------------------------------------
    void setLastError(BWAPI::Error e)
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
    //-------------------------------------------------- LOCK FLAGS --------------------------------------------
    void lockFlags()
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
    void addShape(Shape* s)
    {
    }
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

    bool inScreen(int ctype, int x, int y)
    {
    }

    bool inScreen(int ctype, int x1, int y1, int x2, int y2)
    {
    }

    bool inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3)
    {
    }

    bool gluMessageBox(char* message, int type)
    {
    }

    bool gluEditBox(char* message, char* dest, size_t destsize, char* restricted)
    {
    }*/
  };
};
