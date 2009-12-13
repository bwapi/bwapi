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
    //--------------------------------------------------- GAME SPEED -------------------------------------------
    void  setLocalSpeed(int speed)
    {
    }
    */
    //----------------------------------------------------- DRAW -----------------------------------------------
    /* TODO: reform
    void addShape(Shape* s)
    {
      // Adds a shape to the draw queue 
      shapes.push_back(s);
    }
    void  drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      // Draws a box 
      if (!inScreen(ctype,left,top,right,bottom)) return;
      addShape(new ShapeBox(ctype, left, top, right, bottom, color.getID(), isSolid));
    }
    void  drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      // Draws a box in relation to the map 
      if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom)) return;
      addShape(new ShapeBox(BWAPI::CoordinateType::Map, left, top, right, bottom, color.getID(), isSolid));
    }
    void  drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      // Draws a box in relation to the mouse 
      if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom)) return;
      addShape(new ShapeBox(BWAPI::CoordinateType::Mouse, left, top, right, bottom, color.getID(), isSolid));
    }
    void  drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      // Draws a box in relation to the screen 
      if (!inScreen(BWAPI::CoordinateType::Screen,left,top,right,bottom)) return;
      addShape(new ShapeBox(BWAPI::CoordinateType::Screen, left, top, right, bottom, color.getID(), isSolid));
    }

    void  drawDot(int ctype, int x, int y, Color color)
    {
      if (!inScreen(ctype,x,y)) return;
      addShape(new ShapeDot(ctype, x, y, color.getID()));
    }
    void  drawDotMap(int x, int y, Color color)
    {
      if (!inScreen(BWAPI::CoordinateType::Map,x,y)) return;
      addShape(new ShapeDot(BWAPI::CoordinateType::Map, x, y, color.getID()));
    }
    void  drawDotMouse(int x, int y, Color color)
    {
      if (!inScreen(BWAPI::CoordinateType::Mouse,x,y)) return;
      addShape(new ShapeDot(BWAPI::CoordinateType::Mouse, x, y, color.getID()));
    }
    void  drawDotScreen(int x, int y, Color color)
    {
      if (!inScreen(BWAPI::CoordinateType::Screen,x,y)) return;
      addShape(new ShapeDot(BWAPI::CoordinateType::Screen, x, y, color.getID()));
    }

    void  drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
    {
      if (!inScreen(ctype,x-radius,y-radius,x+radius,y+radius)) return;
      addShape(new ShapeCircle(ctype, x, y, radius, color.getID(), isSolid));
    }
    void  drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
    {
      if (!inScreen(BWAPI::CoordinateType::Map,x-radius,y-radius,x+radius,y+radius)) return;
      addShape(new ShapeCircle(BWAPI::CoordinateType::Map, x, y, radius, color.getID(), isSolid));
    }
    void  drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
    {
      if (!inScreen(BWAPI::CoordinateType::Mouse,x-radius,y-radius,x+radius,y+radius)) return;
      addShape(new ShapeCircle(BWAPI::CoordinateType::Mouse, x, y, radius, color.getID(), isSolid));
    }
    void  drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
    {
      if (!inScreen(BWAPI::CoordinateType::Screen,x-radius,y-radius,x+radius,y+radius)) return;
      addShape(new ShapeCircle(BWAPI::CoordinateType::Screen, x, y, radius, color.getID(), isSolid));
    }

    void  drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      if (!inScreen(ctype,x-xrad,y-yrad,x+xrad,y+yrad)) return;
      addShape(new ShapeEllipse(ctype, x, y, xrad, yrad, color.getID(), isSolid));
    }
    void  drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      if (!inScreen(BWAPI::CoordinateType::Map,x-xrad,y-yrad,x+xrad,y+yrad)) return;
      addShape(new ShapeEllipse(BWAPI::CoordinateType::Map, x, y, xrad, yrad, color.getID(), isSolid));
    }
    void  drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      if (!inScreen(BWAPI::CoordinateType::Mouse,x-xrad,y-yrad,x+xrad,y+yrad)) return;
      addShape(new ShapeEllipse(BWAPI::CoordinateType::Mouse, x, y, xrad, yrad, color.getID(), isSolid));
    }
    void  drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      if (!inScreen(BWAPI::CoordinateType::Screen,x-xrad,y-yrad,x+xrad,y+yrad)) return;
      addShape(new ShapeEllipse(BWAPI::CoordinateType::Screen, x, y, xrad, yrad, color.getID(), isSolid));
    }

    void  drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
    {
      if (!inScreen(ctype,x1,y1,x2,y2)) return;
      addShape(new ShapeLine(ctype, x1, y1, x2, y2, color.getID()));
    }
    void  drawLineMap(int x1, int y1, int x2, int y2, Color color)
    {
      if (!inScreen(BWAPI::CoordinateType::Map,x1,y1,x2,y2)) return;
      addShape(new ShapeLine(BWAPI::CoordinateType::Map, x1, y1, x2, y2, color.getID()));
    }
    void  drawLineMouse(int x1, int y1, int x2, int y2, Color color)
    {
      if (!inScreen(BWAPI::CoordinateType::Mouse,x1,y1,x2,y2)) return;
      addShape(new ShapeLine(BWAPI::CoordinateType::Mouse, x1, y1, x2, y2, color.getID()));
    }
    void  drawLineScreen(int x1, int y1, int x2, int y2, Color color)
    {
      if (!inScreen(BWAPI::CoordinateType::Screen,x1,y1,x2,y2)) return;
      addShape(new ShapeLine(BWAPI::CoordinateType::Screen, x1, y1, x2, y2, color.getID()));
    }

    void  drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      if (!inScreen(ctype,ax,ay,bx,by,cx,cy)) return;
      addShape(new ShapeTriangle(ctype, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
    }
    void  drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      if (!inScreen(BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy)) return;
      addShape(new ShapeTriangle(BWAPI::CoordinateType::Map, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
    }
    void  drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      if (!inScreen(BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy)) return;
      addShape(new ShapeTriangle(BWAPI::CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
    }
    void  drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      if (!inScreen(BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy)) return;
      addShape(new ShapeTriangle(BWAPI::CoordinateType::Screen, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
    }

    void  drawText(int ctype, int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      addShape(new ShapeText(ctype,x,y,std::string(buffer)));
    }
    void  drawTextMap(int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      addShape(new ShapeText(BWAPI::CoordinateType::Map,x,y,std::string(buffer)));
    }
    void  drawTextMouse(int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      addShape(new ShapeText(BWAPI::CoordinateType::Mouse,x,y,std::string(buffer)));
    }
    void  drawTextScreen(int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      addShape(new ShapeText(BWAPI::CoordinateType::Screen,x,y,std::string(buffer)));
    }
    */
  };
};
