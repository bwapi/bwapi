#include "Game.h"

#include "Unit.h"

#include <Util\Version.h>
#include <Util\Buffer.h>

#include <string>

namespace BWAPI2
{
  namespace Game
  {
    //private:
    std::set<Unit*> unitsOnTileData[256][256];
    std::set<Force*> forces;
    std::set<Player*> players;
    std::set<Unit*> allUnits;
    std::set<Unit*> minerals;
    std::set<Unit*> geysers;
    std::set<Unit*> neutralUnits;
    std::set<Unit*> staticMinerals;
    std::set<Unit*> staticGeysers;
    std::set<Unit*> staticNeutralUnits;
    std::set<Unit*> selectedUnits;
    std::map<int,Force*> forceMap;
    std::map<int,Player*> playerMap;
    std::map<int,Unit*> unitMap;
    const int BUFFER_SIZE=1024;
    char buffer[BUFFER_SIZE];

    //public:
    //------------------------------------------------- GET FORCES ---------------------------------------------
    std::set< Force* >& getForces()
    {
      return forces;
    }
    //------------------------------------------------- GET PLAYERS --------------------------------------------
    std::set< Player* >& getPlayers()
    {
      return players;
    }
    //------------------------------------------------- GET ALL UNITS ------------------------------------------
    std::set< Unit* >& getAllUnits()
    {
      return allUnits;
    }
    //------------------------------------------------- GET MINERALS -------------------------------------------
    std::set< Unit* >& getMinerals()
    {
      return minerals;
    }
    //------------------------------------------------- GET GEYSERS --------------------------------------------
    std::set< Unit* >& getGeysers()
    {
      return geysers;
    }
    //------------------------------------------------- GET NEUTRAL UNITS --------------------------------------
    std::set< Unit* >& getNeutralUnits()
    {
      return neutralUnits;
    }
    //------------------------------------------------- GET STATIC MINERALS ------------------------------------
    std::set< Unit* >& getStaticMinerals()
    {
      return staticMinerals;
    }
    //------------------------------------------------- GET STATIC GEYSERS -------------------------------------
    std::set< Unit* >& getStaticGeysers()
    {
      return staticGeysers;
    }
    //------------------------------------------------- GET STATIC NEUTRAL UNITS -------------------------------
    std::set< Unit* >& getStaticNeutralUnits()
    {
      return staticNeutralUnits;
    }
    //----------------------------------------------- GET FORCE ------------------------------------------------
    Force* getForce(int forceId)
    {
      return forceMap[forceId];
    }
    //----------------------------------------------- GET PLAYER -----------------------------------------------
    Player* getPlayer(int playerId)
    {
      return playerMap[playerId];
    }
    //----------------------------------------------- GET UNIT -------------------------------------------------
    Unit* getUnit(int unitId)
    {
      return unitMap[unitId];
    }
    //---------------------------------------------- GET LATENCY -----------------------------------------------
    int getLatency()
    {
      return BWAPI::GetStaticGameData()->getLatency;
    }
    //--------------------------------------------- GET FRAME COUNT --------------------------------------------
    int getFrameCount()
    {
      return BWAPI::GetStaticGameData()->frameCount;
    }
    //---------------------------------------------- GET MOUSE X -----------------------------------------------
    int getMouseX()
    {
      return BWAPI::GetStaticGameData()->mouseX;
    }
    //---------------------------------------------- GET MOUSE Y -----------------------------------------------
    int getMouseY()
    {
      return BWAPI::GetStaticGameData()->mouseY;
    }
    //-------------------------------------------- GET MOUSE POSITION ------------------------------------------
    Position getMousePosition()
    {
      return Position(BWAPI::GetStaticGameData()->mouseX,BWAPI::GetStaticGameData()->mouseY);
    }
    //---------------------------------------------- GET MOUSE STATE -------------------------------------------
    bool getMouseState(MouseButton button)
    {
      return getMouseState((int)button);
    }
    //---------------------------------------------- GET MOUSE STATE -------------------------------------------
    bool getMouseState(int button)
    {
      return false;//BWAPI::GetStaticGameData()->mouseState[button];
    }
    //----------------------------------------------- GET KEY STATE --------------------------------------------
    bool getKeyState(Key key)
    {
      return getKeyState((int)key);
    }
    //----------------------------------------------- GET KEY STATE --------------------------------------------
    bool getKeyState(int key)
    {
      return false;//BWAPI::GetStaticGameData()->keyState[key];
    }
    //---------------------------------------------- GET SCREEN X ----------------------------------------------
    int getScreenX()
    {
      return BWAPI::GetStaticGameData()->screenX;
    }
    //---------------------------------------------- GET SCREEN Y ----------------------------------------------
    int getScreenY()
    {
      return BWAPI::GetStaticGameData()->screenY;
    }
    //-------------------------------------------- GET SCREEN POSITION -----------------------------------------
    Position getScreenPosition()
    {
      return Position(BWAPI::GetStaticGameData()->screenX,BWAPI::GetStaticGameData()->screenY);
    }
    //-------------------------------------------- SET SCREEN POSITION -----------------------------------------
    void setScreenPosition(int x, int y)
    {
      //BWAPI::SetScreenPosition(x,y);
    }
    //-------------------------------------------- SET SCREEN POSITION -----------------------------------------
    void setScreenPosition(Position p)
    {
      //BWAPI::SetScreenPosition(p.x(),p.y());
    }
    //----------------------------------------------- PING MINIMAP ---------------------------------------------
    void pingMinimap(int x, int y)
    {
      //BWAPI::PingMinimap(x,y);
    }
    //----------------------------------------------- PING MINIMAP ---------------------------------------------
    void pingMinimap(Position p)
    {
      //BWAPI::PingMinimap(p.x(),p.y());
    }
    bool isFlagEnabled(int flag)
    {
      return false;//BWAPI::GetStaticGameData()->isFlagEnabled(flag);
    }
    void enableFlag(int flag)
    {
      //BWAPI::EnableFlag(flag);
    }
    std::set<Unit*>& unitsOnTile(int x, int y)
    {
      return unitsOnTileData[x][y];
    }
    //----------------------------------------------- MAP WIDTH ------------------------------------------------
    int mapWidth()
    {
      return BWAPI::GetStaticGameData()->mapWidth;
    }
    //----------------------------------------------- MAP HEIGHT -----------------------------------------------
    int mapHeight()
    {
      return BWAPI::GetStaticGameData()->mapHeight;
    }
    //---------------------------------------------- MAP FILENAME ----------------------------------------------
    std::string mapFilename()
    {
      return std::string(BWAPI::GetStaticGameData()->mapFilename);
    }
    //------------------------------------------------ MAP NAME ------------------------------------------------
    std::string mapName()
    {
      return std::string(BWAPI::GetStaticGameData()->mapName);
    }
    //----------------------------------------------- GET MAP HASH ---------------------------------------------
    int getMapHash()
    {
      return BWAPI::GetStaticGameData()->mapHash;
    }

