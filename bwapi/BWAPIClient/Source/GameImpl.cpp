#include <BWAPI.h>
#include "GameImpl.h"
#include "ForceImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include "BulletImpl.h"

#include <Util\Foreach.h>
#include <string>

namespace BWAPI
{
  const int BUFFER_SIZE=1024;
  char buffer[BUFFER_SIZE];

  Game* Broodwar = NULL;
  GameImpl::GameImpl(GameData* data)
  {
    this->data=data;
    for(int i=0;i<4;i++)
      forceVector.push_back(ForceImpl(i));
    for(int i=0;i<12;i++)
      playerVector.push_back(PlayerImpl(i));
    for(int i=0;i<10000;i++)
      unitVector.push_back(UnitImpl(i));
    for(int i=0;i<100;i++)
      bulletVector.push_back(BulletImpl(i));
    inGame = false;
  }
  int GameImpl::addShape(BWAPIC::Shape &s)
  {
    data->shapes[data->shapeCount]=s;
    return data->shapeCount++;
  }
  int GameImpl::addString(const char* text)
  {
    strncpy(data->strings[data->stringCount],text,256);
    return data->stringCount++;
  }
  int GameImpl::addText(BWAPIC::Shape &s, const char* text)
  {
    s.extra1=addString(text);
    data->shapes[data->shapeCount]=s;
    return data->shapeCount++;
  }
  int GameImpl::addCommand(BWAPIC::Command &c)
  {
    data->commands[data->commandCount]=c;
    return data->commandCount++;
  }
  int GameImpl::addUnitCommand(BWAPIC::UnitCommand& c)
  {
    data->unitCommands[data->unitCommandCount]=c;
    return data->unitCommandCount++;
  }
  Event GameImpl::makeEvent(BWAPIC::Event e)
  {
    Event e2;
    e2.type=e.type;
    if (e.type==EventType::MatchEnd)
      e2.isWinner=(e.v1 != 0);
    if (e.type==EventType::NukeDetect)
      e2.position=Position(e.v1,e.v2);
    if (e.type==EventType::PlayerLeft)
      e2.player=getPlayer(e.v1);
    if (e.type==EventType::SaveGame || e.type==EventType::SendText)
      e2.text=data->strings[e.v1];
    if (e.type==EventType::ReceiveText)
    {
      e2.player=getPlayer(e.v1);
      e2.text=data->strings[e.v2];
    }
    if (e.type==EventType::UnitDiscover ||
        e.type==EventType::UnitEvade ||
        e.type==EventType::UnitShow ||
        e.type==EventType::UnitHide ||
        e.type==EventType::UnitCreate ||
        e.type==EventType::UnitDestroy ||
        e.type==EventType::UnitRenegade ||
        e.type==EventType::UnitMorph)
      e2.unit=getUnit(e.v1);
    return e2;

  }
  void GameImpl::clearAll()
  {
    //clear everything
    inGame = false;
    startLocations.clear();
    forces.clear();
    players.clear();
    bullets.clear();
    accessibleUnits.clear();
    minerals.clear();
    geysers.clear();
    neutralUnits.clear();
    staticMinerals.clear();
    staticGeysers.clear();
    staticNeutralUnits.clear();
    selectedUnits.clear();
    pylons.clear();
    events.clear();
    thePlayer = NULL;
    theEnemy = NULL;

    //clear unitsOnTileData
    for(int x=0;x<256;x++)
      for(int y=0;y<256;y++)
        unitsOnTileData[x][y].clear();

    //clear unit data
    for(int i=0;i<10000;i++)
      unitVector[i].clear();

    //clear player data
    for(int i=0;i<12;i++)
      playerVector[i].units.clear();

  }
  //------------------------------------------------- ON MATCH START -----------------------------------------
  void GameImpl::onMatchStart()
  {
    clearAll();
    inGame = true;

    //load forces, players, and initial units from shared memory
    for(int i=0;i<data->forceCount;i++)
      forces.insert(&forceVector[i]);
    for(int i=0;i<data->playerCount;i++)
      players.insert(&playerVector[i]);
    for(int i=0;i<data->initialUnitCount;i++)
    {
      accessibleUnits.insert(&unitVector[i]);
      //save the initial state of each initial unit
      unitVector[i].saveInitialState();
    }

    //load start locations from shared memory
    for(int i=0;i<data->startLocationCount;i++)
      startLocations.insert(BWAPI::TilePosition(data->startLocationsX[i],data->startLocationsY[i]));

    thePlayer = getPlayer(data->self);
    if (thePlayer!=NULL)
    {
      foreach(Player* p, players)
      {
        if (thePlayer->isEnemy(p))
          theEnemy=p;
      }
    }
    onMatchFrame();
    staticMinerals = minerals;
    staticGeysers = geysers;
    staticNeutralUnits = neutralUnits;
  }
  //------------------------------------------------- ON MATCH END -------------------------------------------
  void GameImpl::onMatchEnd()
  {
    clearAll();
  }
  //------------------------------------------------- ON MATCH FRAME -----------------------------------------
  void GameImpl::onMatchFrame()
  {
    events.clear();
    bullets.clear();
    for(int i=0;i<100;i++)
    {
      if (bulletVector[i].exists())
      {
        bullets.insert(&bulletVector[i]);
      }
    }
    for (int y = 0; y < data->mapHeight; y++)
      for (int x = 0; x < data->mapWidth; x++)
        unitsOnTileData[x][y].clear();
    for(int e=0; e<data->eventCount; e++)
    {
      events.push_back(this->makeEvent(data->events[e]));
      int id=data->events[e].v1;
      if (data->events[e].type == EventType::UnitDiscover)
      {
        Unit* u=&unitVector[id];
        accessibleUnits.insert(u);
        ((PlayerImpl*)u->getPlayer())->units.insert(u);
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
          if (u->getPlayer() == Broodwar->self() && u->getType() == UnitTypes::Protoss_Pylon)
            pylons.insert(u);
        }
      }
      else if (data->events[e].type==EventType::UnitEvade)
      {
        Unit* u=&unitVector[id];
        accessibleUnits.erase(u);
        ((PlayerImpl*)u->getPlayer())->units.erase(u);
        if (u->getPlayer()->isNeutral())
        {
          neutralUnits.erase(u);
          if (u->getType()==UnitTypes::Resource_Mineral_Field)
            minerals.erase(u);
          else if (u->getType()==UnitTypes::Resource_Vespene_Geyser)
            geysers.erase(u);
        }
        else
        {
          if (u->getPlayer() == Broodwar->self() && u->getType() == UnitTypes::Protoss_Pylon)
            pylons.erase(u);
        }
      }
      else if (data->events[e].type==EventType::UnitRenegade)
      {
        Unit* u=&unitVector[id];
        for each(Player* p in players)
          ((PlayerImpl*)p)->units.erase(u);
        ((PlayerImpl*)u->getPlayer())->units.insert(u);
      }
      else if (data->events[e].type==EventType::UnitMorph)
      {
        Unit* u=&unitVector[id];
        if (u->getType()==UnitTypes::Resource_Vespene_Geyser)
        {
          geysers.insert(u);
          neutralUnits.insert(u);
        }
        else if (u->getType().isRefinery())
        {
          geysers.erase(u);
          neutralUnits.erase(u);
        }
      }
    }
    foreach(Unit* u, accessibleUnits)
    {
      ((UnitImpl*)u)->connectedUnits.clear();
      ((UnitImpl*)u)->loadedUnits.clear();
    }
    foreach(Unit* u, accessibleUnits)
    {
      int startX = (u->getPosition().x() - u->getType().dimensionLeft()) / TILE_SIZE;
      int endX   = (u->getPosition().x() + u->getType().dimensionRight() + TILE_SIZE - 1) / TILE_SIZE; // Division - round up
      int startY = (u->getPosition().y() - u->getType().dimensionUp()) / TILE_SIZE;
      int endY   = (u->getPosition().y() + u->getType().dimensionDown() + TILE_SIZE - 1) / TILE_SIZE;
      for (int x = startX; x < endX; x++)
        for (int y = startY; y < endY; y++)
          unitsOnTileData[x][y].insert(u);
      if (u->getType()==UnitTypes::Zerg_Larva && u->getHatchery()!=NULL)
        ((UnitImpl*)u->getHatchery())->connectedUnits.insert(u);
      if (u->getType()==UnitTypes::Protoss_Interceptor && u->getCarrier()!=NULL)
        ((UnitImpl*)u->getCarrier())->connectedUnits.insert(u);
      if (u->getTransport()!=NULL)
        ((UnitImpl*)u->getTransport())->loadedUnits.insert(u);
    }
    selectedUnits.clear();
    for(int i=0;i<data->selectedUnitCount;i++)
    {
      Unit* u=getUnit(data->selectedUnits[i]);
      if (u!=NULL)
        selectedUnits.insert(u);
    }
  }
  //------------------------------------------------- GET FORCES ---------------------------------------------
  std::set< Force* >& GameImpl::getForces()
  {
    return forces;
  }
  //------------------------------------------------- GET PLAYERS --------------------------------------------
  std::set< Player* >& GameImpl::getPlayers()
  {
    return players;
  }
  //------------------------------------------------- GET ALL UNITS ------------------------------------------
  std::set< Unit* >& GameImpl::getAllUnits()
  {
    return accessibleUnits;
  }
  //------------------------------------------------- GET MINERALS -------------------------------------------
  std::set< Unit* >& GameImpl::getMinerals()
  {
    return minerals;
  }
  //------------------------------------------------- GET GEYSERS --------------------------------------------
  std::set< Unit* >& GameImpl::getGeysers()
  {
    return geysers;
  }
  //------------------------------------------------- GET NEUTRAL UNITS --------------------------------------
  std::set< Unit* >& GameImpl::getNeutralUnits()
  {
    return neutralUnits;
  }
  //------------------------------------------------- GET STATIC MINERALS ------------------------------------
  std::set< Unit* >& GameImpl::getStaticMinerals()
  {
    return staticMinerals;
  }
  //------------------------------------------------- GET STATIC GEYSERS -------------------------------------
  std::set< Unit* >& GameImpl::getStaticGeysers()
  {
    return staticGeysers;
  }
  //------------------------------------------------- GET STATIC NEUTRAL UNITS -------------------------------
  std::set< Unit* >& GameImpl::getStaticNeutralUnits()
  {
    return staticNeutralUnits;
  }
  //------------------------------------------------ GET BULLETS ---------------------------------------------
  std::set< Bullet* >& GameImpl::getBullets()
  {
    return bullets;
  }
  //------------------------------------------------ GET EVENTS ----------------------------------------------
  std::list< Event >& GameImpl::getEvents()
  {
    return events;
  }
  //----------------------------------------------- GET FORCE ------------------------------------------------
  Force* GameImpl::getForce(int forceId)
  {
    if (forceId<0 || forceId>=(int)forceVector.size()) return NULL;
    return &forceVector[forceId];
  }
  //----------------------------------------------- GET PLAYER -----------------------------------------------
  Player* GameImpl::getPlayer(int playerId)
  {
    if (playerId<0 || playerId>=(int)playerVector.size()) return NULL;
    return &playerVector[playerId];
  }
  //----------------------------------------------- GET UNIT -------------------------------------------------
  Unit* GameImpl::getUnit(int unitId)
  {
    if (unitId<0 || unitId>=(int)unitVector.size()) return NULL;
    return &unitVector[unitId];
  }
  //----------------------------------------------- INDEX TO UNIT --------------------------------------------
  Unit* GameImpl::indexToUnit(int unitIndex)
  {
    if (unitIndex<0 || unitIndex>=1700)
      return NULL;
    return getUnit(data->unitArray[unitIndex]);
  }
  //--------------------------------------------- GET GAME TYPE ----------------------------------------------
  GameType GameImpl::getGameType()
  {
    return GameType(data->gameType);
  }
  //---------------------------------------------- GET LATENCY -----------------------------------------------
  int GameImpl::getLatency()
  {
    return data->latency;
  }
  //--------------------------------------------- GET FRAME COUNT --------------------------------------------
  int GameImpl::getFrameCount()
  {
    return data->frameCount;
  }
  //------------------------------------------------ GET FPS -------------------------------------------------
  int GameImpl::getFPS()
  {
    return data->fps;
  }
  //------------------------------------------------ GET FPS -------------------------------------------------
  double GameImpl::getAverageFPS()
  {
    return data->averageFPS;
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int GameImpl::getMouseX()
  {
    return data->mouseX;
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int GameImpl::getMouseY()
  {
    return data->mouseY;
  }
  //-------------------------------------------- GET MOUSE POSITION ------------------------------------------
  Position GameImpl::getMousePosition()
  {
    return Position(data->mouseX,data->mouseY);
  }
  //---------------------------------------------- GET MOUSE STATE -------------------------------------------
  bool GameImpl::getMouseState(MouseButton button)
  {
    return data->mouseState[(int)button];
  }
  //---------------------------------------------- GET MOUSE STATE -------------------------------------------
  bool GameImpl::getMouseState(int button)
  {
    return data->mouseState[button];
  }
  //----------------------------------------------- GET KEY STATE --------------------------------------------
  bool GameImpl::getKeyState(Key key)
  {
    return data->keyState[key];
  }
  //----------------------------------------------- GET KEY STATE --------------------------------------------
  bool GameImpl::getKeyState(int key)
  {
    return data->keyState[key];
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int GameImpl::getScreenX()
  {
    return data->screenX;
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int GameImpl::getScreenY()
  {
    return data->screenY;
  }
  //-------------------------------------------- GET SCREEN POSITION -----------------------------------------
  Position GameImpl::getScreenPosition()
  {
    return Position(data->screenX,data->screenY);
  }
  //-------------------------------------------- SET SCREEN POSITION -----------------------------------------
  void GameImpl::setScreenPosition(int x, int y)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetScreenPosition,x,y));
  }
  //-------------------------------------------- SET SCREEN POSITION -----------------------------------------
  void GameImpl::setScreenPosition(Position p)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetScreenPosition,p.x(),p.y()));
  }
  //----------------------------------------------- PING MINIMAP ---------------------------------------------
  void GameImpl::pingMinimap(int x, int y)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::PingMinimap,x,y));
  }
  //----------------------------------------------- PING MINIMAP ---------------------------------------------
  void GameImpl::pingMinimap(Position p)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::PingMinimap,p.x(),p.y()));
  }
  //----------------------------------------------- IS FLAG ENABLED ------------------------------------------
  bool GameImpl::isFlagEnabled(int flag)
  {
    if (flag<0 || flag>=2) return false;
    return data->flags[flag];
  }
  //----------------------------------------------- ENABLE FLAG ----------------------------------------------
  void GameImpl::enableFlag(int flag)
  {
    if (flag<0 || flag>=2) return;
    if (data->flags[flag]==false)
      addCommand(BWAPIC::Command(BWAPIC::CommandType::EnableFlag,flag));
  }
  //----------------------------------------------- UNITS ON TILE --------------------------------------------
  std::set<Unit*>& GameImpl::unitsOnTile(int x, int y)
  {
    return unitsOnTileData[x][y];
  }
  //----------------------------------------------- GET LAST ERROR -------------------------------------------
  Error GameImpl::getLastError() const
  {
    return lastError;
  }
  //----------------------------------------------- MAP WIDTH ------------------------------------------------
  int GameImpl::mapWidth()
  {
    return data->mapWidth;
  }
  //----------------------------------------------- MAP HEIGHT -----------------------------------------------
  int GameImpl::mapHeight()
  {
    return data->mapHeight;
  }
  //---------------------------------------------- MAP FILE NAME ---------------------------------------------
  std::string GameImpl::mapFileName()
  {
    return std::string(data->mapFileName);
  }
  //---------------------------------------------- MAP PATH NAME ---------------------------------------------
  std::string GameImpl::mapPathName()
  {
    return std::string(data->mapPathName);
  }
  //------------------------------------------------ MAP NAME ------------------------------------------------
  std::string GameImpl::mapName()
  {
    return std::string(data->mapName);
  }
  //----------------------------------------------- GET MAP HASH ---------------------------------------------
  std::string GameImpl::mapHash()
  {
    return std::string(data->mapHash);
  }
  //--------------------------------------------- IS WALKABLE ------------------------------------------------
  bool GameImpl::isWalkable(int x, int y)
  {
    if (x<0 || y<0 || x>=data->mapWidth*4 || y>=data->mapHeight*4)
      return 0;
    return data->isWalkable[x][y];
  }
  //--------------------------------------------- GET GROUND HEIGHT ------------------------------------------
  int GameImpl::getGroundHeight(int x, int y)
  {
    if (x<0 || y<0 || x>=data->mapWidth || y>=data->mapHeight)
      return 0;
    return data->getGroundHeight[x][y];
  }
  //--------------------------------------------- GET GROUND HEIGHT ------------------------------------------
  int GameImpl::getGroundHeight(TilePosition position)
  {
    return getGroundHeight(position.x(),position.y());
  }
  //--------------------------------------------- IS BUILDABLE -----------------------------------------------
  bool GameImpl::isBuildable(int x, int y)
  {
    if (x<0 || y<0 || x>=data->mapWidth || y>=data->mapHeight)
      return 0;
    return data->isBuildable[x][y];
  }
  //--------------------------------------------- IS BUILDABLE -----------------------------------------------
  bool GameImpl::isBuildable(TilePosition position)
  {
    return isBuildable(position.x(),position.y());
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool GameImpl::isVisible(int x, int y)
  {
    if (x<0 || y<0 || x>=data->mapWidth || y>=data->mapHeight)
      return 0;
    return data->isVisible[x][y];
  }
  //--------------------------------------------- IS VISIBLE -------------------------------------------------
  bool GameImpl::isVisible(TilePosition position)
  {
    return isVisible(position.x(),position.y());
  }
  //--------------------------------------------- IS EXPLORED ------------------------------------------------
  bool GameImpl::isExplored(int x, int y)
  {
    if (x<0 || y<0 || x>=data->mapWidth || y>=data->mapHeight)
      return 0;
    return data->isExplored[x][y];
  }
  //--------------------------------------------- IS EXPLORED ------------------------------------------------
  bool GameImpl::isExplored(TilePosition position)
  {
    return isExplored(position.x(),position.y());
  }
  //--------------------------------------------- HAS CREEP --------------------------------------------------
  bool GameImpl::hasCreep(int x, int y)
  {
    if (x<0 || y<0 || x>=data->mapWidth || y>=data->mapHeight)
      return 0;
    return data->hasCreep[x][y];
  }
  //--------------------------------------------- HAS CREEP --------------------------------------------------
  bool GameImpl::hasCreep(TilePosition position)
  {
    return hasCreep(position.x(),position.y());
  }
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPower(int x, int y, int tileWidth, int tileHeight)
  {
    if (!(tileWidth == 2 && tileHeight == 2) &&
        !(tileWidth == 3 && tileHeight == 2) &&
        !(tileWidth == 4 && tileHeight == 3))
      return false;

    if (tileWidth == 4)
      x++;
    /* Loop through all pylons for the current player */
    foreach (Unit* i, pylons)
    {
      if (i->isCompleted()==false) continue;
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
  //--------------------------------------------- HAS POWER --------------------------------------------------
  bool GameImpl::hasPower(TilePosition position, int tileWidth, int tileHeight)
  {
    return hasPower(position.x(),position.y(),tileWidth,tileHeight);
  }
  //--------------------------------------------- CAN BUILD HERE ---------------------------------------------
  bool GameImpl::canBuildHere(Unit* builder, TilePosition position, UnitType type)
  {
    /* Error checking */
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
    if (type.getRace() == BWAPI::Races::Zerg)
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
  //--------------------------------------------- CAN MAKE ---------------------------------------------------
  bool GameImpl::canMake(Unit* builder, UnitType type)
  {
    /* Error checking */
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
      if (builder->getType() != type.whatBuilds().first)
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
      if (self()->supplyTotal() < self()->supplyUsed() + type.supplyRequired() - type.whatBuilds().first.supplyRequired())
      {
        lastError = Errors::Insufficient_Supply;
        return false;
      }
    UnitType addon = UnitTypes::None;
    for(std::map<UnitType, int>::const_iterator i = type.requiredUnits().begin(); i != type.requiredUnits().end(); i++)
      if (i->first.isAddon())
        addon=i->first;
    for(std::map<UnitType, int>::const_iterator i = type.requiredUnits().begin(); i != type.requiredUnits().end(); i++)
    {
      bool pass = false;
      if (self()->completedUnitCount(i->first) >= i->second)
        pass = true;
      if (i->first == UnitTypes::Zerg_Hatchery)
      {
        if (self()->completedUnitCount(UnitTypes::Zerg_Lair) >= i->second)
          pass = true;
        if (self()->completedUnitCount(UnitTypes::Zerg_Hive) >= i->second)
          pass = true;
      }
      if (i->first == UnitTypes::Zerg_Lair)
        if (self()->completedUnitCount(UnitTypes::Zerg_Hive) >= i->second)
          pass = true;
      if (pass == false)
      {
        lastError = Errors::Insufficient_Tech;
        return false;
      }
    }
    if (type.requiredTech() != TechTypes::None)
      if (!self()->hasResearched(type.requiredTech()))
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
  //--------------------------------------------- CAN RESEARCH -----------------------------------------------
  bool GameImpl::canResearch(Unit* unit, TechType type)
  {
    /* Error checking */
    lastError = Errors::None;
    if (self() == NULL)
    {
      lastError = Errors::Unit_Not_Owned;
      return false;
    }
    if (unit != NULL)
    {
      if (unit->getPlayer()!=self())
      {
        lastError = Errors::Unit_Not_Owned;
        return false;
      }
      if (unit->getType() != type.whatResearches())
      {
        lastError = Errors::Incompatible_UnitType;
        return false;
      }
    }
    if (self()->hasResearched(type))
    {
      lastError = Errors::Already_Researched;
      return false;
    }
    if (self()->minerals() < type.mineralPrice())
    {
      lastError = Errors::Insufficient_Minerals;
      return false;
    }
    if (self()->gas() < type.gasPrice())
    {
      lastError = Errors::Insufficient_Gas;
      return false;
    }
    return true;
  }
  //--------------------------------------------- CAN UPGRADE ------------------------------------------------
  bool GameImpl::canUpgrade(Unit* unit, UpgradeType type)
  {
    /* Error checking */
    lastError = Errors::None;
    if (self() == NULL)
    {
      lastError = Errors::Unit_Not_Owned;
      return false;
    }
    if (unit != NULL)
    {
      if (unit->getPlayer()!=self())
      {
        lastError = Errors::Unit_Not_Owned;
        return false;
      }
      if (unit->getType() != type.whatUpgrades())
      {
        lastError = Errors::Incompatible_UnitType;
        return false;
      }
    }
    if (self()->getUpgradeLevel(type)>=type.maxRepeats())
    {
      lastError = Errors::Fully_Upgraded;
      return false;
    }
    if (self()->minerals() < type.mineralPriceBase()+type.mineralPriceFactor()*(self()->getUpgradeLevel(type)))
    {
      lastError = Errors::Insufficient_Minerals;
      return false;
    }
    if (self()->gas() < type.gasPriceBase()+type.gasPriceFactor()*(self()->getUpgradeLevel(type)))
    {
      lastError = Errors::Insufficient_Gas;
      return false;
    }
    return true;
  }
  //--------------------------------------------- GET START LOCATIONS ----------------------------------------
  std::set< TilePosition >& GameImpl::getStartLocations()
  {
    return startLocations;
  }
  //------------------------------------------------ PRINTF --------------------------------------------------
  void GameImpl::printf(const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    addCommand(BWAPIC::Command(BWAPIC::CommandType::Printf,addString(buffer)));
    return;
  }
  //--------------------------------------------- SEND TEXT --------------------------------------------------
  void GameImpl::sendText(const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SendText,addString(buffer)));
    return;
  }
  //--------------------------------------------- SEND TEXT EX -----------------------------------------------
  void GameImpl::sendTextEx(bool toAllies, const char *format, ...)
  {
    return; //todo: implement
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void GameImpl::changeRace(Race race)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::ChangeRace));
  }
  //----------------------------------------------- IS IN GAME -----------------------------------------------
  bool GameImpl::isInGame()
  {
    return data->isInGame;
  }
  //--------------------------------------------- IS MULTIPLAYER ---------------------------------------------
  bool GameImpl::isMultiplayer()
  {
    return data->isMultiplayer;
  }  
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::isBattleNet()
  {
    return data->isBattleNet;
  }
  //----------------------------------------------- IS PAUSED ------------------------------------------------
  bool GameImpl::isPaused()
  {
    return data->isPaused;
  }
  //----------------------------------------------- IS REPLAY ------------------------------------------------
  bool GameImpl::isReplay()
  {
    return data->isReplay;
  }
  //----------------------------------------------- START GAME -----------------------------------------------
  void GameImpl::startGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::StartGame));
  }
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void GameImpl::pauseGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::PauseGame));
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void GameImpl::resumeGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::ResumeGame));
  }
  //---------------------------------------------- LEAVE GAME ------------------------------------------------
  void GameImpl::leaveGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::LeaveGame));
  }
  //--------------------------------------------- RESTART GAME -----------------------------------------------
  void GameImpl::restartGame()
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::RestartGame));
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setLocalSpeed(int speed)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetLocalSpeed,speed));
  }
  //------------------------------------------ GET SELECTED UNITS --------------------------------------------
  std::set<Unit*>& GameImpl::getSelectedUnits()
  {
    return selectedUnits;
  }
  //------------------------------------------------- SELF ---------------------------------------------------
  Player* GameImpl::self()
  {
    return thePlayer;
  }
  //------------------------------------------------- ENEMY --------------------------------------------------
  Player* GameImpl::enemy()
  {
    return theEnemy;
  }
  //---------------------------------------------- SET TEXT SIZE ---------------------------------------------
  void GameImpl::setTextSize(int size)
  {
    addCommand(BWAPIC::Command(BWAPIC::CommandType::SetTextSize, size));
  }
  //-------------------------------------------------- DRAW --------------------------------------------------
  void GameImpl::drawText(int ctype, int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addText(BWAPIC::Shape(BWAPIC::ShapeType::Text,ctype,x,y,0,0,0,0,0,false),buffer);
  }
  void GameImpl::drawTextMap(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addText(BWAPIC::Shape(BWAPIC::ShapeType::Text,(int)BWAPI::CoordinateType::Map,x,y,0,0,0,0,0,false),buffer);
  }
  void GameImpl::drawTextMouse(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addText(BWAPIC::Shape(BWAPIC::ShapeType::Text,(int)BWAPI::CoordinateType::Mouse,x,y,0,0,0,0,0,false),buffer);
  }
  void GameImpl::drawTextScreen(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addText(BWAPIC::Shape(BWAPIC::ShapeType::Text,(int)BWAPI::CoordinateType::Screen,x,y,0,0,0,0,0,false),buffer);
  }

  void GameImpl::drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Box,ctype,left,top,right,bottom,0,0,color.getID(),isSolid));
  }
  void GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Box,(int)BWAPI::CoordinateType::Map,left,top,right,bottom,0,0,color.getID(),isSolid));
  }
  void GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Box,(int)BWAPI::CoordinateType::Mouse,left,top,right,bottom,0,0,color.getID(),isSolid));
  }
  void GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Box,(int)BWAPI::CoordinateType::Screen,left,top,right,bottom,0,0,color.getID(),isSolid));
  }

  void GameImpl::drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Triangle,ctype,ax,ay,bx,by,cx,cy,color.getID(),isSolid));
  }
  void GameImpl::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Triangle,(int)BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy,color.getID(),isSolid));
  }
  void GameImpl::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Triangle,(int)BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy,color.getID(),isSolid));
  }
  void GameImpl::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Triangle,(int)BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy,color.getID(),isSolid));
  }

  void GameImpl::drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Circle,ctype,x,y,0,0,radius,0,color.getID(),isSolid));
  }
  void GameImpl::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Circle,(int)BWAPI::CoordinateType::Map,x,y,0,0,radius,0,color.getID(),isSolid));
  }
  void GameImpl::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Circle,(int)BWAPI::CoordinateType::Mouse,x,y,0,0,radius,0,color.getID(),isSolid));
  }
  void GameImpl::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Circle,(int)BWAPI::CoordinateType::Screen,x,y,0,0,radius,0,color.getID(),isSolid));
  }

  void GameImpl::drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Ellipse,ctype,x,y,0,0,xrad,yrad,color.getID(),isSolid));
  }
  void GameImpl::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Ellipse,(int)BWAPI::CoordinateType::Map,x,y,0,0,xrad,yrad,color.getID(),isSolid));
  }
  void GameImpl::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Ellipse,(int)BWAPI::CoordinateType::Mouse,x,y,0,0,xrad,yrad,color.getID(),isSolid));
  }
  void GameImpl::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Ellipse,(int)BWAPI::CoordinateType::Screen,x,y,0,0,xrad,yrad,color.getID(),isSolid));
  }

  void GameImpl::drawDot(int ctype, int x, int y, Color color)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Dot,ctype,x,y,0,0,0,0,color.getID(),false));
  }
  void GameImpl::drawDotMap(int x, int y, Color color)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Dot,(int)BWAPI::CoordinateType::Map,x,y,0,0,0,0,color.getID(),false));
  }
  void GameImpl::drawDotMouse(int x, int y, Color color)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Dot,(int)BWAPI::CoordinateType::Mouse,x,y,0,0,0,0,color.getID(),false));
  }
  void GameImpl::drawDotScreen(int x, int y, Color color)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Dot,(int)BWAPI::CoordinateType::Screen,x,y,0,0,0,0,color.getID(),false));
  }

  void GameImpl::drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Line,ctype,x1,y1,x2,y2,0,0,color.getID(),false));
  }
  void GameImpl::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Line,(int)BWAPI::CoordinateType::Map,x1,y1,x2,y2,0,0,color.getID(),false));
  }
  void GameImpl::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Line,(int)BWAPI::CoordinateType::Mouse,x1,y1,x2,y2,0,0,color.getID(),false));
  }
  void GameImpl::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    addShape(BWAPIC::Shape(BWAPIC::ShapeType::Line,(int)BWAPI::CoordinateType::Screen,x1,y1,x2,y2,0,0,color.getID(),false));
  }
  void* GameImpl::getScreenBuffer()
  {
    return (void*)NULL;
  }
  int GameImpl::getLatencyFrames()
  {
    return data->latencyFrames;
  }
  int GameImpl::getLatencyTime()
  {
    return data->latencyTime;
  }
  int GameImpl::getRemainingLatencyFrames()
  {
    return data->remainingLatencyFrames;
  }
  int GameImpl::getRemainingLatencyTime()
  {
    return data->remainingLatencyTime;
  }
  int GameImpl::getRevision()
  {
    return data->revision;
  }
  bool GameImpl::isDebug()
  {
    return data->isDebug;
  }
};
