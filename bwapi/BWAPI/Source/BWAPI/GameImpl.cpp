#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "../../svnrev.h"
#include "GameImpl.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>

#include <Util/FileLogger.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/ForceImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/Command.h>
#include <BWAPI/CommandCancelTrain.h>
#include <BWAPI/Map.h>
#include <BWAPI/ScreenLogger.h>
#include <BWAPI/Flag.h>
#include <BWAPI.h>

#include <BW/Unit.h>
#include <BW/Offsets.h>
#include <BW/UnitTarget.h>
#include <BW/OrderTypes.h>
#include <BW/Latency.h>
#include <BW/TileType.h>
#include <BW/TileSet.h>
#include <BW/UnitType.h>
#include <BW/GameType.h>
#include <BW/WeaponType.h>
#include <BW/CheatType.h>

#include "BWAPI/AIModule.h"
#include "DLLMain.h"

#include "ShapeBox.h"
#include "ShapeCircle.h"
#include "ShapeEllipse.h"
#include "ShapeDot.h"
#include "ShapeLine.h"
#include "ShapeTriangle.h"
#include "ShapeText.h"

namespace BWAPI
{
  Game* Broodwar;
  GameImpl BroodwarImpl;

  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  GameImpl::GameImpl()
      : onStartCalled(false)
      , unitsOnTileData(0, 0)
      , enabled(true)
      , client(NULL)
      , startedClient(false)
      , inUpdate(false)
      , inGame(false)
      , calledOnEnd(false)
      , autoMenuGameType(0)
  {
    BWAPI::Broodwar = static_cast<Game*>(this);

    /* initialize the unit types */
    BW::UnitType::initialize();

    try
    {
      /* create log handles */
      if (logging)
      {
        this->commandLog = new Util::FileLogger(std::string(logPath) + "\\commands", Util::LogLevel::MicroDetailed);
        this->newUnitLog = new Util::FileLogger(std::string(logPath) + "\\new_unit_id", Util::LogLevel::MicroDetailed);
      }
      else
      {
        this->commandLog = new Util::FileLogger(std::string(logPath) + "\\commands", Util::LogLevel::DontLog);
        this->newUnitLog = new Util::FileLogger(std::string(logPath) + "\\new_unit_id", Util::LogLevel::DontLog);
      }
      this->autoMenuGameType = GetPrivateProfileIntA("config", "menu", 0, "bwapi-data\\bwapi.ini");
      unitArrayCopyLocal = new BW::UnitArray;

      /* iterate through players and create PlayerImpl for each */
      for (int i = 0; i < BW::PLAYER_COUNT; i++)
        players[i] = new PlayerImpl((u8)i);

      /* iterate through units and create UnitImpl for each */
      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
        unitArray[i] = new UnitImpl(&BW::BWDATA_UnitNodeTable->unit[i],
                                    &unitArrayCopyLocal->unit[i],
                                    (u16)i);

      /* iterate through unit types and create UnitType for each */
      for (int i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        unitTypes.insert(BW::UnitType((u16)i));
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error.txt", "a+");
      fprintf(f, "Exception caught inside Game constructor: %s\n", exception.getMessage().c_str());
      fclose(f);
    }
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  GameImpl::~GameImpl()
  {
    delete unitArrayCopyLocal;

    /* destroy all UnitImpl */
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete unitArray[i];

    /* destroy all PlayerImpl */
    for (int i = 0; i < BW::PLAYER_COUNT; i++)
      delete players[i];

    /* destroy all log handles */
    delete this->commandLog;
    delete this->newUnitLog;
  }
  //----------------------------------------------- MAP WIDTH ------------------------------------------------
  int  GameImpl::mapWidth()
  {
    /* Get the width of the map */
    this->setLastError(Errors::None);
    return Map::getWidth();
  }
  //----------------------------------------------- MAP HEIGHT -----------------------------------------------
  int  GameImpl::mapHeight()
  {
    /* Get the height of the map */
    this->setLastError(Errors::None);
    return Map::getHeight();
  }
  //---------------------------------------------- MAP FILENAME ----------------------------------------------
  std::string  GameImpl::mapFilename()
  {
    /* Get the map file name */
    this->setLastError(Errors::None);
    return Map::getFileName();
  }
  //------------------------------------------------ MAP NAME ------------------------------------------------
  std::string  GameImpl::mapName()
  {
    /* Get the name of the map */
    this->setLastError(Errors::None);
    return Map::getName();
  }
  //---------------------------------------------- GROUND HEIGHT ---------------------------------------------
  int  GameImpl::getGroundHeight(int x, int y)
  {
    /* Return the ground height */
    this->setLastError(Errors::None);
    return this->map.groundHeight(x, y);
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool  GameImpl::isBuildable(int x, int y)
  {
    /* Check if the specified coordinates are buildable */
    this->setLastError(Errors::None);
    return this->map.buildable(x, y);
  }
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool  GameImpl::isBuildable(TilePosition position)
  {
    /* Check if the specified coordinates are buildable */
    this->setLastError(Errors::None);
    return this->map.buildable(position.x(), position.y());
  }
  //------------------------------------------------ WALKABLE ------------------------------------------------
  bool  GameImpl::isWalkable(int x, int y)
  {
    /* Check if the specified coordinates are walkable */
    this->setLastError(Errors::None);
    return this->map.walkable(x, y);
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool  GameImpl::isVisible(int x, int y)
  {
    /* Check if the specified coordinates are visible */
    this->setLastError(Errors::None);
    return this->map.visible(x, y);
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool  GameImpl::isVisible(TilePosition position)
  {
    /* Check if the specified coordinates are visible */
    this->setLastError(Errors::None);
    return this->map.visible(position.x(), position.y());
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool  GameImpl::isExplored(int x, int y)
  {
    /* Check if the specified coordinates are visible */
    this->setLastError(Errors::None);
    return this->map.isExplored(x, y);
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool  GameImpl::isExplored(TilePosition position)
  {
    /* Check if the specified coordinates are visible */
    this->setLastError(Errors::None);
    return this->map.isExplored(position.x(), position.y());
  }
  //------------------------------------------------ HAS CREEP -----------------------------------------------
  bool  GameImpl::hasCreep(int x, int y)
  {
    /* Check if the specified coordinates have creep */
    this->setLastError(Errors::None);
    /* Deny this information if you don't have complete map information */
    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !this->isVisible(x, y))
    {
      this->setLastError(Errors::Access_Denied);
      return false;
    }
    return this->map.hasCreep(x, y);
  }
  //------------------------------------------------ HAS CREEP -----------------------------------------------
  bool  GameImpl::hasCreep(TilePosition position)
  {
    return this->hasCreep(position.x(), position.y());
  }
  //------------------------------------------------ HAS POWER -----------------------------------------------
  bool  GameImpl::hasPower(int x, int y, int tileWidth, int tileHeight)
  {
    /* Check if the specified coordinates have power */
    this->setLastError(Errors::None);
    if (!(tileWidth == 2 && tileHeight == 2) &&
        !(tileWidth == 3 && tileHeight == 2) &&
        !(tileWidth == 4 && tileHeight == 3))
    {
      return false;
    }
    if (tileWidth == 4)
    {
      x++;
    }
    /* Loop through all pylons for the current player */
    foreach (UnitImpl* i, myPylons)
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
  //------------------------------------------------ HAS POWER -----------------------------------------------
  bool  GameImpl::hasPower(TilePosition position, int tileWidth, int tileHeight)
  {
    return this->hasPower(position.x(),position.y(),tileWidth,tileHeight);
  }
  //---------------------------------------------- CAN BUILD HERE --------------------------------------------
  bool  GameImpl::canBuildHere(Unit* builder, TilePosition position, UnitType type)
  {
    this->setLastError(Errors::Unbuildable_Location);
    if (position.x() < 0)
      return false;
    if (position.y() < 0)
      return false;
    int width  = type.tileWidth();
    int height = type.tileHeight();
    if (position.x() + width > this->mapWidth())
      return false;
    if (position.y() + height >= this->mapHeight())
      return false;
    if (type.isRefinery())
    {
      foreach (Unit* g, getGeysers())
      {
        if (g->getTilePosition() == position)
        {
          if (g->isVisible() && g->getType()!=UnitTypes::Resource_Vespene_Geyser)
            return false;
          this->setLastError(Errors::None);
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

        if (!this->isBuildable(x,y) || groundUnits.size() > 1)
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
            if (!BWAPI::Broodwar->hasCreep(x, y))
              return false;
    }
    else
    {
      for(int x = position.x(); x < position.x() + width; x++)
        for(int y = position.y(); y < position.y() + height; y++)
          if (BWAPI::Broodwar->hasCreep(x, y))
            return false;
    }

    if (type.requiresPsi())
    {
      if (this->hasPower(position.x(), position.y(), width, height))
      {
        this->setLastError(Errors::None);
        return true;
      }
      return false;
    }

    if (type.isResourceDepot())
    {
      foreach (BWAPI::Unit* m, getStaticMinerals())
      {
        if (isVisible(m->getInitialTilePosition()) ||
            isVisible(m->getInitialTilePosition().x() + 1, m->getInitialTilePosition().y()))
          if (!m->isVisible())
            continue; // tile position is visible, but mineral is not => mineral does not exist
        if (m->getInitialTilePosition().x() > position.x() - 5 &&
            m->getInitialTilePosition().y() > position.y() - 4 &&
            m->getInitialTilePosition().x() < position.x() + 7 &&
            m->getInitialTilePosition().y() < position.y() + 6)
        {
          return false;
        }
      }
      foreach (BWAPI::Unit* g, getStaticGeysers())
      {
        if (g->getInitialTilePosition().x() > position.x() - 7 &&
            g->getInitialTilePosition().y() > position.y() - 5 &&
            g->getInitialTilePosition().x() < position.x() + 7 &&
            g->getInitialTilePosition().y() < position.y() + 6)
        {
          return false;
        }
      }
    }
    this->setLastError(Errors::None);
    return true;
  }
  //------------------------------------------------- CAN MAKE -----------------------------------------------
  bool  GameImpl::canMake(Unit* builder, UnitType type)
  {
    /* Error checking */
    this->setLastError(Errors::None);
    if (self() == NULL)
    {
      this->setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if (builder != NULL)
    {
      /* Check if the owner of the unit is you */
      if (builder->getPlayer() != self())
      {
        this->setLastError(Errors::Unit_Not_Owned);
        return false;
      }

      /* Check if this unit can actually build the unit type */
      if (builder->getType() != type.whatBuilds().first)
      {
        this->setLastError(Errors::Incompatible_UnitType);
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
          this->setLastError(Errors::Insufficient_Space);
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
          this->setLastError(Errors::Insufficient_Space);
          return false;
        }
      }
    }

    /* Check if player has enough minerals */
    if (self()->minerals() < type.mineralPrice())
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }

    /* Check if player has enough gas */
    if (self()->gas() < type.gasPrice())
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    
    /* Check if player has enough supplies */
    if (type.supplyRequired() > 0)
      if (self()->supplyTotal() < self()->supplyUsed() + type.supplyRequired() - type.whatBuilds().first.supplyRequired())
      {
        this->setLastError(Errors::Insufficient_Supply);
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
        this->setLastError(Errors::Insufficient_Tech);
        return false;
      }
    }

    if (type.requiredTech() != TechTypes::None)
      if (!self()->hasResearched(type.requiredTech()))
      {
        this->setLastError(Errors::Insufficient_Tech);
        return false;
      }

    if (builder != NULL)
      if (addon != UnitTypes::None && addon.whatBuilds().first==type.whatBuilds().first)
        if (builder->getAddon() == NULL || builder->getAddon()->getType() != addon)
        {
          this->setLastError(Errors::Insufficient_Tech);
          return false;
        }
    return true;
  }
  //----------------------------------------------- CAN RESEARCH ---------------------------------------------
  bool  GameImpl::canResearch(Unit* unit, TechType type)
  {
    /* Error checking */
    this->setLastError(Errors::None);
    if (self() == NULL)
    {
      this->setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if (unit != NULL)
    {
      if (unit->getPlayer()!=self())
      {
        this->setLastError(Errors::Unit_Not_Owned);
        return false;
      }
      if (unit->getType() != type.whatResearches())
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }
    }
    if (self()->hasResearched(type))
    {
      this->setLastError(Errors::Already_Researched);
      return false;
    }
    if (self()->minerals() < type.mineralPrice())
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }
    if (self()->gas() < type.gasPrice())
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    return true;
  }
  //----------------------------------------------- CAN UPGRADE ----------------------------------------------
  bool  GameImpl::canUpgrade(Unit* unit, UpgradeType type)
  {
    this->setLastError(Errors::None);
    if (self() == NULL)
    {
      this->setLastError(Errors::Unit_Not_Owned);
      return false;
    }

    if (unit != NULL)
    {
      if (unit->getPlayer()!=self())
      {
        this->setLastError(Errors::Unit_Not_Owned);
        return false;
      }
      if (unit->getType() != type.whatUpgrades())
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }
    }
    if (self()->getUpgradeLevel(type)>=type.maxRepeats())
    {
      this->setLastError(Errors::Fully_Upgraded);
      return false;
    }
    if (self()->minerals() < type.mineralPriceBase()+type.mineralPriceFactor()*(self()->getUpgradeLevel(type)))
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }
    if (self()->gas() < type.gasPriceBase()+type.gasPriceFactor()*(self()->getUpgradeLevel(type)))
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    return true;
  }
  //--------------------------------------------- GET START LOCATIONS ----------------------------------------
  std::set< TilePosition >& GameImpl::getStartLocations()
  {
    /* Return the set of Start Locations */
    this->setLastError(Errors::None);
    return this->startLocations;
  }
  //----------------------------------------------- GET MAP HASH ---------------------------------------------
  int  GameImpl::getMapHash()
  {
    /* Return a hash of the map's terrain */
    this->setLastError(Errors::None);
    return BWAPI::Map::getMapHash();
  }
  //----------------------------------------------- GET FORCES -----------------------------------------------
  std::set< Force* >& GameImpl::getForces()
  {
    /* Return a set of forces */
    this->setLastError(Errors::None);
    return this->forces;
  }
  //----------------------------------------------- GET PLAYERS ----------------------------------------------
  std::set< Player* >& GameImpl::getPlayers()
  {
    this->setLastError(Errors::None);
    return this->playerSet;
  }
  //------------------------------------------------- GET UNITS ----------------------------------------------
  std::set< Unit* >& GameImpl::getAllUnits()
  {
    this->setLastError(Errors::None);
    return this->allUnits;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  std::set< Unit* >& GameImpl::getMinerals()
  {
    this->setLastError(Errors::None);
    return this->minerals;
  }
  //---------------------------------------------- GET GEYSERS -----------------------------------------------
  std::set< Unit* >& GameImpl::getGeysers()
  {
    this->setLastError(Errors::None);
    return this->geysers;
  }
  //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
  std::set< Unit* >& GameImpl::getNeutralUnits()
  {
    this->setLastError(Errors::None);
    return this->neutralUnits;
  }
  //---------------------------------------------- GET MINERALS ----------------------------------------------
  std::set< Unit* >& GameImpl::getStaticMinerals()
  {
    this->setLastError(Errors::None);
    return this->staticMinerals;
  }
  //---------------------------------------------- GET GEYSERS -----------------------------------------------
  std::set< Unit* >& GameImpl::getStaticGeysers()
  {
    this->setLastError(Errors::None);
    return this->staticGeysers;
  }
  //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
  std::set< Unit* >& GameImpl::getStaticNeutralUnits()
  {
    this->setLastError(Errors::None);
    return this->staticNeutralUnits;
  }
  //---------------------------------------------- GET EVENTS ------------------------------------------------
  std::list< Event >& GameImpl::getEvents()
  {
    this->setLastError(Errors::None);
    return this->events;
  }

  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  void GameImpl::IssueCommand(PBYTE pbBuffer, u32 iSize)
  {
    __asm
    {
      mov ecx, pbBuffer
      mov edx, iSize
    }
    NewIssueCommand();
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void GameImpl::update()
  {
    this->inGame=true;
    try
    {
      this->inUpdate = true;
      if (!this->isOnStartCalled())
        this->onGameStart();
      
      if (!this->enabled)
        return;
      if (this->client != NULL && this->calledOnEnd == false)
      {
        if (this->BWAPIPlayer != NULL)
        {
          if (this->BWAPIPlayer->isVictorious())
          {
            this->client->onEnd(true);
            events.push_back(Event::MatchEnd(true));
            this->calledOnEnd = true;
          }
          if (this->BWAPIPlayer->isDefeated())
          {
            this->client->onEnd(false);
            events.push_back(Event::MatchEnd(false));
            this->calledOnEnd = true;
          }
        }
        else
        {
          bool allDone = true;
          foreach(Player* p, this->playerSet)
          {
            if (p->getID() >= 8) continue;
            if (!p->isDefeated() && !p->isVictorious() && !p->leftGame())
              allDone=false;
          }
          if (allDone)
          {
            this->client->onEnd(false);
            events.push_back(Event::MatchEnd(false));
            this->calledOnEnd = true;
          }
        }
      }

      memcpy(this->unitArrayCopyLocal, BW::BWDATA_UnitNodeTable, sizeof(BW::UnitArray));
      refreshSelectionStates();

      for (int i = 0; i < BW::PLAYER_COUNT; i++)
      {
        bool prevLeftGame=this->players[i]->leftGame();
        this->players[i]->update();
        if (!prevLeftGame && this->players[i]->leftGame())
        {
          if (this->client!=NULL)
          {
            this->client->onPlayerLeft((Player*)this->players[i]);
            events.push_back(Event::PlayerLeft((Player*)this->players[i]));
          }
        }
      }

      std::list<UnitImpl*> unitList;
      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      {
        this->getUnit(i)->buildUnit = NULL;
        this->getUnit(i)->larva.clear();
      }
        

      for (UnitImpl* i = this->getFirst(); i != NULL; i = i->getNext())
        unitList.push_back(i);

/*
      for(BW::AttackType *curritem = *BW::BWDATA_AttackNodeTable_FirstElement ; curritem; curritem = curritem->next)
      {
        Broodwar->drawTextMap(curritem->pos_x, curritem->pos_y, "%s frames: %d", AttackType(curritem->type).getName().c_str(), curritem->time_left>>8);
        Broodwar->drawCircle(BWAPI::CoordinateType::Map, curritem->pos_x, curritem->pos_y, 4, BWAPI::Colors::White, false);
      }
*/
      this->unitsToBeAdded.clear();
      std::list<Position> detectedNukes;
      foreach (UnitImpl* i, unitList)
      {
        if (this->units.find(i) == this->units.end())
        {
          i->alive = true;
          this->units.insert(i);
          this->unitsToBeAdded.insert(i);
        }
      }
      foreach (UnitImpl* i, units)
      {
        if (i->_getOrderTarget() != NULL && i->_getOrderTarget()->exists() && i->getBWOrder() == BW::OrderID::ConstructingBuilding)
        {
          UnitImpl* j = (UnitImpl*)(i->_getOrderTarget());
          i->buildUnit = j;
          j->buildUnit = i;
        }
        if (i->getHatchery() != NULL)
          ((UnitImpl*)i->getHatchery())->larva.insert((Unit*)i);
        if (i->getType() == UnitTypes::Terran_Nuclear_Missile &&
            i->nukeDetected == false &&
            i->getRawDataLocal()->connectedUnit->unitID == BW::UnitID::Terran_Ghost)
        {
          i->nukeDetected = true;
          BW::Position bwtarget = i->getRawDataLocal()->orderTargetPos;
          Position target(bwtarget.x, bwtarget.y);
          if (this->client)
          {
            if (this->isFlagEnabled(Flag::CompleteMapInformation) || this->isVisible(target.x() / 32, target.y() / 32))
              detectedNukes.push_back(target);
            else
              detectedNukes.push_back(Positions::Unknown);
          }
        }
      }

      while ((int)(this->commandBuffer.size()) > this->getLatency())
      {
        for (unsigned int i = 0; i < this->commandBuffer[0].size(); i++)
          delete this->commandBuffer[0][i];

        this->commandBuffer.erase(this->commandBuffer.begin());
      }

      this->commandBuffer.push_back(std::vector<Command *>());
      for (unsigned int i = 0; i < this->commandBuffer.size(); i++)
        for (unsigned int j = 0; j < this->commandBuffer[i].size(); j++)
          this->commandBuffer[i][j]->execute();

      this->updateUnits();
      foreach(Position i, detectedNukes)
      {
        this->client->onNukeDetect(i);
        events.push_back(Event::NukeDetect(i));
      }

      for (unsigned int i = 0; i < this->shapes.size(); i++)
        delete this->shapes[i];
      this->shapes.clear();
      this->inUpdate = false;
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error", "a+");
      fprintf(f, "Exception caught inside Game::update: %s", exception.getMessage().c_str());
      fclose(f);
    }

    if (this->startedClient == false)
    {
      sendText("BWAPI revision %s is now live.", SVN_REV_STR);

      TCHAR szDllPath[MAX_PATH];
      GetPrivateProfileStringA("ai", "ai_dll", "NULL", szDllPath, MAX_PATH, "bwapi-data\\bwapi.ini");
      if (_strcmpi(szDllPath, "NULL") == 0)
      {
        sendText("\x06 Could not find ai_dll under ai in \"bwapi-data\\bwapi.ini\".");
        FILE* f = fopen("bwapi-error.txt", "a+");
        fprintf(f, "Could not find ai_dll under ai in \"bwapi-data\\bwapi.ini\".\n");
        fclose(f);
      }

      Util::Logger::globalLog->logCritical("Loading AI DLL from: %s", szDllPath);
      hMod = LoadLibrary(szDllPath);
      if (hMod == NULL)
      {
        Util::Logger::globalLog->logCritical("ERROR: Failed to load the AI Module");
        this->client = new AIModule();
        this->enableFlag(Flag::UserInput);
        this->enableFlag(Flag::CompleteMapInformation);
        this->lockFlags();
        sendText("Error: Failed to load the AI Module");
      }
      else
      {
        Util::Logger::globalLog->logCritical("Loaded AI Module");
        Util::Logger::globalLog->logCritical("Importing by Virtual Function Table from AI DLL");

        typedef AIModule* (*PFNCreateA1)(BWAPI::Game*);
        Util::Logger::globalLog->logCritical("Creating an Object of AIModule");

        PFNCreateA1 newAIModule = (PFNCreateA1)GetProcAddress(hMod, TEXT("newAIModule"));
        this->client = newAIModule(this);
        Util::Logger::globalLog->logCritical("Created an Object of AIModule");
        sendText("BWAPI: Loaded the AI Module: %s", szDllPath);
      }
      this->client->onStart();
      events.push_back(Event::MatchStart());
      this->startedClient = true;
      this->lockFlags();
    }
    this->client->onFrame();
    events.push_back(Event::MatchFrame());
    this->server.update();

    foreach(std::string i, sentMessages)
    {
      bool send = !BroodwarImpl.onSendText(i.c_str());
      if (send)
        BroodwarImpl.sendText(i.c_str());
    }
    this->sentMessages.clear();
    

    this->loadSelected();
    if (!this->isPaused())
      this->frameCount++;
  }
  //---------------------------------------------- ON MENU FRAME ---------------------------------------------
  void GameImpl::onMenuFrame()
  {
    this->inGame=false;
    events.push_back(Event::MenuFrame());
    this->server.update();
    int menu = *BW::BWDATA_glGluesMode;
    if (autoMenuGameType == 0) return;
    if (menu == 0) //main menu
    {
      if (autoMenuGameType == 1 || autoMenuGameType == 2)
        this->pressKey('S');
      if (autoMenuGameType == 3 || autoMenuGameType == 4)
        this->pressKey('M');
      this->pressKey('E');
    }
    if (menu == 5) //registry screen
    {
      this->pressKey('O');
    }
    if (menu == 2) //multiplayer select connection screen
    {
      this->pressKey(VK_DOWN);
      this->pressKey(VK_DOWN);
      this->pressKey(VK_DOWN);
      this->pressKey(VK_DOWN);
      this->pressKey('O');
    }
    if (menu == 22) //single player play custom / load replay selection screen
    {
      if (autoMenuGameType == 1)
        this->pressKey('U');
      if (autoMenuGameType == 2)
        this->pressKey('R');
    }
    if (menu == 11) //create single/multi player game screen
    {
      //first select map, game type, speed, (and if single player, also race and opponents)
      if (autoMenuGameType == 1) //single player create game screen
      {
      }
      else //multiplayer create game screen
      {
      }
    }
    if (menu == 10) //lan games lobby
    {
      if (autoMenuGameType == 3)
        this->pressKey('G');
    }
    if (menu == 3) //multiplayer game ready screen
    {
    }
    if (menu == 12) //select replay screen
    {
    }
  }
  //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
  void GameImpl::refreshSelectionStates()
  {
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      this->unitArray[i]->setSelected(false);

    this->saveSelected();
    for (int i = 0; savedSelectionStates[i] != NULL; i++)
      BWAPI::UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[i])->setSelected(true);
  }
  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::isMultiplayer()
  {
    this->setLastError(Errors::None);
    return (*BW::BWDATA_IsMultiplayer != 0);
  }
  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::_isSinglePlayer() const
  {
    return (*BW::BWDATA_IsMultiplayer == 0);
  }
  //------------------------------------------- IS ON START CALLED -------------------------------------------
  bool GameImpl::isOnStartCalled() const
  {
    return this->onStartCalled;
  }
  //------------------------------------------ SET ON START CALLED -------------------------------------------
  void GameImpl::setOnStartCalled(bool onStartCalled)
  {
    this->onStartCalled = onStartCalled;
  }
  //------------------------------------------------ IS IN GAME ----------------------------------------------
  bool GameImpl::isInGame()
  {
    this->setLastError(Errors::None);
    return this->inGame;
  }
  //------------------------------------------------ IS IN GAME ----------------------------------------------
  bool GameImpl::_isInGame() const
  {
    return *(BW::BWDATA_InGame) != 0;
  }
  //----------------------------------------------- IS PAUSED ------------------------------------------------
  bool GameImpl::isPaused()
  {
    this->setLastError(Errors::None);
    return *BW::BWDATA_IsNotPaused == 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool  GameImpl::isReplay()
  {
    this->setLastError(Errors::None);
    return *(BW::BWDATA_InReplay) != 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool  GameImpl::_isReplay() const
  {
    return *(BW::BWDATA_InReplay) != 0;
  }
  const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];

  //---------------------------------------------- PRINT WITH PLAYER ID --------------------------------------
  void GameImpl::printEx(int pID, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    char* txtout = buffer;
    if (_isInGame() || _isReplay())
    {
      __asm
      {
        pushad
        push 0       // Unknown
        mov eax, pID   // Player ID (-1 for notification area)
        push txtout  // Text
        call dword ptr [BW::BWFXN_PrintText]
        popad
      }
    }
    else
      sendText(txtout); // until lobby print private text is found
  }
  //------------------------------------------------- PRINTF -------------------------------------------------
  void  GameImpl::printf(const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    if (_isReplay() || _isInGame())
    {
      printEx(8, "%s", buffer);
      return;
    }

    if (!_isInGame())
      printEx(8, "%s", buffer);
  }

  void  GameImpl::sendText(const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    char* txtout = buffer;

    if (_isReplay())
    {
      printEx(8, "%s", buffer);
      return;
    }

    if (_isInGame() && _isSinglePlayer())
    {
      BW::CheatFlags::Enum cheatID = BW::getCheatFlag(text);
      if (cheatID != BW::CheatFlags::None)
      {
        this->cheatFlags ^= cheatID;
        BroodwarImpl.IssueCommand((PBYTE)&BW::Orders::UseCheat(this->cheatFlags), sizeof(BW::Orders::UseCheat));
        if (cheatID == BW::CheatFlags::ShowMeTheMoney ||
            cheatID == BW::CheatFlags::BreatheDeep ||
            cheatID == BW::CheatFlags::WhatsMineIsMine)
          this->cheatFlags ^= cheatID;
      }
      else
      {
        printEx(this->BWAPIPlayer->getID(), "%s", buffer);
      }
      return;
    }

    if (_isInGame())
    {
      memset(BW::BWDATA_SendTextRequired, 0xFF, 2);
      __asm
      {
        pushad
        mov esi, txtout
        call [BW::BWFXN_SendPublicCallTarget]
        popad
      }

    }
    else
      __asm
      {
        pushad
        mov edi, txtout
        call [BW::BWFXN_SendLobbyCallTarget]
        popad
      }
  }
  void GameImpl::pressKey(int key)
  {
    INPUT *keyp          = new INPUT;
    keyp->type           = INPUT_KEYBOARD;
    keyp->ki.wVk         = (WORD)key;
    keyp->ki.dwFlags     = 0;
    keyp->ki.time        = 0;
    keyp->ki.wScan       = 0;
    keyp->ki.dwExtraInfo = 0;
    SendInput(1,keyp,sizeof(INPUT));
    keyp->ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1,keyp,sizeof(INPUT));
  }
  void GameImpl::mouseDown(int x, int y)
  {
    INPUT *i          = new INPUT;
    i->type           = INPUT_MOUSE;
    i->mi.dx          = x;
    i->mi.dy          = y;
    i->mi.dwFlags     = MOUSEEVENTF_LEFTDOWN;
    i->mi.dwExtraInfo = 0;
    SendInput(1,i,sizeof(INPUT));
  }
  void GameImpl::mouseUp(int x, int y)
  {
    INPUT *i          = new INPUT;
    i->type           = INPUT_MOUSE;
    i->mi.dx          = x;
    i->mi.dy          = y;
    i->mi.dwFlags     = MOUSEEVENTF_LEFTUP;
    i->mi.dwExtraInfo = 0;
    SendInput(1,i,sizeof(INPUT));
  }

  //---------------------------------------------- CHANGE SLOT -----------------------------------------------
  void GameImpl::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID), 3);
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void  GameImpl::changeRace(BWAPI::Race race)
  {
    this->setLastError(Errors::None);
    IssueCommand((PBYTE)&BW::Orders::ChangeRace(static_cast<u8>(race.getID()), (u8)this->BWAPIPlayer->getID()), 3);
  }
  //----------------------------------------- ADD TO COMMAND BUFFER ------------------------------------------
  void GameImpl::addToCommandBuffer(Command* command)
  {
    command->execute();
    this->commandBuffer[this->commandBuffer.size() - 1].push_back(command);
    this->commandLog->log("(%4d) %s", this->frameCount, command->describe().c_str());
  }
  //--------------------------------------------- ON GAME START ----------------------------------------------
  void GameImpl::onGameStart()
  {
    /* initialize the variables */
    this->frameCount  = 0;
    this->setOnStartCalled(true);
    this->BWAPIPlayer = NULL;
    this->opponent    = NULL;
    this->calledOnEnd = false;

    /* set all the flags to the default of disabled */
    for (int i = 0; i < FLAG_COUNT; i++)
      this->flags[i] = false;
    this->flagsLocked = false;

    /* load the map data */
    map.load();

    if (*(BW::BWDATA_InReplay)) /* set replay flags */
    {
      for (int i = 0; i < FLAG_COUNT; i++)
        this->flags[i] = true;
      this->flagsLocked = false;
    }
    else
    {
      /* find the current player by name */
      for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
        if (this->players[i] != NULL && strcmp(BW::BWDATA_CurrentPlayer, this->players[i]->getName().c_str()) == 0)
          this->BWAPIPlayer = this->players[i];

      /* error if player not found */
      if (this->BWAPIPlayer == NULL)
      {
        this->commandLog->log("Error: Could not locate BWAPI player.");
        return;
      }

      /* find the opponent player */
      for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
        if ((this->players[i]->playerType() == BW::PlayerType::Computer ||
             this->players[i]->playerType() == BW::PlayerType::Human ||
             this->players[i]->playerType() == BW::PlayerType::ComputerSlot) &&
            this->opponent == NULL &&
            this->BWAPIPlayer->isEnemy(this->players[i]))
          this->opponent = this->players[i];

      /* error if opponent not found */
      if (this->opponent == NULL)
        this->commandLog->log("Warning: Could not find any opponent");
    }

    /* get the start locations */
    BW::Positions* posptr = BW::BWDATA_startPositions;
    this->startLocations.clear();
    this->playerSet.clear();
    this->forces.clear();
    while (posptr->x != 0 || posptr->y != 0)
    {
      startLocations.insert(BWAPI::TilePosition((int)((posptr->x - BW::TILE_SIZE*2)          / BW::TILE_SIZE),
                                                (int)((posptr->y - (int)(BW::TILE_SIZE*1.5)) / BW::TILE_SIZE)));
      posptr++;
    }

    /* get force names */
    std::set<std::string> force_names;
    std::map<std::string, ForceImpl*> force_name_to_forceimpl;
    for (int i = 0; i < BW::PLAYER_COUNT; i++)
      if (this->players[i] != NULL && this->players[i]->getName().length() > 0)
      {
        force_names.insert(std::string(this->players[i]->getForceName()));
        this->playerSet.insert(this->players[i]);
      }

    /* create ForceImpl for force names */
    foreach (std::string i, force_names)
    {
      ForceImpl* newforce = new ForceImpl(i);
      this->forces.insert((Force*)newforce);
      force_name_to_forceimpl.insert(std::make_pair(i, newforce));
    }

    /* create ForceImpl for players */
    for (int i = 0; i < BW::PLAYER_COUNT; i++)
      if (this->players[i] != NULL && this->players[i]->getName().length() > 0)
      {
        ForceImpl* force = force_name_to_forceimpl.find(std::string(this->players[i]->getForceName()))->second;
        force->players.insert(this->players[i]);
        this->players[i]->force = force;
      }
    this->unitsOnTileData.resize(Map::getWidth(), Map::getHeight());
  }
  //------------------------------------------- PLAYER ID CONVERT --------------------------------------------
  int GameImpl::stormIdToPlayerId(int dwStormId)
  {
    /* Get Player ID */
    for (int i = 0; i < BW::PLAYER_COUNT; i++)
    {
      if ( BW::BWDATA_Players->player[i].dwStormId == (u32)dwStormId )
        return i;
    }
    return -1;
  }
  int GameImpl::playerIdToStormId(int dwPlayerId)
  {
    /* Get storm ID */
    return BW::BWDATA_Players->player[dwPlayerId].dwStormId;
  }
  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  bool GameImpl::onSendText(const char* text)
  {
    /* prep onSendText */
    if (this->parseText(text) || !this->isFlagEnabled(BWAPI::Flag::UserInput))
      return true;
    else
    {
      if (this->client != NULL)
      {
        events.push_back(Event::SendText(std::string(text)));
        return !this->client->onSendText(std::string(text));
      }
    }
    return false;
  }
  //---------------------------------------------- ON RECV TEXT ----------------------------------------------
  void GameImpl::onReceiveText(int playerId, std::string text)
  {
    /* Do onReceiveText */
    int realId = stormIdToPlayerId(playerId);
    if ( this->client != NULL && realId != -1 && realId != this->BWAPIPlayer->getID())
      this->client->onReceiveText(this->players[realId], text);
  }
  //----------------------------------------------- PARSE TEXT -----------------------------------------------
  bool GameImpl::parseText(const char* text)
  {
    /* analyze the string */
    std::string message = text;
    std::vector<std::string> parsed = Util::Strings::splitString(message, " ");

    /* fix for bad referencing */
    while (parsed.size() < 5)
      parsed.push_back("");

    /* commands list */
    if (parsed[0] == "/leave")
    {
      this->leaveGame();
      return true;
    }
    else if (parsed[0] == "/latency")
    {
      printf("latency: %d",getLatency());
      return true;
    }
    else if (parsed[0] == "/speed")
    {
      if (parsed[1] != "")
        setLocalSpeed(atoi(parsed[1].c_str()));
      else
        setLocalSpeed();
      return true;
    }
    else if (parsed[0] == "/restart")
    {
      restartGame();
      return true;
    }
    return false;
  }
  //---------------------------------------------- ON GAME END -----------------------------------------------
  void GameImpl::onGameEnd()
  {
    this->setOnStartCalled(false);

    if (this->client != NULL)
    {
      if (this->calledOnEnd == false)
      {
        bool win = true;
        if (this->_isReplay())
          win = false;
        else
        {
          for (UnitImpl* i = this->getFirst(); i != NULL; i = i->getNext())
          {
            if (self()->isEnemy(i->_getPlayer()))
              win = false;
          }
        }
        this->client->onEnd(win);
        events.push_back(Event::MatchEnd(win));
        this->calledOnEnd = true;
      }
      delete this->client;
      this->client = NULL;
    }
    this->units.clear();
    this->forces.clear();
    this->playerSet.clear();
    this->allUnits.clear();
    this->minerals.clear();
    this->geysers.clear();
    this->neutralUnits.clear();
    this->myPylons.clear();
    this->unitsToBeAdded.clear();

    this->staticMinerals.clear();
    this->staticGeysers.clear();
    this->staticNeutralUnits.clear();

    this->commandBuffer.clear();
    FreeLibrary(hMod);
    Util::Logger::globalLog->logCritical("Unloaded AI Module");
    for (int i = 0; i < 13; i++) // Why is this 13? There can only be 12 units selected.
      this->savedSelectionStates[i] = NULL;

    this->invalidIndices.clear();
    this->selectedUnitSet.clear();
    this->startedClient = false;
    foreach (UnitImpl* d, this->deadUnits)
      delete d;

    this->deadUnits.clear();

    for (unsigned int i = 0; i < this->shapes.size(); i++)
      delete this->shapes[i];

    this->shapes.clear();

    for(int i = 0 ; i < BW::PLAYER_COUNT; i++)
      if (this->players[i] != NULL)
        this->players[i]->onGameEnd();
    this->setLocalSpeed(-1);

    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      if (unitArray[i] == NULL)
        continue;
      unitArray[i]->userSelected      = false;
      unitArray[i]->buildUnit         = NULL;
      unitArray[i]->alive             = false;
      unitArray[i]->dead              = false;
      unitArray[i]->savedPlayer       = NULL;
      unitArray[i]->savedUnitType     = NULL;
      unitArray[i]->staticInformation = false;
      unitArray[i]->lastVisible       = false;
      unitArray[i]->lastType          = UnitTypes::Unknown;
      unitArray[i]->lastPlayer        = NULL;
      unitArray[i]->nukeDetected      = false;
    }
    this->cheatFlags  = 0;
    this->calledOnEnd = false;
  }
  //----------------------------------------------- START GAME -----------------------------------------------
  void GameImpl::startGame()
  {
    /* Starts the game as a lobby host */
    this->setLastError(Errors::None);
    this->IssueCommand((PBYTE)&BW::Orders::StartGame(), sizeof(BW::Orders::StartGame));
  }
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void GameImpl::pauseGame()
  {
    /* Pauses the game */
    this->setLastError(Errors::None);
    this->IssueCommand((PBYTE)&BW::Orders::PauseGame(), sizeof(BW::Orders::PauseGame));
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void GameImpl::resumeGame()
  {
    /* Resumes the game */
    this->setLastError(Errors::None);
    this->IssueCommand((PBYTE)&BW::Orders::ResumeGame(), sizeof(BW::Orders::ResumeGame));
  }
  //---------------------------------------------- LEAVE GAME ------------------------------------------------
  void GameImpl::leaveGame()
  {
    /* Leaves the current game. Moves directly to the post-game score screen */
    this->setLastError(Errors::None);
    *BW::BWDATA_GameState    = 0;
    *BW::BWDATA_GamePosition = 6;
  }
  //--------------------------------------------- RESTART GAME -----------------------------------------------
  void GameImpl::restartGame()
  {
    /* Restarts the current match 
       Does not work on Battle.net */
    this->setLastError(Errors::None);
    *BW::BWDATA_GameState    = 0;
    *BW::BWDATA_GamePosition = 5;
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int GameImpl::getMouseX()
  {
    /* Retrieves the mouse's X coordinate */
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return BW::BWDATA_Mouse->x;
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int GameImpl::getMouseY()
  {
    /* Retrieves the mouse's Y coordinate */
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return BW::BWDATA_Mouse->y;
  }
  //------------------------------------------- GET MOUSE POSITION -------------------------------------------
  BWAPI::Position GameImpl::getMousePosition()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return BWAPI::Positions::Unknown;
    }
    return BWAPI::Position(BW::BWDATA_Mouse->x, BW::BWDATA_Mouse->y);
  }
  //--------------------------------------------- GET MOUSE STATE --------------------------------------------
  bool GameImpl::getMouseState(MouseButton button)
  {
    return getMouseState((int)button);
  }
  //--------------------------------------------- GET MOUSE STATE --------------------------------------------
  bool GameImpl::getMouseState(int button)
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return false;
    }
    if (button<0 || button>=3) return false;
    SHORT ButtonDown = 0;
    switch (button)
    {
      case BWAPI::M_LEFT:
        ButtonDown = GetKeyState(VK_LBUTTON);
        break;
      case BWAPI::M_MIDDLE:
        ButtonDown = GetKeyState(VK_MBUTTON);
        break;
      case BWAPI::M_RIGHT:
        ButtonDown = GetKeyState(VK_RBUTTON);
        break;
      default:
        break;
    }
    bool pressed = (ButtonDown & 128) > 0;
    return pressed;
    
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(Key key)
  {
    return getKeyState((int)key);
  }
  //---------------------------------------------- GET KEY STATE ---------------------------------------------
  bool GameImpl::getKeyState(int key)
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return false;
    }
    if (key < 0 || key >= 255)
      return false;

    return (GetKeyState(key) & 128) > 0;
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int GameImpl::getScreenX()
  {
    /* Retrieves the screen's X coordinate in relation to the map */
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return *(BW::BWDATA_ScreenX);
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int GameImpl::getScreenY()
  {
    /* Retrieves the screen's Y coordinate in relation to the map */
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return *(BW::BWDATA_ScreenY);
  }
  //------------------------------------------- GET SCREEN POSITION ------------------------------------------
  BWAPI::Position GameImpl::getScreenPosition()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return BWAPI::Positions::Unknown;
    }
    return BWAPI::Position(*(BW::BWDATA_ScreenX),*(BW::BWDATA_ScreenY));
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setScreenPosition(int x, int y)
  {
    /* Sets the screen's position relative to the map */
    if (x < 0 || y < 0)
      return;

    this->setLastError(Errors::None);
    x &= 0xFFFFFFF8;
    y &= 0xFFFFFFF8;
    *BW::BWDATA_MoveToX = x;
    *BW::BWDATA_MoveToTileX = (u16)(x >> 5);
    *BW::BWDATA_MoveToY = y;
    *BW::BWDATA_MoveToTileY = (u16)(y >> 5);
    BW::BWFXN_UpdateScreenPosition();
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setScreenPosition(BWAPI::Position p)
  {
    setScreenPosition(p.x(),p.y());
  }
  //---------------------------------------------- PING MINIMAP ----------------------------------------------
  void GameImpl::pingMinimap(int x, int y)
  {
    this->setLastError(Errors::None);
    IssueCommand((PBYTE)&BW::Orders::MinimapPing(BW::Position((u16)x,(u16)y)), sizeof(BW::Orders::MinimapPing));
  }
  //---------------------------------------------- PING MINIMAP ----------------------------------------------
  void GameImpl::pingMinimap(BWAPI::Position p)
  {
    this->setLastError(Errors::None);
    IssueCommand((PBYTE)&BW::Orders::MinimapPing(BW::Position((u16)p.x(),(u16)p.y())), sizeof(BW::Orders::MinimapPing));
  }
  //----------------------------------------------------------------------------------------------------------
  UnitImpl* GameImpl::getUnit(int index)
  {
    int i = (index & 0x7FF);
    return this->unitArray[i];
  }
  //----------------------------------------------------------------------------------------------------------
  Unit* GameImpl::indexToUnit(int unitIndex)
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(Flag::CompleteMapInformation) == false)
      return NULL;
    int i = (unitIndex & 0x7FF);
    if ( i < 1700 && this->unitArray[i]->canAccess() )
      return this->unitArray[i];
    return NULL;
  }
  //--------------------------------------------- SAVE SELECTED ----------------------------------------------
  void GameImpl::saveSelected()
  {
    memcpy(&savedSelectionStates, BW::BWDATA_CurrentPlayerSelectionGroup, 4 * 12);
    savedSelectionStates[12] = NULL;
    selectedUnitSet.clear();
    for (int i = 0; savedSelectionStates[i] != NULL; i++)
      selectedUnitSet.insert(UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[i]));
  }
  //--------------------------------------------- LOAD SELECTED ----------------------------------------------
  void GameImpl::loadSelected()
  {
    int unitCount = 0;
    while (savedSelectionStates[unitCount] != NULL)
      unitCount++;

    if (unitCount > 0)
      BW::selectUnits(unitCount, savedSelectionStates);
  }
  //------------------------------------------ GET SELECTED UNITS --------------------------------------------
  std::set<BWAPI::Unit*>& GameImpl::getSelectedUnits()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return emptySet;
    }
    return selectedUnitSet;
  }
  //--------------------------------------------- ON REMOVE UNIT ---------------------------------------------
  void GameImpl::onUnitDestroy(BWAPI::UnitImpl* unit)
  {
    /* Called when a unit dies(death animation), not when it is removed */
    if (!unit->alive)
      return;
    this->units.erase(unit);
    deadUnits.push_back(unit);
    int index = unit->getIndex();
    unitArray[index] = new UnitImpl(&BW::BWDATA_UnitNodeTable->unit[index],
                                    &unitArrayCopyLocal->unit[index],
                                    (u16)index);
    if (this->client != NULL)
    {
      bool isInUpdate = this->inUpdate;
      this->inUpdate = false;
      if (unit != NULL && unit->canAccessSpecial())
      {
        unit->makeVisible = true;
        if (unit->lastVisible)
        {
          this->client->onUnitHide(unit);
          events.push_back(Event::UnitHide(unit));
        }

        /* notify the client that the units in the transport died */
        std::list<Unit*> loadedList = unit->getLoadedUnits();

        //units in terran bunker survive
        if (unit->getType() != UnitTypes::Terran_Bunker)
        {
  		    foreach(Unit* loaded, loadedList)
	  		    this->onUnitDestroy((UnitImpl*)loaded);
        }
        this->client->onUnitDestroy(unit);
        events.push_back(Event::UnitDestroy(unit));
        unit->makeVisible = false;
      }
      this->inUpdate = isInUpdate;
    }
    unit->die();
  }
  //----------------------------------------------- GET FIRST ------------------------------------------------
  UnitImpl* GameImpl::getFirst()
  {
    return UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeTable_FirstElement);
  }
  //---------------------------------------------- GET LATENCY -----------------------------------------------
  int GameImpl::getLatency()
  {
    /* Returns the real latency values */

    /* Error checking */
    this->setLastError(Errors::None);
    if (_isSinglePlayer())
      return BWAPI::Latency::SinglePlayer;

    /* Lame options checking */
    switch(*BW::BWDATA_Latency)
    {
      case 0:
        return BWAPI::Latency::LanLow;
      case 1:
        return BWAPI::Latency::LanMedium;
      case 2:
        return BWAPI::Latency::LanHigh;
      default:
        return BWAPI::Latency::LanLow;
    }
  }
  //------------------------------------------ UPDATE UNITS ON TILE ------------------------------------------
  void GameImpl::updateUnits()
  {
    this->inUpdate = false;
    /* Clear all units on tile data */
    for (int y = 0; y < Map::getHeight(); y++)
      for (int x = 0; x < Map::getWidth(); x++)
        this->unitsOnTileData[x][y].clear();

    /* Clear other stuff */
    this->allUnits.clear();
    this->minerals.clear();
    this->geysers.clear();
    this->myPylons.clear();
    std::list<BWAPI::UnitImpl*> morphUnits;
    std::list<BWAPI::UnitImpl*> showUnits;
    std::list<BWAPI::UnitImpl*> hideUnits;
    std::list<BWAPI::UnitImpl*> renegadeUnits;
    std::list< std::pair<BWAPI::UnitImpl*, bool> > dyingUnits;

    foreach (Player* i, playerSet)
      ((PlayerImpl*)i)->units.clear();

    foreach (UnitImpl* i, units)
    {
      if (i->canAccess())
      {
        if ((i->_getType().isInvincible()==false && i->getHitPoints()<=0) || i->getOrder().getID() == BW::OrderID::Die)
        {
          std::pair<UnitImpl*, bool> p(i,i->lastVisible);
          dyingUnits.push_back(p);
          continue;
        }
        int startX = (i->_getPosition().x() - i->_getType().dimensionLeft()) / BW::TILE_SIZE;
        int endX   = (i->_getPosition().x() + i->_getType().dimensionRight() + BW::TILE_SIZE - 1) / BW::TILE_SIZE; // Division - round up
        int startY = (i->_getPosition().y() - i->_getType().dimensionUp()) / BW::TILE_SIZE;
        int endY   = (i->_getPosition().y() + i->_getType().dimensionDown() + BW::TILE_SIZE - 1) / BW::TILE_SIZE;
        for (int x = startX; x < endX; x++)
          for (int y = startY; y < endY; y++)
            this->unitsOnTileData[x][y].insert(i);

        ((PlayerImpl*)(i->_getPlayer()))->units.insert(i);

        this->allUnits.insert(i);

        if (i->_getPlayer()->isNeutral())
        {
          this->neutralUnits.insert(i);
          if (i->_getType() == UnitTypes::Resource_Mineral_Field)
            this->minerals.insert(i);
          else
          {
            if (i->_getType() == UnitTypes::Resource_Vespene_Geyser)
              this->geysers.insert(i);
          }
        }
        else
        {
          if (i->_getPlayer() == (Player*)this->BWAPIPlayer && i->_getType() == UnitTypes::Protoss_Pylon && i->_isCompleted())
            this->myPylons.push_back(i);
        }
        if (i->lastType != i->_getType() && i->lastType != UnitTypes::Unknown && i->_getType() != UnitTypes::Unknown)
          morphUnits.push_back(i);

        if (i->lastPlayer != i->_getPlayer() && i->lastPlayer != NULL && i->_getPlayer() != NULL)
          renegadeUnits.push_back(i);
      }

      i->startingAttack           = i->getAirWeaponCooldown() > i->lastAirWeaponCooldown || i->getGroundWeaponCooldown() > i->lastGroundWeaponCooldown;
      i->lastAirWeaponCooldown    = i->getAirWeaponCooldown();
      i->lastGroundWeaponCooldown = i->getGroundWeaponCooldown();

      i->lastType   = i->_getType();
      i->lastPlayer = i->_getPlayer();
      if (!i->lastVisible && i->isVisible())
      {
        i->lastVisible = true;
        showUnits.push_back(i);
      }
      else
      {
        if (i->lastVisible && !i->isVisible())
        {
          i->lastVisible = false;
          hideUnits.push_back(i);
        }
      }
    }
    if (this->staticNeutralUnits.empty())
    {
      foreach (UnitImpl* i, units)
      {
        if (i->_getPlayer()->isNeutral())
        {
          i->saveInitialInformation();
          this->staticNeutralUnits.insert(i);
          if (i->_getType() == UnitTypes::Resource_Mineral_Field)
            this->staticMinerals.insert(i);
          else
          {
            if (i->_getType() == UnitTypes::Resource_Vespene_Geyser)
              this->staticGeysers.insert(i);
          }
        }
      }
    }

    //here we have all dying units, not just the ones visible to the AI client
    for each(std::pair<BWAPI::UnitImpl*, bool> i in dyingUnits)
    {
      //set lastVisible if the unit was visible to the AI client just before death.
      //This will cause onUnitDestroy to also call onUnitHide before onUnitDestroy.
      if (i.second)
        i.first->lastVisible = true;
      this->onUnitDestroy(i.first); //GameImpl::onUnitDestroy will decide what unit destroy messages to pass along to the AI client
      i.first->lastVisible = false;
    }

    foreach (BWAPI::UnitImpl* i, unitsToBeAdded)
      if (this->client && i->_exists())
        if (i->canAccess())
        {
          this->client->onUnitCreate(i);
          events.push_back(Event::UnitCreate(i));
        }

    /* Pass all renegade units to the AI client */
    foreach (BWAPI::UnitImpl* i, renegadeUnits)
      if (this->client && i->_exists())
      {
        this->client->onUnitRenegade(i);
        events.push_back(Event::UnitRenegade(i));
      }

    /* Pass all morphing units to the AI client */
    foreach (BWAPI::UnitImpl* i, morphUnits)
      if (this->client && i->_exists())
      {
        this->client->onUnitMorph(i);
        events.push_back(Event::UnitMorph(i));
      }

    /* Pass all showing units to the AI client */
    foreach (BWAPI::UnitImpl* i, showUnits)
      if (this->client && i->_exists())
      {
        this->client->onUnitShow(i);
        events.push_back(Event::UnitShow(i));
      }

    /* Pass all hiding units to the AI client */
    foreach (BWAPI::UnitImpl* i, hideUnits)
      if (this->client && i->_exists())
      {
        i->makeVisible = true;
        this->client->onUnitHide(i);
        events.push_back(Event::UnitHide(i));
        i->makeVisible = false;
      }
    this->inUpdate = true;
  }
  //--------------------------------------------- GET FRAME COUNT --------------------------------------------
  int  GameImpl::getFrameCount()
  {
    /* Retrieves the number of frames since game start */
    this->setLastError(Errors::None);
    return this->frameCount;
  }
  //--------------------------------------------- UNITS ON TILE ----------------------------------------------
  std::set<Unit*>& GameImpl::unitsOnTile(int x, int y)
  {
    /* Retrieves a set of units that are on the specified tile */
    this->setLastError(Errors::None);
    if (x < 0 || y < 0 || x >= this->mapWidth() || y >= this->mapHeight())
      return this->emptySet;

    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !isVisible(x,y))
    {
      this->setLastError(Errors::Access_Denied);
      return this->emptySet;
    }
    return unitsOnTileData[x][y];
  }
  //--------------------------------------------- GET LAST ERROR ---------------------------------------------
  Error  GameImpl::getLastError() const
  {
    /* returns the last error encountered in BWAPI */
    return this->lastError;
  }
  //--------------------------------------------- SET LAST ERROR ---------------------------------------------
  void GameImpl::setLastError(BWAPI::Error e)
  {
    /* implies that an error has occured */
    this->lastError = e;
  }
  //--------------------------------------------- IS FLAG ENABLED --------------------------------------------
  bool  GameImpl::isFlagEnabled(int flag)
  {
    /* checks if a BWAPI flag is enabled */
    this->setLastError(Errors::None);
    return this->flags[flag];
  }
  //----------------------------------------------- ENABLE FLAG ----------------------------------------------
  void  GameImpl::enableFlag(int flag)
  {
    /* Enable the specified flag */

    /* Error checking */
    this->setLastError(Errors::None);
    if (this->flagsLocked == true)
    {
      this->sendText("Flags can only be enabled at the start of a game.");
      return;
    }

    if (flag >= BWAPI::FLAG_COUNT)
    {
      this->sendText("Invalid flag (%d).", flag);
      return;
    }

    /* Modify flag state */
    this->flags[flag] = true;
    switch(flag)
    {
    case BWAPI::Flag::CompleteMapInformation:
      this->sendText("Enabled Flag CompleteMapInformation");
      break;
    case BWAPI::Flag::UserInput:
      this->sendText("Enabled Flag UserInput");
      break;
    }
  }
  //-------------------------------------------------- LOCK FLAGS --------------------------------------------
  void GameImpl::lockFlags()
  {
    /* Prevent BWAPI flags from being modified */
    this->flagsLocked = true;
  }
  //----------------------------------------------------- SELF -----------------------------------------------
  Player*  GameImpl::self()
  {
    /* Retrieves the class for the current player */
    this->setLastError(Errors::None);
    return (Player*)this->BWAPIPlayer;
  }
  //----------------------------------------------------- ENEMY ----------------------------------------------
  Player*  GameImpl::enemy()
  {
    /* Retrieves the class for the first opponent player */
    this->setLastError(Errors::None);
    return (Player*)this->opponent;
  }
  //----------------------------------------------------- DRAW -----------------------------------------------
  void GameImpl::addShape(Shape* s)
  {
    /* Adds a shape to the draw queue */
    this->shapes.push_back(s);
  }
  void  GameImpl::drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box */
    if (!inScreen(ctype,left,top,right,bottom)) return;
    addShape(new ShapeBox(ctype, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the map */
    if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom)) return;
    addShape(new ShapeBox(BWAPI::CoordinateType::Map, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the mouse */
    if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom)) return;
    addShape(new ShapeBox(BWAPI::CoordinateType::Mouse, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    /* Draws a box in relation to the screen */
    if (!inScreen(BWAPI::CoordinateType::Screen,left,top,right,bottom)) return;
    addShape(new ShapeBox(BWAPI::CoordinateType::Screen, left, top, right, bottom, color.getID(), isSolid));
  }

  void  GameImpl::drawDot(int ctype, int x, int y, Color color)
  {
    if (!inScreen(ctype,x,y)) return;
    addShape(new ShapeDot(ctype, x, y, color.getID()));
  }
  void  GameImpl::drawDotMap(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x,y)) return;
    addShape(new ShapeDot(BWAPI::CoordinateType::Map, x, y, color.getID()));
  }
  void  GameImpl::drawDotMouse(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x,y)) return;
    addShape(new ShapeDot(BWAPI::CoordinateType::Mouse, x, y, color.getID()));
  }
  void  GameImpl::drawDotScreen(int x, int y, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x,y)) return;
    addShape(new ShapeDot(BWAPI::CoordinateType::Screen, x, y, color.getID()));
  }

  void  GameImpl::drawCircle(int ctype, int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(new ShapeCircle(ctype, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMap(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(new ShapeCircle(BWAPI::CoordinateType::Map, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleMouse(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(new ShapeCircle(BWAPI::CoordinateType::Mouse, x, y, radius, color.getID(), isSolid));
  }
  void  GameImpl::drawCircleScreen(int x, int y, int radius, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-radius,y-radius,x+radius,y+radius)) return;
    addShape(new ShapeCircle(BWAPI::CoordinateType::Screen, x, y, radius, color.getID(), isSolid));
  }

  void  GameImpl::drawEllipse(int ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(ctype,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(new ShapeEllipse(ctype, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMap(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(new ShapeEllipse(BWAPI::CoordinateType::Map, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseMouse(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(new ShapeEllipse(BWAPI::CoordinateType::Mouse, x, y, xrad, yrad, color.getID(), isSolid));
  }
  void  GameImpl::drawEllipseScreen(int x, int y, int xrad, int yrad, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x-xrad,y-yrad,x+xrad,y+yrad)) return;
    addShape(new ShapeEllipse(BWAPI::CoordinateType::Screen, x, y, xrad, yrad, color.getID(), isSolid));
  }

  void  GameImpl::drawLine(int ctype, int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(ctype,x1,y1,x2,y2)) return;
    addShape(new ShapeLine(ctype, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMap(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,x1,y1,x2,y2)) return;
    addShape(new ShapeLine(BWAPI::CoordinateType::Map, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineMouse(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,x1,y1,x2,y2)) return;
    addShape(new ShapeLine(BWAPI::CoordinateType::Mouse, x1, y1, x2, y2, color.getID()));
  }
  void  GameImpl::drawLineScreen(int x1, int y1, int x2, int y2, Color color)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,x1,y1,x2,y2)) return;
    addShape(new ShapeLine(BWAPI::CoordinateType::Screen, x1, y1, x2, y2, color.getID()));
  }

  void  GameImpl::drawTriangle(int ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(ctype,ax,ay,bx,by,cx,cy)) return;
    addShape(new ShapeTriangle(ctype, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMap(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,ax,ay,bx,by,cx,cy)) return;
    addShape(new ShapeTriangle(BWAPI::CoordinateType::Map, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleMouse(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,ax,ay,bx,by,cx,cy)) return;
    addShape(new ShapeTriangle(BWAPI::CoordinateType::Mouse, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }
  void  GameImpl::drawTriangleScreen(int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,ax,ay,bx,by,cx,cy)) return;
    addShape(new ShapeTriangle(BWAPI::CoordinateType::Screen, ax, ay, bx, by, cx, cy, color.getID(), isSolid));
  }

  void  GameImpl::drawText(int ctype, int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addShape(new ShapeText(ctype,x,y,std::string(buffer)));
  }
  void  GameImpl::drawTextMap(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addShape(new ShapeText(BWAPI::CoordinateType::Map,x,y,std::string(buffer)));
  }
  void  GameImpl::drawTextMouse(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addShape(new ShapeText(BWAPI::CoordinateType::Mouse,x,y,std::string(buffer)));
  }
  void  GameImpl::drawTextScreen(int x, int y, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);
    addShape(new ShapeText(BWAPI::CoordinateType::Screen,x,y,std::string(buffer)));
  }

  //--------------------------------------------------- GAME SPEED -------------------------------------------
  void  GameImpl::setLocalSpeed(int speed)
  {
    /* Sets the frame rate of the client */
    if (speed < 0)
    {
      /* Reset the speed if it is negative */
      BW::BWDATA_GameSpeedModifiers[0] = 501;
      BW::BWDATA_GameSpeedModifiers[1] = 333;
      BW::BWDATA_GameSpeedModifiers[2] = 249;
      BW::BWDATA_GameSpeedModifiers[3] = 201;
      BW::BWDATA_GameSpeedModifiers[4] = 168;
      BW::BWDATA_GameSpeedModifiers[5] = 144;
      BW::BWDATA_GameSpeedModifiers[6] = 126;
    }
    else
    {
      /* Set all speeds if it is positive */
      for (int i = 0; i < 7; i++)
        BW::BWDATA_GameSpeedModifiers[i] = speed;
    }
  }

  bool GameImpl::inScreen(int ctype, int x, int y)
  {
    int screen_x1 = x;
    int screen_y1 = y;
    if (ctype == 2)
    {
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
    }
    else if (ctype == 3)
    {
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
    }
    if (screen_x1 < 0   || screen_y1 < 0 ||
        screen_x1 > 640 || screen_y1 > 480) return false;
    return true;
  }

  bool GameImpl::inScreen(int ctype, int x1, int y1, int x2, int y2)
  {
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    if (ctype == 2)
    {
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
      screen_x2 -= *(BW::BWDATA_ScreenX);
      screen_y2 -= *(BW::BWDATA_ScreenY);
    }
    else if (ctype == 3)
    {
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
    }
    if ((screen_x1 < 0 && screen_x2 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0) ||
        (screen_x1 > 640 && screen_x2 > 640) ||
        (screen_y1 > 480 && screen_y2 > 480)) return false;
    return true;
  }

  bool GameImpl::inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3)
  {
    int screen_x1 = x1;
    int screen_y1 = y1;
    int screen_x2 = x2;
    int screen_y2 = y2;
    int screen_x3 = x3;
    int screen_y3 = y3;
    if (ctype == 2)
    {
      screen_x1 -= *(BW::BWDATA_ScreenX);
      screen_y1 -= *(BW::BWDATA_ScreenY);
      screen_x2 -= *(BW::BWDATA_ScreenX);
      screen_y2 -= *(BW::BWDATA_ScreenY);
      screen_x3 -= *(BW::BWDATA_ScreenX);
      screen_y3 -= *(BW::BWDATA_ScreenY);
    }
    else if (ctype == 3)
    {
      screen_x1 += BW::BWDATA_Mouse->x;
      screen_y1 += BW::BWDATA_Mouse->y;
      screen_x2 += BW::BWDATA_Mouse->x;
      screen_y2 += BW::BWDATA_Mouse->y;
      screen_x3 += BW::BWDATA_Mouse->x;
      screen_y3 += BW::BWDATA_Mouse->y;
    }
    if ((screen_x1 < 0 && screen_x2 < 0 && screen_x3 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0 && screen_y3 < 0) ||
        (screen_x1 > 640 && screen_x2 > 640 && screen_x3 > 640) ||
        (screen_y1 > 480 && screen_y2 > 480 && screen_y3 > 480)) return false;
    return true;
  }
  char *GameImpl::dummyMessage(char *message)
  {
    /* This function is only to move a char* to register EAX */
    return message;
  }
  //--------------------------------------------------- MESSAGE BOXES ----------------------------------------
  bool GameImpl::gluMessageBox(char *message, int type)
  {
    switch(type)
    {
    case MB_OKCANCEL:
      dummyMessage(message);  // moves the message to EAX
      return BW::BWFXN_gluPOKCancel_MBox(); // message is a parameter passed in EAX
    default:  // MB_OK
      dummyMessage(message);  // moves the message to EAX
      BW::BWFXN_gluPOK_MBox();   // message is a parameter passed in EAX
    }
    return false;
  }

  bool GameImpl::gluEditBox(char* message, char* dest, size_t destsize, char* restricted)
  {
    return BW::BWFXN_gluPEdit_MBox(message, dest, destsize, restricted);
  }

//--------------------------------------------------- ON SAVE ------------------------------------------------
  void GameImpl::onSaveGame(char *name)
  {
    /* called when the game is being saved */
    if (this->client != NULL)
    {
      this->client->onSaveGame(std::string(name));  // calls the client callback
      events.push_back(Event::SaveGame(std::string(name)));
    }
  }
//--------------------------------------------------- ISCRIPT ------------------------------------------------
  BWAPI::UnitImpl *GameImpl::spriteToUnit(BW::CSprite *sprite)
  {
    /* Retrieves a sprite's parent unit */
    BWAPI::UnitImpl* unit=NULL;
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++) // iterate through every unit
      if (BW::BWDATA_UnitNodeTable->unit[i].sprite == sprite) // compare unit with sprite we're looking for
        unit=unitArray[i];

    return unit;
  }

  void GameImpl::iscriptParser(BW::CSprite *sprite, u8 anim)
  {
    BWAPI::UnitImpl *unit = spriteToUnit(sprite); // get sprite's parent unit
    if (unit != NULL)   // make sure the unit exists
    {
      unit->animState = anim; // associate the animation directly with the unit

      /* New onUnitDestroy hook. Triggered when the unit's animation changes.
      For example: unit plays death animation, we can check its order to see if it's really dying
      and call the old onUnitDestroy. */
      if (unit->getOrder().getID() == BW::OrderID::Die)
        this->onUnitDestroy(unit);
    }
  }

};