    //------------------------------------------- GET GROUND HEIGHT --------------------------------------------
    int getGroundHeight(int x, int y)
    {
      if (x<0 || y<0 || x>=BWAPI::GetStaticGameData()->mapWidth*4 || y>=BWAPI::GetStaticGameData()->mapHeight*4)
        return 0;
      return BWAPI::GetStaticGameData()->getGroundHeight[x][y];
    }
    //---------------------------------------------- IS WALKABLE -----------------------------------------------
    bool isWalkable(int x, int y)
    {
      if (x<0 || y<0 || x>=BWAPI::GetStaticGameData()->mapWidth*4 || y>=BWAPI::GetStaticGameData()->mapHeight*4)
        return 0;
      return BWAPI::GetStaticGameData()->isWalkable[x][y];
    }
    //--------------------------------------------- IS BUILDABLE -----------------------------------------------
    bool isBuildable(int x, int y)
    {
      if (x<0 || y<0 || x>=BWAPI::GetStaticGameData()->mapWidth || y>=BWAPI::GetStaticGameData()->mapHeight)
        return 0;
      return BWAPI::GetStaticGameData()->isBuildable[x][y];
    }
    //---------------------------------------------- IS VISIBLE ------------------------------------------------
    bool isVisible(int x, int y)
    {
      if (x<0 || y<0 || x>=BWAPI::GetStaticGameData()->mapWidth || y>=BWAPI::GetStaticGameData()->mapHeight)
        return 0;
      return BWAPI::GetStaticGameData()->isVisible[x][y];
    }
    //---------------------------------------------- IS EXPLORED -----------------------------------------------
    bool isExplored(int x, int y)
    {
      if (x<0 || y<0 || x>=BWAPI::GetStaticGameData()->mapWidth || y>=BWAPI::GetStaticGameData()->mapHeight)
        return 0;
      return BWAPI::GetStaticGameData()->isExplored[x][y];
    }
    //----------------------------------------------- HAS CREEP ------------------------------------------------
    bool hasCreep(int x, int y)
    {
      if (x<0 || y<0 || x>=BWAPI::GetStaticGameData()->mapWidth || y>=BWAPI::GetStaticGameData()->mapHeight)
        return 0;
      return BWAPI::GetStaticGameData()->hasCreep[x][y];
    }

