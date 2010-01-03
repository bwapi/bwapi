#include "Game.h"

#include "Force.h"
#include "Player.h"
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
    std::set< TilePosition > startLocations;
    std::map<int,Force*> forceMap;
    std::map<int,Player*> playerMap;
    std::map<int,Unit*> unitMap;
    const int BUFFER_SIZE=1024;
    char buffer[BUFFER_SIZE];
    const BWAPI::StaticGameData* sgd;
    //------------------------------------------------- ON START -----------------------------------------------
    void onStart()
    {
      sgd=BWAPI::GetStaticGameData();
      startLocations.clear();
      for(std::map<int,Force*>::iterator i=forceMap.begin();i!=forceMap.end();i++)
      {
        delete i->second;
      }
      for(std::map<int,Player*>::iterator i=playerMap.begin();i!=playerMap.end();i++)
      {
        delete i->second;
      }
      for(std::map<int,Unit*>::iterator i=unitMap.begin();i!=unitMap.end();i++)
      {
        delete i->second;
      }
      forceMap.clear();
      playerMap.clear();
      unitMap.clear();
      for(int i=0;i<sgd->startLocations.size;i++)
      {
        BWAPI::Position p(sgd->startLocations.data[i]);
        BWAPI2::TilePosition p2(p.x,p.y);
        startLocations.insert(p2);
      }

    }
    //------------------------------------------------- ON FRAME -----------------------------------------------
    void onFrame()
    {
      allUnits.clear();
      HANDLE h=BWAPI::AllUnitsBegin();
      int unitId=BWAPI::AllUnitsNext(h);
      while(unitId!=-1)
      {
        if (unitMap.find(unitId)==unitMap.end())
          unitMap[unitId]=new Unit(unitId);

        allUnits.insert(getUnit(unitId));

        unitId=BWAPI::AllUnitsNext(h);
      }
      BWAPI::AllUnitsClose(h);
    }
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
      return sgd->getLatency;
    }
    //--------------------------------------------- GET FRAME COUNT --------------------------------------------
    int getFrameCount()
    {
      return sgd->frameCount;
    }
    //---------------------------------------------- GET MOUSE X -----------------------------------------------
    int getMouseX()
    {
      return sgd->mouseX;
    }
    //---------------------------------------------- GET MOUSE Y -----------------------------------------------
    int getMouseY()
    {
      return sgd->mouseY;
    }
    //-------------------------------------------- GET MOUSE POSITION ------------------------------------------
    Position getMousePosition()
    {
      return Position(sgd->mouseX,sgd->mouseY);
    }
    //---------------------------------------------- GET MOUSE STATE -------------------------------------------
    bool getMouseState(MouseButton button)
    {
      return getMouseState((int)button);
    }
    //---------------------------------------------- GET MOUSE STATE -------------------------------------------
    bool getMouseState(int button)
    {
      return false;//sgd->mouseState[button];
    }
    //----------------------------------------------- GET KEY STATE --------------------------------------------
    bool getKeyState(Key key)
    {
      return getKeyState((int)key);
    }
    //----------------------------------------------- GET KEY STATE --------------------------------------------
    bool getKeyState(int key)
    {
      return false;//sgd->keyState[key];
    }
    //---------------------------------------------- GET SCREEN X ----------------------------------------------
    int getScreenX()
    {
      return sgd->screenX;
    }
    //---------------------------------------------- GET SCREEN Y ----------------------------------------------
    int getScreenY()
    {
      return sgd->screenY;
    }
    //-------------------------------------------- GET SCREEN POSITION -----------------------------------------
    Position getScreenPosition()
    {
      return Position(sgd->screenX,sgd->screenY);
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
      return false;//sgd->isFlagEnabled(flag);
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
      return sgd->mapWidth;
    }
    //----------------------------------------------- MAP HEIGHT -----------------------------------------------
    int mapHeight()
    {
      return sgd->mapHeight;
    }
    //---------------------------------------------- MAP FILENAME ----------------------------------------------
    std::string mapFilename()
    {
      return std::string(sgd->mapFilename);
    }
    //------------------------------------------------ MAP NAME ------------------------------------------------
    std::string mapName()
    {
      return std::string(sgd->mapName);
    }
    //----------------------------------------------- GET MAP HASH ---------------------------------------------
    int getMapHash()
    {
      return sgd->mapHash;
    }

    //------------------------------------------- GET GROUND HEIGHT --------------------------------------------
    int getGroundHeight(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapWidth*4 || y>=sgd->mapHeight*4)
        return 0;
      return sgd->getGroundHeight[x][y];
    }
    //---------------------------------------------- IS WALKABLE -----------------------------------------------
    bool isWalkable(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapWidth*4 || y>=sgd->mapHeight*4)
        return 0;
      return sgd->isWalkable[x][y];
    }
    //--------------------------------------------- IS BUILDABLE -----------------------------------------------
    bool isBuildable(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapWidth || y>=sgd->mapHeight)
        return 0;
      return sgd->isBuildable[x][y];
    }
    //---------------------------------------------- IS VISIBLE ------------------------------------------------
    bool isVisible(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapWidth || y>=sgd->mapHeight)
        return 0;
      return sgd->isVisible[x][y];
    }
    //---------------------------------------------- IS EXPLORED -----------------------------------------------
    bool isExplored(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapWidth || y>=sgd->mapHeight)
        return 0;
      return sgd->isExplored[x][y];
    }
    //----------------------------------------------- HAS CREEP ------------------------------------------------
    bool hasCreep(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapWidth || y>=sgd->mapHeight)
        return 0;
      return sgd->hasCreep[x][y];
    }

    //--------------------------------------------- IS MULTIPLAYER ---------------------------------------------
    bool isMultiplayer()
    {
      return sgd->isMultiplayer;
    }
    //----------------------------------------------- IS REPLAY ------------------------------------------------
    bool isReplay()
    {
      return sgd->isReplay;
    }
    //----------------------------------------------- IS PAUSED ------------------------------------------------
    bool isPaused()
    {
      return sgd->isPaused;
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
    */
    //--------------------------------------------- GET START LOCATIONS ----------------------------------------
    std::set< TilePosition >& getStartLocations()
    {
      return startLocations;
    }

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
      //return getPlayer(sgd->self);
      return NULL;
    }
    //----------------------------------------------------- ENEMY ----------------------------------------------
    Player*  enemy()
    {
      //return getPlayer(sgd->enemy);
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
