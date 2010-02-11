#include "Game.h"
#include "Constants.h"
#include "CoordinateType.h"
#include "Force.h"
#include "Player.h"
#include "Unit.h"

#include <Util\Version.h>
#include <Util\Buffer.h>
#include <Util\Foreach.h>
#include <string>

namespace BWSL
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
    std::set<Unit*> pylons;
    std::set< TilePosition > startLocations;
    std::map<int,Force*> forceMap;
    std::map<int,Player*> playerMap;
    std::map<int,Unit*> unitMap;
    std::map<const Player*, std::set<Unit*> > playerUnits;
    const int BUFFER_SIZE=1024;
    char buffer[BUFFER_SIZE];
    bool flagEnabled[2];
    const BWAPI::StaticGameData* sgd;
    Player* theEnemy;
    Error lastError;
    void saveInitialState()
    {
      //TODO: Implement
    }
    //------------------------------------------------- ON START -----------------------------------------------
    void onStart()
    {
      sgd=BWAPI::getStaticGameData();
      saveInitialState();
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
        BWSL::TilePosition p2(p.x,p.y);
        startLocations.insert(p2);
      }
      for each(int playerId in sgd->players)
      {
        if (playerMap.find(playerId)==playerMap.end())
        {
          playerMap[playerId]=new Player(playerId);
          players.insert(playerMap[playerId]);
        }
      }
      theEnemy = NULL;
      foreach(Player* p, players)
      {
        if (self()->isEnemy(p))
          theEnemy=p;
      }
      for(int forceId=0;forceId<4;forceId++)
      {
        if (forceMap.find(forceId)==forceMap.end())
        {
          forceMap[forceId]=new Force(forceId);
          forces.insert(forceMap[forceId]);
        }
      }

    }
    //------------------------------------------------- ON FRAME -----------------------------------------------
    void onFrame()
    {
      allUnits.clear();
      neutralUnits.clear();
      minerals.clear();
      geysers.clear();
      pylons.clear();
      playerUnits.clear();
      foreach(Player* player,players)
      {
        player->update();
      }
      for (int y = 0; y < sgd->mapSize.y; y++)
        for (int x = 0; x < sgd->mapSize.x; x++)
          unitsOnTileData[x][y].clear();

      foreach(int unitId,sgd->units)
      {
        if (unitMap.find(unitId)==unitMap.end())
          unitMap[unitId]=new Unit(unitId);
        Unit* u=getUnit(unitId);
        u->larva.clear();
      }
      foreach(int unitId,sgd->units)
      {
        Unit* u=getUnit(unitId);
        if (u->getHatchery()!=NULL)
          u->getHatchery()->larva.insert(u);
        u->startingAttack           = (u->getAirWeaponCooldown() > u->lastAirWeaponCooldown) || (u->getGroundWeaponCooldown() > u->lastGroundWeaponCooldown);
        u->lastAirWeaponCooldown    = u->getAirWeaponCooldown();
        u->lastGroundWeaponCooldown = u->getGroundWeaponCooldown();
        int startX = (u->getPosition().x() - u->getType().dimensionLeft()) / BWSL::TILE_SIZE;
        int endX   = (u->getPosition().x() + u->getType().dimensionRight() + BWSL::TILE_SIZE - 1) / BWSL::TILE_SIZE; // Division - round up
        int startY = (u->getPosition().y() - u->getType().dimensionUp()) / BWSL::TILE_SIZE;
        int endY   = (u->getPosition().y() + u->getType().dimensionDown() + BWSL::TILE_SIZE - 1) / BWSL::TILE_SIZE;
        for (int x = startX; x < endX; x++)
          for (int y = startY; y < endY; y++)
            unitsOnTileData[x][y].insert(u);

        allUnits.insert(u);
        playerUnits[u->getPlayer()].insert(u);
        if (u->getPlayer()->isNeutral())
        {
          neutralUnits.insert(u);
          if (u->getType()==UnitTypes::Resource_Mineral_Field)
            minerals.insert(u);
          else if (u->getType()==UnitTypes::Resource_Vespene_Geyser)
            geysers.insert(u);
        }
        else
        {
          if (u->getType()==UnitTypes::Protoss_Pylon)
            pylons.insert(u);
        }
      }
    }
    std::set<Unit*>& getPlayerUnits(const Player* player)
    {
      return playerUnits[player];
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
      std::map<int,Force*>::iterator i=forceMap.find(forceId);
      if (i==forceMap.end())
        return NULL;
      return i->second;
    }
    //----------------------------------------------- GET PLAYER -----------------------------------------------
    Player* getPlayer(int playerId)
    {
      std::map<int,Player*>::iterator i=playerMap.find(playerId);
      if (i==playerMap.end())
        return NULL;
      return i->second;
    }
    //----------------------------------------------- GET UNIT -------------------------------------------------
    Unit* getUnit(int unitId)
    {
      std::map<int,Unit*>::iterator i=unitMap.find(unitId);
      if (i==unitMap.end())
        return NULL;
      return i->second;
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
      return sgd->mousePosition.x;
    }
    //---------------------------------------------- GET MOUSE Y -----------------------------------------------
    int getMouseY()
    {
      return sgd->mousePosition.y;
    }
    //-------------------------------------------- GET MOUSE POSITION ------------------------------------------
    Position getMousePosition()
    {
      return Position(sgd->mousePosition.x,sgd->mousePosition.y);
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
      return sgd->screenPosition.x;
    }
    //---------------------------------------------- GET SCREEN Y ----------------------------------------------
    int getScreenY()
    {
      return sgd->screenPosition.y;
    }
    //-------------------------------------------- GET SCREEN POSITION -----------------------------------------
    Position getScreenPosition()
    {
      return Position(sgd->screenPosition.x,sgd->screenPosition.y);
    }
    //-------------------------------------------- SET SCREEN POSITION -----------------------------------------
    void setScreenPosition(int x, int y)
    {
      BWAPI::setScreenPosition(BWAPI::Position(x,y));
    }
    //-------------------------------------------- SET SCREEN POSITION -----------------------------------------
    void setScreenPosition(Position p)
    {
      BWAPI::setScreenPosition(BWAPI::Position(p.x(),p.y()));
    }
    //----------------------------------------------- PING MINIMAP ---------------------------------------------
    void pingMinimap(int x, int y)
    {
      BWAPI::pingMinimap(BWAPI::Position(x,y));
    }
    //----------------------------------------------- PING MINIMAP ---------------------------------------------
    void pingMinimap(Position p)
    {
      BWAPI::pingMinimap(BWAPI::Position(p.x(),p.y()));
    }
    //----------------------------------------------- IS FLAG ENABLED ------------------------------------------
    bool isFlagEnabled(int flag)
    {
      if (flag<0 || flag>=2) return false;
      return flagEnabled[flag];
    }
    //----------------------------------------------- ENABLE FLAG ----------------------------------------------
    void enableFlag(int flag)
    {
      if (flag<0 || flag>=2) return;
      BWAPI::enableFlag((BWAPI::Flag)flag);
      flagEnabled[flag]=true;
    }
    //----------------------------------------------- UNITS ON TILE --------------------------------------------
    std::set<Unit*>& unitsOnTile(int x, int y)
    {
      return unitsOnTileData[x][y];
    }
    //----------------------------------------------- GET LAST ERROR -------------------------------------------
    Error getLastError()
    {
      return lastError;
    }
    //----------------------------------------------- MAP WIDTH ------------------------------------------------
    int mapWidth()
    {
      return sgd->mapSize.x;
    }
    //----------------------------------------------- MAP HEIGHT -----------------------------------------------
    int mapHeight()
    {
      return sgd->mapSize.y;
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
      if (x<0 || y<0 || x>=sgd->mapSize.x*4 || y>=sgd->mapSize.y*4)
        return 0;
      return sgd->getGroundHeight[x][y];
    }
    //---------------------------------------------- IS WALKABLE -----------------------------------------------
    bool isWalkable(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapSize.x*4 || y>=sgd->mapSize.y*4)
        return 0;
      return sgd->isWalkable[x][y];
    }
    //--------------------------------------------- IS BUILDABLE -----------------------------------------------
    bool isBuildable(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapSize.x || y>=sgd->mapSize.y)
        return 0;
      return sgd->isBuildable[x][y];
    }
    //---------------------------------------------- IS VISIBLE ------------------------------------------------
    bool isVisible(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapSize.x || y>=sgd->mapSize.y)
        return 0;
      return sgd->isVisible[x][y];
    }
    //---------------------------------------------- IS EXPLORED -----------------------------------------------
    bool isExplored(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapSize.x || y>=sgd->mapSize.y)
        return 0;
      return sgd->isExplored[x][y];
    }
    //----------------------------------------------- HAS CREEP ------------------------------------------------
    bool hasCreep(int x, int y)
    {
      if (x<0 || y<0 || x>=sgd->mapSize.x || y>=sgd->mapSize.y)
        return 0;
      return sgd->hasCreep[x][y];
    }
    //------------------------------------------------ HAS POWER -----------------------------------------------
    bool hasPower(int x, int y, int tileWidth, int tileHeight)
    {
      lastError = Errors::None;
      if (!(tileWidth == 2 && tileHeight == 2) && !(tileWidth == 3 && tileHeight == 2) && !(tileWidth == 4 && tileHeight == 3))
      {
        return false;
      }
      if (tileWidth == 4)
      {
        x++;
      }
      /* Loop through all pylons for the current player */
      foreach (Unit* i, pylons)
      {
        int px = i->getTilePosition().x();
        int py = i->getTilePosition().y();
        int bx = x - px + 7;
        int by = y - py + 4;
        /* Deal with special cases, pylon offset cutoff */
        if (bx >= 0 && by >= 0 && bx <= 14 && by <= 8)
        {
          switch(by)
          {
            case 0:
              if (bx >= 1 && bx <= 12) return true;
            break;
            case 1:
              if (bx <= 13) return true;
            break;
            case 2:
            case 3:
            case 4:
            case 5:
              return true;
            break;
            case 6:
              if (bx <= 13) return true;
            case 7:
              if (bx >= 1 && bx <= 12) return true;
            case 8:
              if (bx >= 4 && bx <= 9) return true;
            break;
          }
        }
      }
      return false;
    }
    //------------------------------------------------ BUILDABLE -----------------------------------------------
    bool isBuildable(TilePosition position)
    {
      return isBuildable(position.x(),position.y());
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool isVisible(TilePosition position)
    {
      return isVisible(position.x(),position.y());
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool isExplored(TilePosition position)
    {
      return isExplored(position.x(),position.y());
    }
    //------------------------------------------------ HAS CREEP -----------------------------------------------
    bool hasCreep(TilePosition position)
    {
      return hasCreep(position.x(),position.y());
    }
    //------------------------------------------------ HAS POWER -----------------------------------------------
    bool hasPower(TilePosition position, int tileWidth, int tileHeight)
    {
      return hasPower(position.x(),position.y(),tileWidth,tileHeight);
    }
    //---------------------------------------------- CAN BUILD HERE --------------------------------------------
    bool canBuildHere(Unit* builder, TilePosition position, UnitType type)
    {
      lastError = Errors::Unbuildable_Location;
      if (position.x()<0) return false;
      if (position.y()<0) return false;
      int width=type.tileWidth();
      int height=type.tileHeight();
      if (position.x()+width>mapWidth()) return false;
      if (position.y()+height>=mapHeight()) return false;
      if (position.y()+height==mapHeight()-1)
      {
        if (position.x()<5) return false;
        if (position.x()+width>mapWidth()-5) return false;
      }
      if (type.isRefinery())
      {
        foreach (Unit* g, geysers)
        {
          if (g->getTilePosition() == position)
          {
            lastError = Errors::None;
            return true;
          }
        }
        return false;
      }
      for(int x = position.x(); x < position.x() + width; x++)
      {
        for(int y = position.y(); y < position.y() + height; y++)
        {
          std::set<Unit*> groundUnits;
          foreach (Unit* i, unitsOnTile(x,y))
            if (!i->getType().isFlyer() && !i->isLifted())
              groundUnits.insert(i);

          if (!isBuildable(x,y) || groundUnits.size() > 1)
            return false;

          if (!groundUnits.empty())
          {
            Unit* blocking = *(groundUnits.begin());
            if (blocking != builder)
              return false;
          }
        }
      }
      if (type.getRace() == BWSL::Races::Zerg)
      {
        if (!type.isResourceDepot())
          for(int x = position.x(); x < position.x() + width; x++)
            for(int y = position.y(); y < position.y() + height; y++)
              if (!hasCreep(x,y))
                return false;
      }
      else
      {
        for(int x = position.x(); x < position.x() + width; x++)
          for(int y = position.y(); y < position.y() + height; y++)
            if (hasCreep(x,y))
              return false;
      }

      if (type.requiresPsi())
      {
        if (hasPower(position.x(), position.y(), width, height))
        {
          lastError = Errors::None;
          return true;
        }
        return false;
      }

      if (type.isResourceDepot())
      {
        foreach (Unit* m, staticMinerals)
        {
          if (isVisible(m->getInitialTilePosition()) ||
              isVisible(m->getInitialTilePosition().x()+1,m->getInitialTilePosition().y()))
            if (!m->isVisible())
              continue; // tile position is visible, but mineral is not => mineral does not exist
          if (m->getInitialTilePosition().x()>position.x()-5 &&
              m->getInitialTilePosition().y()>position.y()-4 &&
              m->getInitialTilePosition().x()<position.x()+7 &&
              m->getInitialTilePosition().y()<position.y()+6)
          {
            return false;
          }
        }
        foreach (Unit* g, staticGeysers)
        {
          if (g->getInitialTilePosition().x()>position.x()-7 &&
              g->getInitialTilePosition().y()>position.y()-5 &&
              g->getInitialTilePosition().x()<position.x()+7 &&
              g->getInitialTilePosition().y()<position.y()+6)
          {
            return false;
          }
        }
      }
      lastError = Errors::None;
      return true;
    }
    //------------------------------------------------- CAN MAKE -----------------------------------------------
    bool canMake(Unit* builder, UnitType type)
    {
      lastError = Errors::None;
      if (self() == NULL)
      {
        lastError = Errors::Unit_Not_Owned;
        return false;
      }

      if (builder != NULL)
      {
        /* Check if the owner of the unit is you */
        if (builder->getPlayer() != self())
        {
          lastError = Errors::Unit_Not_Owned;
          return false;
        }

        /* Check if this unit can actually build the unit type */
        if (builder->getType() != *(type.whatBuilds().first))
        {
          lastError = Errors::Incompatible_UnitType;
          return false;
        }

        /* Carrier space */
        if (builder->getType() == UnitTypes::Protoss_Carrier)
        {
          int max_amt = 4;
          if (self()->getUpgradeLevel(UpgradeTypes::Carrier_Capacity)>0)
            max_amt += 4;
          if (builder->getInterceptorCount() + (int)builder->getTrainingQueue().size() >= max_amt)
          {
            lastError = Errors::Insufficient_Space;
            return false;
          }
        }

        /* Reaver Space */
        if (builder->getType() == UnitTypes::Protoss_Reaver)
        {
          int max_amt = 5;
          if (self()->getUpgradeLevel(UpgradeTypes::Reaver_Capacity) > 0)
            max_amt += 5;
          if (builder->getScarabCount() + (int)builder->getTrainingQueue().size() >= max_amt)
          {
            lastError = Errors::Insufficient_Space;
            return false;
          }
        }
      }

      /* Check if player has enough minerals */
      if (self()->minerals() < type.mineralPrice())
      {
        lastError = Errors::Insufficient_Minerals;
        return false;
      }

      /* Check if player has enough gas */
      if (self()->gas() < type.gasPrice())
      {
        lastError = Errors::Insufficient_Gas;
        return false;
      }
      
      /* Check if player has enough supplies */
      if (type.supplyRequired() > 0)
        if (self()->supplyTotal() < self()->supplyUsed() + type.supplyRequired() - type.whatBuilds().first->supplyRequired())
        {
          lastError = Errors::Insufficient_Supply;
          return false;
        }

      UnitType addon = UnitTypes::None;
      for(std::map<const UnitType*, int>::const_iterator i = type.requiredUnits().begin(); i != type.requiredUnits().end(); i++)
        if (i->first->isAddon())
          addon=*i->first;

      for(std::map<const UnitType*, int>::const_iterator i = type.requiredUnits().begin(); i != type.requiredUnits().end(); i++)
      {
        bool pass = false;
        if (self()->completedUnitCount(*(i->first)) >= i->second)
          pass = true;
        if (*i->first == UnitTypes::Zerg_Hatchery)
        {
          if (self()->completedUnitCount(UnitTypes::Zerg_Lair) >= i->second)
            pass = true;
          if (self()->completedUnitCount(UnitTypes::Zerg_Hive) >= i->second)
            pass = true;
        }
        if (*i->first == UnitTypes::Zerg_Lair)
          if (self()->completedUnitCount(UnitTypes::Zerg_Hive) >= i->second)
            pass = true;
        if (pass == false)
        {
          lastError = Errors::Insufficient_Tech;
          return false;
        }
      }

      if (*type.requiredTech() != TechTypes::None)
        if (!self()->hasResearched(*(type.requiredTech())))
        {
          lastError = Errors::Insufficient_Tech;
          return false;
        }

      if (builder != NULL)
        if (addon != UnitTypes::None && addon.whatBuilds().first==type.whatBuilds().first)
          if (builder->getAddon() == NULL || builder->getAddon()->getType() != addon)
          {
            lastError = Errors::Insufficient_Tech;
            return false;
          }
      return true;
    }
    //----------------------------------------------- CAN RESEARCH ---------------------------------------------
    bool  canResearch(Unit* unit, TechType type)
    {
      //Todo: implement here
      return false;
    }
    //----------------------------------------------- CAN UPGRADE ----------------------------------------------
    bool  canUpgrade(Unit* unit, UpgradeType type)
    {
      //Todo: implement here
      return false;
    }
    //--------------------------------------------- GET START LOCATIONS ----------------------------------------
    std::set< TilePosition >& getStartLocations()
    {
      return startLocations;
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
      BWAPI::printText(buffer);
      return;
    }
    //------------------------------------------------ SEND TEXT -----------------------------------------------
    void sendText(const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::sendText(buffer);
      return;
    }
    //---------------------------------------------- CHANGE RACE -----------------------------------------------
    void  changeRace(BWAPI::Race race)
    {
      //BWAPI::changeRace(race.getID());
    }
    //----------------------------------------------- START GAME -----------------------------------------------
    void  startGame()
    {
      BWAPI::startGame();
    }
    //----------------------------------------------- PAUSE GAME -----------------------------------------------
    void  pauseGame()
    {
      BWAPI::pauseGame();
    }
    //---------------------------------------------- RESUME GAME -----------------------------------------------
    void  resumeGame()
    {
      BWAPI::resumeGame();
    }
    //---------------------------------------------- LEAVE GAME ------------------------------------------------
    void  leaveGame()
    {
      BWAPI::leaveGame();
    }
    //--------------------------------------------- RESTART GAME -----------------------------------------------
    void  restartGame()
    {
      BWAPI::restartGame();
    }
    //------------------------------------------- SET SCREEN POSITION ------------------------------------------
    void setLocalSpeed(int speed)
    {
      BWAPI::setLocalSpeed(speed);
    }
    //------------------------------------------ GET SELECTED UNITS --------------------------------------------
    std::set<Unit*>& getSelectedUnits()
    {
      return selectedUnits;
    }
    //----------------------------------------------------- SELF -----------------------------------------------
    Player*  self()
    {
      return getPlayer(sgd->self);
    }
    //----------------------------------------------------- ENEMY ----------------------------------------------
    Player*  enemy()
    {
      return theEnemy;
    }
    //----------------------------------------------------- DRAW -----------------------------------------------
    void drawText(int ctype, int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::drawText((BWAPI::CoordinateBase)ctype,BWAPI::Position(x,y),buffer);
    }
    void drawTextMap(int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::drawTextMap(BWAPI::Position(x,y),buffer);
    }
    void drawTextMouse(int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::drawTextMouse(BWAPI::Position(x,y),buffer);
    }
    void drawTextScreen(int x, int y, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      BWAPI::drawTextScreen(BWAPI::Position(x,y),buffer);
    }

    void drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      BWAPI::drawRectangle((BWAPI::CoordinateBase)ctype,BWAPI::Position(left,top),BWAPI::Position(right-left,bottom-top),color.getID(),isSolid);
    }
    void drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      BWAPI::drawRectangleMap(BWAPI::Position(left,top),BWAPI::Position(right-left,bottom-top),color.getID(),isSolid);
    }
    void drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      BWAPI::drawRectangleMouse(BWAPI::Position(left,top),BWAPI::Position(right-left,bottom-top),color.getID(),isSolid);
    }
    void drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      BWAPI::drawRectangleScreen(BWAPI::Position(left,top),BWAPI::Position(right-left,bottom-top),color.getID(),isSolid);
    }

    void drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      BWAPI::drawTriangle((BWAPI::CoordinateBase)ctype,BWAPI::Position(ax,ay),BWAPI::Position(bx,by),BWAPI::Position(cx,cy),color.getID(),isSolid);
    }
    void drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      BWAPI::drawTriangleMap(BWAPI::Position(ax,ay),BWAPI::Position(bx,by),BWAPI::Position(cx,cy),color.getID(),isSolid);
    }
    void drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      BWAPI::drawTriangleMouse(BWAPI::Position(ax,ay),BWAPI::Position(bx,by),BWAPI::Position(cx,cy),color.getID(),isSolid);
    }
    void drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
    {
      BWAPI::drawTriangleScreen(BWAPI::Position(ax,ay),BWAPI::Position(bx,by),BWAPI::Position(cx,cy),color.getID(),isSolid);
    }

    void drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
    {
      BWAPI::drawCircle((BWAPI::CoordinateBase)ctype,BWAPI::Position(x,y),radius,color.getID(),isSolid);
    }
    void drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
    {
      BWAPI::drawCircleMap(BWAPI::Position(x,y),radius,color.getID(),isSolid);
    }
    void drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
    {
      BWAPI::drawCircleMouse(BWAPI::Position(x,y),radius,color.getID(),isSolid);
    }
    void drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
    {
      BWAPI::drawCircleScreen(BWAPI::Position(x,y),radius,color.getID(),isSolid);
    }

    void drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      BWAPI::drawEllipse((BWAPI::CoordinateBase)ctype,BWAPI::Position(x,y),xrad,yrad,color.getID(),isSolid);
    }
    void drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      BWAPI::drawEllipseMap(BWAPI::Position(x,y),xrad,yrad,color.getID(),isSolid);
    }
    void drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      BWAPI::drawEllipseMouse(BWAPI::Position(x,y),xrad,yrad,color.getID(),isSolid);
    }
    void drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
    {
      BWAPI::drawEllipseScreen(BWAPI::Position(x,y),xrad,yrad,color.getID(),isSolid);
    }

    void drawDot(int ctype, int x, int y, Color color)
    {
      BWAPI::drawDot((BWAPI::CoordinateBase)ctype,BWAPI::Position(x,y),color.getID());
    }
    void drawDotMap(int x, int y, Color color)
    {
      BWAPI::drawDotMap(BWAPI::Position(x,y),color.getID());
    }
    void drawDotMouse(int x, int y, Color color)
    {
      BWAPI::drawDotMouse(BWAPI::Position(x,y),color.getID());
    }
    void drawDotScreen(int x, int y, Color color)
    {
      BWAPI::drawDotScreen(BWAPI::Position(x,y),color.getID());
    }
    void drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
    {
      BWAPI::drawLine((BWAPI::CoordinateBase)ctype,BWAPI::Position(x1,y1),BWAPI::Position(x2,y2),color.getID());
    }
    void drawLineMap(int x1, int y1, int x2, int y2, Color color)
    {
      BWAPI::drawLineMap(BWAPI::Position(x1,y1),BWAPI::Position(x2,y2),color.getID());
    }
    void drawLineMouse(int x1, int y1, int x2, int y2, Color color)
    {
      BWAPI::drawLineMouse(BWAPI::Position(x1,y1),BWAPI::Position(x2,y2),color.getID());
    }
    void drawLineScreen(int x1, int y1, int x2, int y2, Color color)
    {
      BWAPI::drawLineScreen(BWAPI::Position(x1,y1),BWAPI::Position(x2,y2),color.getID());
    }
  };
};