    //--------------------------------------------- IS MULTIPLAYER ---------------------------------------------
    bool isMultiplayer()
    {
      return BWAPI::GetStaticGameData()->isMultiplayer;
    }
    //----------------------------------------------- IS REPLAY ------------------------------------------------
    bool isReplay()
    {
      return BWAPI::GetStaticGameData()->isReplay;
    }
    //----------------------------------------------- IS PAUSED ------------------------------------------------
    bool isPaused()
    {
      return BWAPI::GetStaticGameData()->isPaused;
    }
    //------------------------------------------------ PRINTF --------------------------------------------------
    void printf(const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::PrintText(buffer);
      return;
    }
    //------------------------------------------------ SEND TEXT -----------------------------------------------
    void sendText(const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::SendText(buffer);
      return;
    }
    //------------------------------------------------ BUILDABLE -----------------------------------------------
    bool  isBuildable(TilePosition position)
    {
      return isBuildable(position.x(),position.y());
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool  isVisible(TilePosition position)
    {
      return isVisible(position.x(),position.y());
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool  isExplored(TilePosition position)
    {
      return isExplored(position.x(),position.y());
    }
    //------------------------------------------------ HAS CREEP -----------------------------------------------
    bool  hasCreep(TilePosition position)
    {
      return hasCreep(position.x(),position.y());
    }
    /*
    //------------------------------------------------ HAS POWER -----------------------------------------------
    bool  hasPower(int x, int y, int tileWidth, int tileHeight)
    {
      //Todo: implement here
    }
    //------------------------------------------------ HAS POWER -----------------------------------------------
    bool  hasPower(TilePosition position, int tileWidth, int tileHeight)
    {
      //Todo: implement here
    }
    //---------------------------------------------- CAN BUILD HERE --------------------------------------------
    bool  canBuildHere(Unit* builder, TilePosition position, UnitType type)
    {
      //Todo: implement here
    }
    //------------------------------------------------- CAN MAKE -----------------------------------------------
    bool  canMake(Unit* builder, UnitType type)
    {
      //Todo: implement here
    }
    //----------------------------------------------- CAN RESEARCH ---------------------------------------------
    bool  canResearch(Unit* unit, TechType type)
    {
      //Todo: implement here
    }
    //----------------------------------------------- CAN UPGRADE ----------------------------------------------
    bool  canUpgrade(Unit* unit, UpgradeType type)
    {
      //Todo: implement here
    }
    //--------------------------------------------- GET START LOCATIONS ----------------------------------------
    std::set< TilePosition >& getStartLocations()
    {
      
    }
    */

    //---------------------------------------------- CHANGE RACE -----------------------------------------------
    void  changeRace(BWAPI::Race race)
    {
      //BWAPI::ChangeRace(race.getID());
    }
    //----------------------------------------------- START GAME -----------------------------------------------
    void  startGame()
    {
      //BWAPI::StartGame();
    }
    //----------------------------------------------- PAUSE GAME -----------------------------------------------
    void  pauseGame()
    {
      //BWAPI::PauseGame();
    }
    //---------------------------------------------- RESUME GAME -----------------------------------------------
    void  resumeGame()
    {
      //BWAPI::ResumeGame();
    }
    //---------------------------------------------- LEAVE GAME ------------------------------------------------
    void  leaveGame()
    {
      //BWAPI::LeaveGame();
    }
    //--------------------------------------------- RESTART GAME -----------------------------------------------
    void  restartGame()
    {
      //BWAPI::RestartGame();
    }
    //------------------------------------------- SET SCREEN POSITION ------------------------------------------
    void setLocalSpeed(int speed)
    {
      //BWAPI::SetLocalSpeed(speed);
    }
    //------------------------------------------ GET SELECTED UNITS --------------------------------------------
    std::set<Unit*>& getSelectedUnits()
    {
      return selectedUnits;
    }
    //----------------------------------------------------- SELF -----------------------------------------------
    Player*  self()
    {
      //return getPlayer(BWAPI::GetStaticGameData()->self);
      return NULL;
    }
    //----------------------------------------------------- ENEMY ----------------------------------------------
    Player*  enemy()
    {
      //return getPlayer(BWAPI::GetStaticGameData()->enemy);
      return NULL;
    }
    //----------------------------------------------------- DRAW -----------------------------------------------
    void drawText(int ctype, int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::DrawText(x,y,buffer);
    }
    void drawTextMap(int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::DrawText(x,y,buffer);
    }
    void drawTextMouse(int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::DrawText(x,y,buffer);
    }
    void drawTextScreen(int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::DrawText(x,y,buffer);
    }

    void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      BWAPI::DrawRectangle(left,top,right-left,bottom-top,color.getID(),isSolid);
    }
    void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      BWAPI::DrawRectangle(left,top,right-left,bottom-top,color.getID(),isSolid);
    }
    void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      BWAPI::DrawRectangle(left,top,right-left,bottom-top,color.getID(),isSolid);
    }
    void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      BWAPI::DrawRectangle(left,top,right-left,bottom-top,color.getID(),isSolid);
    }

    void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      //BWAPI::DrawTriangle(ax,ay,bx,by,cx,cy,color.getID(),isSolid);
    }
    void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      //BWAPI::DrawTriangle(ax,ay,bx,by,cx,cy,color.getID(),isSolid);
    }
    void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      //BWAPI::DrawTriangle(ax,ay,bx,by,cx,cy,color.getID(),isSolid);
    }
    void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      //BWAPI::DrawTriangle(ax,ay,bx,by,cx,cy,color.getID(),isSolid);
    }

    void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
    {
      BWAPI::DrawCircle(x,y,radius,color.getID(),isSolid);
    }
    void drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
    {
      BWAPI::DrawCircle(x,y,radius,color.getID(),isSolid);
    }
    void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
    {
      BWAPI::DrawCircle(x,y,radius,color.getID(),isSolid);
    }
    void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
    {
      BWAPI::DrawCircle(x,y,radius,color.getID(),isSolid);
    }

    void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      //BWAPI::DrawEllipse(x,y,xrad,yrad,color.getID(),isSolid);
    }
    void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      //BWAPI::DrawEllipse(x,y,xrad,yrad,color.getID(),isSolid);
    }
    void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      //BWAPI::DrawEllipse(x,y,xrad,yrad,color.getID(),isSolid);
    }
    void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      //BWAPI::DrawEllipse(x,y,xrad,yrad,color.getID(),isSolid);
    }

    void drawDot(int ctype, int x, int y, Color color)
    {
      BWAPI::DrawDot(x,y,color.getID());
    }
    void drawDotMap(int x, int y, Color color)
    {
      BWAPI::DrawDot(x,y,color.getID());
    }
    void drawDotMouse(int x, int y, Color color)
    {
      BWAPI::DrawDot(x,y,color.getID());
    }
    void drawDotScreen(int x, int y, Color color)
    {
      BWAPI::DrawDot(x,y,color.getID());
    }
    void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
    {
      BWAPI::DrawLine(x1,y1,x2,y2,color.getID());
    }
    void drawLineMap(int x1, int y1, int x2, int y2, Color color)
    {
      BWAPI::DrawLine(x1,y1,x2,y2,color.getID());
    }
    void drawLineMouse(int x1, int y1, int x2, int y2, Color color)
    {
      BWAPI::DrawLine(x1,y1,x2,y2,color.getID());
    }
    void drawLineScreen(int x1, int y1, int x2, int y2, Color color)
    {
      BWAPI::DrawLine(x1,y1,x2,y2,color.getID());
    }
  };
};
