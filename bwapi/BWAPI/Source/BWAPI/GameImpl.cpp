#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "..\\..\\svnrev.h"
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
#include <Util/Dictionary.h>
#include <Util/Sentence.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>

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

#include "BWAPI/AIModule.h"
#include "DLLMain.h"

#include "Globals.h"
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
      , hcachedShapesMutex(::CreateMutex(NULL, FALSE, _T("cachedShapesVector")))
      , inUpdate(false)
  {
    BWAPI::Broodwar = static_cast<Game*>(this);

    /* initialize the unit types */
    BW::UnitType::initialize();

    try
    {
      /* open the config file */
      this->configuration = new Util::Dictionary("bwapi-data\\bwapi.ini");
      config = this->configuration;
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error", "wt");
      fprintf_s(f, "Couldn't load configuration file bwapi.ini because: %s", exception.getMessage().c_str());
      fclose(f);
    }
    try
    {
      /* create log handles */
      this->commandLog        = new Util::FileLogger(config->get("log_path") + "\\commands", Util::LogLevel::MicroDetailed);
      this->newOrderLog       = new Util::FileLogger(config->get("log_path") + "\\new_orders", Util::LogLevel::MicroDetailed);
      this->badAssumptionLog  = new Util::FileLogger(config->get("log_path") + "\\bad_assumptions", Util::LogLevel::MicroDetailed);
      this->newUnitLog        = new Util::FileLogger(config->get("log_path") + "\\new_unit_id", Util::LogLevel::MicroDetailed);
      this->unitSum           = new Util::FileLogger(config->get("log_path") + "\\unit_sum", Util::LogLevel::MicroDetailed);
      this->fatalError        = new Util::FileLogger(config->get("log_path") + "\\FATAL-ERROR", Util::LogLevel::MicroDetailed);

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
        unitTypes.insert(BW::UnitType((BW::UnitID::Enum)i));
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error", "wt");
      fprintf_s(f, "Exception caught inside Game constructor: %s", exception.getMessage().c_str());
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
    delete this->newOrderLog;
    delete this->badAssumptionLog;
    delete this->newUnitLog;
    delete this->unitSum;
    delete this->fatalError;
    delete this->configuration;
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
  //------------------------------------------------ BUILDABLE -----------------------------------------------
  bool  GameImpl::buildable(int x, int y)
  {
    /* Check if the specified coordinates are buildable */
    this->setLastError(Errors::None);
    return this->map.buildable(x, y);
  }
  //------------------------------------------------ WALKABLE ------------------------------------------------
  bool  GameImpl::walkable(int x, int y)
  {
    /* Check if the specified coordinates are walkable */
    this->setLastError(Errors::None);
    return this->map.walkable(x, y);
  }
  //------------------------------------------------- VISIBLE ------------------------------------------------
  bool  GameImpl::visible(int x, int y)
  {
    /* Check if the specified coordinates are visible */
    this->setLastError(Errors::None);
    return this->map.visible(x, y);
  }
  //------------------------------------------------ HAS CREEP -----------------------------------------------
  bool  GameImpl::hasCreep(int x, int y)
  {
    /* Check if the specified coordinates have creep */
    this->setLastError(Errors::None);
    /* Deny this information if you don't have complete map information */
    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !this->visible(x, y))
    {
      this->setLastError(Errors::Access_Denied);
      return false;
    }
    return this->map.hasCreep(x, y);
  }
  //------------------------------------------------ HAS POWER -----------------------------------------------
  bool  GameImpl::hasPower(int x, int y, int tileWidth, int tileHeight)
  {
    /* Check if the specified coordinates have power */
    this->setLastError(Errors::None);
    if (!(tileWidth == 2 && tileHeight == 2) && !(tileWidth == 3 && tileHeight == 2) && !(tileWidth == 4 && tileHeight == 3))
    {
      return false;
    }
    if (tileWidth == 4)
    {
      x++;
    }
    /* Loop through all pylons for the current player */
    for each (UnitImpl* i in myPylons)
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
  //---------------------------------------------- CAN BUILD HERE --------------------------------------------
  bool  GameImpl::canBuildHere(Unit* builder, TilePosition position, UnitType type)
  {
    this->setLastError(Errors::Unbuildable_Location);
    if (position.x()<0) return false;
    if (position.y()<0) return false;
    int width=type.tileWidth();
    int height=type.tileHeight();
    if (position.x()+width>this->mapWidth()) return false;
    if (position.y()+height>=this->mapHeight()) return false;
    if (type.isRefinery())
    {
      for each (Unit* g in getGeysers())
      {
        if (g->getTilePosition() == position)
        {
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
        for each (Unit* i in unitsOnTile(x,y))
          if (!i->getType().isFlyer() && !i->isLifted())
            groundUnits.insert(i);

        if (!this->buildable(x,y) || groundUnits.size() > 1)
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
            if (!BWAPI::Broodwar->hasCreep(x,y))
              return false;
    }
    else
    {
      for(int x = position.x(); x < position.x() + width; x++)
        for(int y = position.y(); y < position.y() + height; y++)
          if (BWAPI::Broodwar->hasCreep(x,y))
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
      for each (BWAPI::Unit* m in getMinerals())
      {
        if (m->getTilePosition().x()>position.x()-5 &&
            m->getTilePosition().y()>position.y()-4 &&
            m->getTilePosition().x()<position.x()+7 &&
            m->getTilePosition().y()<position.y()+6)
        {
          return false;
        }
      }

      for each (BWAPI::Unit* g in getGeysers())
      {
        if (g->getTilePosition().x()>position.x()-7 &&
            g->getTilePosition().y()>position.y()-5 &&
            g->getTilePosition().x()<position.x()+7 &&
            g->getTilePosition().y()<position.y()+6)
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
      if (builder->getType() != *(type.whatBuilds().first))
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }

      /* Carrier space */
      if (builder->getType() == UnitTypes::Protoss_Carrier)
      {
        int max_amt = 4;
        if (self()->upgradeLevel(UpgradeTypes::Carrier_Capacity)>0)
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
        if (self()->upgradeLevel(UpgradeTypes::Reaver_Capacity) > 0)
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
      if (self()->supplyTotal() < self()->supplyUsed() + type.supplyRequired() - type.whatBuilds().first->supplyRequired())
      {
        this->setLastError(Errors::Insufficient_Supply);
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
        this->setLastError(Errors::Insufficient_Tech);
        return false;
      }
    }

    if (*type.requiredTech() != TechTypes::None)
      if (!self()->researched(*(type.requiredTech())))
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
      if (unit->getType() != *(type.whatResearches()))
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }
    }
    if (self()->researched(type))
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
      if (unit->getType() != *(type.whatUpgrades()))
      {
        this->setLastError(Errors::Incompatible_UnitType);
        return false;
      }
    }
    if (self()->upgradeLevel(type)>=type.maxRepeats())
    {
      this->setLastError(Errors::Fully_Upgraded);
      return false;
    }
    if (self()->minerals() < type.mineralPriceBase()+type.mineralPriceFactor()*(self()->upgradeLevel(type)-1))
    {
      this->setLastError(Errors::Insufficient_Minerals);
      return false;
    }
    if (self()->gas() < type.gasPriceBase()+type.gasPriceFactor()*(self()->upgradeLevel(type)-1))
    {
      this->setLastError(Errors::Insufficient_Gas);
      return false;
    }
    return true;
  }
  //---------------------------------------------- GROUND HEIGHT ---------------------------------------------
  int  GameImpl::groundHeight(int x, int y)
  {
    /* Return the ground height */
    this->setLastError(Errors::None);
    return this->map.groundHeight(x, y);
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
  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  void GameImpl::IssueCommand(PBYTE pbBuffer, u32 iSize)
  {
    __asm
    {
      MOV ECX, pbBuffer
      MOV EDX, iSize
    }
    NewIssueCommand();
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
  void GameImpl::update()
  {
    try
    {
      this->inUpdate = true;
      if (!this->isOnStartCalled())
        this->onGameStart();
      
      if (!this->enabled)
        return;

      memcpy(this->unitArrayCopyLocal, BW::BWDATA_UnitNodeTable, sizeof(BW::UnitArray));
      refreshSelectionStates();

      for (int i = 0; i < BW::PLAYER_COUNT; i++)
        this->players[i]->update();

      std::list<UnitImpl*> unitList;
      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
        this->getUnit(i)->buildUnit = NULL;

      for (UnitImpl* i = this->getFirst(); i != NULL; i = i->getNext())
        unitList.push_back(i);

      for each (UnitImpl* i in unitList)
      {
        if (this->units.find(i) == this->units.end())
        {
          i->alive=true;
          this->units.insert(i);
          this->onAddUnit(i);
        }
      }
      for each (UnitImpl* i in units)
      {
        if (i->_getOrderTarget() != NULL && i->_getOrderTarget()->exists() && i->getBWOrder() == BW::OrderID::ConstructingBuilding)
        {
          UnitImpl* j = (UnitImpl*)(i->_getOrderTarget());
          i->buildUnit = j;
          j->buildUnit = i;
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

      this->frameCount++;
      this->logUnknownOrStrange();
      this->updateUnits();

      if (WAIT_OBJECT_0 == ::WaitForSingleObject(hcachedShapesMutex, INFINITE))
      {
        for (unsigned int i = 0; i < this->cachedShapes.size(); i++)
          delete this->cachedShapes[i];

        this->cachedShapes = this->shapes;
        ::ReleaseMutex(hcachedShapesMutex);
      }
      this->shapes.clear();

      this->inUpdate = false;
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error", "wt");
      fprintf_s(f, "Exception caught inside Game::update: %s", exception.getMessage().c_str());
      fclose(f);
    }

    if (this->startedClient == false)
    {
      sendText("BWAPI revision %s is now live.", SVN_REV_STR);

      TCHAR szDllPath[MAX_PATH];
      std::string ai_dll = config->get("ai_dll");
      for (unsigned int i = 0; i < ai_dll.length(); i++)
        szDllPath[i] = TCHAR(ai_dll[i]);

      szDllPath[ai_dll.length()] = TCHAR('\0');
      Util::Logger::globalLog->logCritical("Loading AI DLL from: %s", ai_dll.c_str());
      bool loaded;
      hMod = LoadLibrary(szDllPath);
      if (hMod == NULL)
      {
        loaded = false;
        Util::Logger::globalLog->logCritical("ERROR: Failed to load the AI Module");
        this->client = new AIModule();
        this->enableFlag(Flag::UserInput);
        this->enableFlag(Flag::CompleteMapInformation);
        this->lockFlags();
      }
      else
      {
        loaded = true;
        Util::Logger::globalLog->logCritical("Loaded AI Module");
        Util::Logger::globalLog->logCritical("Importing by Virtual Function Table from AI DLL");

        typedef AIModule* (*PFNCreateA1)(BWAPI::Game*);

        Util::Logger::globalLog->logCritical("Creating an Object of AIModule");

        PFNCreateA1 newAIModule = (PFNCreateA1)GetProcAddress(hMod, TEXT("newAIModule"));
        this->client = newAIModule(this);
        Util::Logger::globalLog->logCritical("Created an Object of AIModule");
      }
      this->client->onStart();
      this->client->onFrame();
      this->lockFlags();
      if (loaded)
        sendText("BWAPI: Loaded the AI Module: %s", ai_dll.c_str());
      else
        sendText("Error: Failed to load the AI Module");
      this->startedClient = true;
    }
    this->client->onFrame();
    this->loadSelected();
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
  //------------------------------------------------ IN GAME -------------------------------------------------
  bool GameImpl::inGame() const
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
  const int BUFFER_SIZE = 2048;
  char buffer[BUFFER_SIZE];

  //---------------------------------------------- PRINT WITH PLAYER ID --------------------------------------
  void GameImpl::printEx(s32 pID, const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    char* txtout = buffer;
    if (inGame() || _isReplay())
      __asm
    {
      pushad
      push 0       // Unknown
      mov eax, pID   // Player ID (-1 for notification area)
      push txtout  // Text
      call dword ptr [BW::BWFXN_PrintText]
      popad
    }
    else
      printf(txtout); // until lobby print private text is found
  }
  //------------------------------------------------- PRINTF -------------------------------------------------
  void  GameImpl::printf(const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    if (_isReplay() || inGame())
    {
      printEx(8, buffer);
      return;
    }

    char* txtout = buffer;
    if (!inGame())
    {
      __asm
      {
        pushad
        mov edi, txtout
        call [BW::BWFXN_SendLobbyCallTarget]
        popad
      }
    }
  }

  void  GameImpl::sendText(const char* text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
    va_end(ap);

    if (_isReplay())
    {
      printEx(8, buffer);
      return;
    }

    if (inGame() && _isSinglePlayer())
    {
      printEx(this->BWAPIPlayer->getID(), buffer);
      return;
    }

    char* txtout = buffer;
    if (inGame())
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
  //---------------------------------------------- CHANGE SLOT -----------------------------------------------
  void GameImpl::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID), 3);
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void  GameImpl::changeRace(BWAPI::Race race)
  {
    this->setLastError(Errors::None);
    IssueCommand((PBYTE)&BW::Orders::ChangeRace(static_cast<BW::Race::Enum>(race.getID()), (u8)this->BWAPIPlayer->getID()), 3);
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
    this->frameCount = 0;
    this->setOnStartCalled(true);
    this->BWAPIPlayer = NULL;
    this->opponent = NULL;

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
        if (strcmp(BW::BWDATA_CurrentPlayer, this->players[i]->getName().c_str()) == 0)
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
    for each (std::string i in force_names)
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
  }

  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  bool GameImpl::onSendText(const char* text)
  {
    /* prep onSendText */
    if (!this->parseText(text) && this->isFlagEnabled(BWAPI::Flag::UserInput))
      if (this->client!=NULL)
        return !this->client->onSendText(std::string(text));
    return true;
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
    else if (parsed[0] == "/save")
    {
      if (parsed[1] == "allies")
      {
        for each (BWAPI::Player* i in getPlayers())
        {
          for each (BWAPI::Player* j in getPlayers())
          {
            BWAPI::Broodwar->printf("%s[%d] alliance data for %s[%d]: %d", i->getName().c_str(), i->getID(), j->getName().c_str(), j->getID(), (int)((PlayerImpl*)i)->getAlliance((u8)j->getID()));
            Util::Logger::globalLog->log("%s[%d] alliance data for %s[%d]: %d", i->getName().c_str(), i->getID(), j->getName().c_str(), j->getID(), (int)((PlayerImpl*)i)->getAlliance((u8)j->getID()));
          }
        }
      }
      else if (parsed[1] == "unit")
      {
        std::ofstream unitData;
        unitData.open ("bwapi-data/unitData.txt");
        for each (BWAPI::UnitType u in BWAPI::UnitTypes::allUnitTypes())
        {
          std::pair<const BWAPI::UnitType*, int> requiredUnits[3];
          BW::UnitType bwu((BW::UnitID::Enum)u.getID());
          requiredUnits[0] = requiredUnits[1] = requiredUnits[2] = std::make_pair((BWAPI::UnitType*)NULL, 0);
          int i = 0;
          for (std::map<const BWAPI::UnitType*, int>::const_iterator r = u.requiredUnits().begin(); r != u.requiredUnits().end(); r++)
            requiredUnits[i++] = *r;

          unitData << "      unitTypeData[" << Util::Strings::stringToVariableName(u.getName()) << ".getID()].set(\""
          << u.getName() << "\",\"";
          if (u != UnitTypes::None && u != UnitTypes::Unknown)
            unitData << std::string(bwu.getSubLabel());

          unitData << "\",Races::" << Util::Strings::stringToVariableName(u.getRace().getName()) << ",&("
          << Util::Strings::stringToVariableName(u.whatBuilds().first->getName()) << "),"
          << u.whatBuilds().second << ",";
          for (int i = 0; i < 3; i++)
          {
            if (requiredUnits[i].first == NULL)
              unitData << "NULL,";
            else
              unitData << "&(" << Util::Strings::stringToVariableName(requiredUnits[i].first->getName()) << "),";

            unitData << requiredUnits[i].second << ",";
          }
          unitData << "&(TechTypes::" << Util::Strings::stringToVariableName(u.requiredTech()->getName()) << "),";
          int count=0;
          for each (TechType t in TechTypes::allTechTypes())
          {
            for each (const UnitType *const u2 in t.whatUses())
            {
              if (u2->getID() == u.getID())
              {
                unitData << "&(TechTypes::" << Util::Strings::stringToVariableName(t.getName()) << "),";
                count++;
              }
            }
          }
          for(int c=count; c < 4; c++)
            unitData << "&(TechTypes::None),";

          if (u != UnitTypes::None && u != UnitTypes::Unknown)
          {
            unitData << "&(UpgradeTypes::";
            if (BWAPI::UpgradeType(bwu.armorUpgrade()) == UpgradeTypes::Unknown)
              unitData << "None),";
            else
              unitData << Util::Strings::stringToVariableName(BWAPI::UpgradeType(bwu.armorUpgrade()).getName()) << "),";

            unitData << (int)bwu.maxHitPoints() << "," << (int)bwu.maxShields() << "," << (int)bwu.maxEnergy() << ","
            << (int)bwu.armor() << "," << (int)bwu.mineralPrice() << "," << (int)bwu.gasPrice() << ","
            << (int)bwu.buildTime() << "," << (int)bwu.supplyRequired() << "," << (int)bwu.supplyProvided() << ","
            << (int)bwu.spaceRequired() << "," << (int)bwu.spaceProvided() << "," << (int)bwu.buildScore() << ","
            << (int)bwu.destroyScore() << ",&(UnitSizeTypes::" << BWAPI::UnitSizeType((int)bwu.size()).getName() << ")," << (int)bwu.tileWidth() << ","
            << (int)bwu.tileHeight() << "," << (int)bwu.dimensionLeft() << "," << (int)bwu.dimensionUp() << ","
            << (int)bwu.dimensionRight() << "," << (int)bwu.dimensionDown() << "," << (int)bwu.seekRange() << ","
            << (int)bwu.sightRange() << ",&(WeaponTypes::"
            << Util::Strings::stringToVariableName(BWAPI::WeaponType((int)bwu.groundWeapon()).getName()) << "),"
            << (int)bwu.maxGroundHits() << ",&(WeaponTypes::"
            << Util::Strings::stringToVariableName(BWAPI::WeaponType((int)bwu.airWeapon()).getName()) << "),"
            << (int)bwu.maxAirHits() << "," << (int)bwu.topSpeed() << "," << (int)bwu.acceleration() << ","
            << (int)bwu.haltDistance() << "," << (int)bwu.turnRadius() << ","
            << bwu.canProduce() << "," << bwu.canAttack() << "," << bwu.canMove() << ","
            << bwu.isFlyer() << "," << bwu.regeneratesHP() << "," << bwu.isSpellcaster() << ","
            << bwu.hasPermanentCloak() << "," << bwu.isInvincible() << "," << bwu.isOrganic() << ","
            << bwu.isMechanical() << "," << bwu.isRobotic() << "," << bwu.isDetector() << ","
            << bwu.isResourceContainer() << "," << bwu.isResourceDepot() << "," << bwu.isWorker() << ","
            << bwu.requiresPsi() << "," << bwu.requiresCreep() << "," << bwu.isTwoUnitsInOneEgg() << ","
            << bwu.isBurrowable() << "," << bwu.isCloakable() << "," << bwu.isBuilding() << ","
            << bwu.isAddon() << "," << bwu.isFlyingBuilding() << "," << bwu.isNeutral() << ","
            << bwu.isRefinery()
            << ");\n";
          }
          else
          {
            unitData << "&(UpgradeTypes::None), 0,0,0, 0,0,0, 0,0,0, 255,0,0, 0,&(UnitSizeTypes::None),0, 0,0,0, 0,0,0, 0,&(WeaponTypes::None), 0,&(WeaponTypes::None),0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);\n";
          }
        }
        unitData.close();
      }
      else if (parsed[1] == "weapon")
      {
        std::ofstream weaponData;
        weaponData.open ("bwapi-data/weaponData.txt");
        for each (BWAPI::WeaponType w in BWAPI::WeaponTypes::allWeaponTypes())
        {
          BW::WeaponType bww((BW::WeaponID::Enum)w.getID());

          weaponData << "      weaponTypeData[" << Util::Strings::stringToVariableName(w.getName()) << ".getID()].set(\""
          << w.getName() << "\",";
          if (w == BWAPI::WeaponTypes::None || w == BWAPI::WeaponTypes::Unknown)
            weaponData << "0,0,0,0,&(UpgradeTypes::None),&(DamageTypes::None),&(ExplosionTypes::None),0,0,0,0,0,0,0,0,0,0,0,0,0,0);";
          else
          {
            weaponData << (int)bww.damageAmount() << "," << (int)bww.damageBonus() << ","
            << (int)bww.damageCooldown() << "," << (int)bww.damageFactor() << ",&(UpgradeTypes::"
            << Util::Strings::stringToVariableName(bww.upgradeType().getName()) << "),&(DamageTypes::"
            << Util::Strings::stringToVariableName(BWAPI::DamageType(bww.damageType()).getName()) << "),&(ExplosionTypes::"
            << Util::Strings::stringToVariableName(BWAPI::ExplosionType(bww.explosionType()).getName())
            << ")," << bww.minRange() << "," << bww.maxRange() << "," << bww.innerSplashRadius() << ","
            << bww.medianSplashRadius() << "," << bww.outerSplashRadius() << "," << bww.targetsAir() << ","
            << bww.targetsGround() << "," << bww.targetsMechanical() << "," << bww.targetsOrganic() << ","
            << bww.targetsNonBuilding() << "," << bww.targetsNonRobotic() << "," << bww.targetsTerrain() << ","
            << bww.targetsOrgOrMech() << "," << bww.targetsOwn()
            << ");\n";
          }
        }
        weaponData.close();
      }
      else if (parsed[1] == "unitsDat")
      {
        struct unitsDat_Unknown
        {
          u8 unitType[BW::UNIT_TYPE_COUNT];
        };
        static unitsDat_Unknown* BWDATA_UnitUnknown = (unitsDat_Unknown*) BW::unitsDat[25].address;
        std::ofstream unitsDat;
        unitsDat.open("bwapi-data/unitsDat.txt");
        for each (UnitType i in UnitTypes::allUnitTypes())
        {
          BW::UnitType bwu((BW::UnitID::Enum)i.getID());
          unitsDat << "(" << i.getID() << ")" << i.getName() << ", seek range: " << (int)bwu.seekRange() << "\n";
        }
        unitsDat.close();
      }
      else if (parsed[1] == "weaponsDat")
      {
        std::ofstream weaponsDat;
        weaponsDat.open("bwapi-data/weaponsDat.txt");
        for each (WeaponType i in WeaponTypes::allWeaponTypes())
        {
          weaponsDat << i.getID() << ": " << std::string(i.getName()) << "\n";
          weaponsDat << "  damage amount:" << i.damageAmount() << "\n";
          weaponsDat << "  damage bonus:" << i.damageBonus() << "\n";
          weaponsDat << "  damage cooldown:" << i.damageCooldown() << "\n";
          weaponsDat << "  damage factor:" << i.damageFactor() << "\n";
          weaponsDat << "  damage type:" << i.damageType()->getName() << "(" << i.damageType()->getID() << ")\n";
          weaponsDat << "  explosion type:" << i.explosionType()->getName() << "(" << i.explosionType()->getID() << ")\n";
          weaponsDat << "  min range:" << i.minRange() << ", max range: " << i.maxRange() << "\n";
          weaponsDat << "  targets ground:" << i.targetsGround() << ", targetsAir: " << i.targetsAir() << "\n";
        }
        weaponsDat.close();
      }
      else if (parsed[1] == "upgradesMax")
      {
        std::ofstream upgradesMax;
        upgradesMax.open("bwapi-data/upgradesMax.txt");
        for each (UpgradeType i in UpgradeTypes::allUpgradeTypes())
        {
          BW::UpgradeType ut=BW::UpgradeType(BW::UpgradeID::Enum(i.getID()));
          upgradesMax << i.getID() << ": " << (int)ut.upgradesMax() << "\n"; 
        }
        upgradesMax.close();
      }
      else
      {
        this->printf("Unknown command \"%s\" - possible commands are: allies, unit, weapon, unitsDat, weaponsDat, upgradesMax", parsed[1].c_str());
      }
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
    return false;
  }
  //---------------------------------------------- ON GAME END -----------------------------------------------
  void GameImpl::onGameEnd()
  {
    this->setOnStartCalled(false);
    if (this->client != NULL)
    {
      this->client->onEnd();
      delete this->client;
      this->client=NULL;
    }
    this->units.clear();
    this->forces.clear();
    this->playerSet.clear();
    this->allUnits.clear();
    this->minerals.clear();
    this->geysers.clear();
    this->neutralUnits.clear();
    this->myPylons.clear();

    this->staticMinerals.clear();
    this->staticGeysers.clear();
    this->staticNeutralUnits.clear();

    this->commandBuffer.clear();
    FreeLibrary(hMod);
    Util::Logger::globalLog->logCritical("Unloaded AI Module");
    for (int i = 0; i < 13; i++)
      this->savedSelectionStates[i] = NULL;

    this->invalidIndices.clear();
    this->selectedUnitSet.clear();
    this->startedClient = false;
    for each (UnitImpl* d in this->deadUnits)
      delete d;

    this->deadUnits.clear();
    if (WAIT_OBJECT_0 == ::WaitForSingleObject(hcachedShapesMutex, INFINITE))
    {
      for (unsigned int i = 0; i < this->cachedShapes.size(); i++)
        delete this->cachedShapes[i];

      this->cachedShapes.clear();
      ::ReleaseMutex(hcachedShapesMutex);
    }

    for (unsigned int i = 0; i < this->shapes.size(); i++)
      delete this->shapes[i];

    this->shapes.clear();

    for(int i = 0 ; i < BW::PLAYER_COUNT; i++)
      if (this->players[i] != NULL)
        this->players[i]->onGameEnd();
    this->setLocalSpeed(-1);


    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      unitArray[i]->userSelected=false;
      unitArray[i]->buildUnit=NULL;
      unitArray[i]->alive=false;
      unitArray[i]->dead=false;
      unitArray[i]->savedPlayer=NULL;
      unitArray[i]->savedUnitType=NULL;
      unitArray[i]->staticInformation=false;
      unitArray[i]->lastVisible=false;
      unitArray[i]->lastType=UnitTypes::Unknown;
    }
  }
  //----------------------------------------------- START GAME -----------------------------------------------
  void  GameImpl::startGame()
  {
    this->setLastError(Errors::None);
    this->IssueCommand((PBYTE)&BW::Orders::StartGame(), sizeof(BW::Orders::StartGame));
  }
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void  GameImpl::pauseGame()
  {
    this->setLastError(Errors::None);
    this->IssueCommand((PBYTE)&BW::Orders::PauseGame(), sizeof(BW::Orders::PauseGame));
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void  GameImpl::resumeGame()
  {
    this->setLastError(Errors::None);
    this->IssueCommand((PBYTE)&BW::Orders::ResumeGame(), sizeof(BW::Orders::ResumeGame));
  }
  //---------------------------------------------- LEAVE GAME ------------------------------------------------
  void  GameImpl::leaveGame()
  {
    this->setLastError(Errors::None);
    *BW::BWDATA_QuitMission_UNKNOWN8 = 0;
    *BW::BWDATA_QuitMission_UNKNOWN9 = 4;

    *BW::BWDATA_NextMenu = 1;
    BW::changeMenu();
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int  GameImpl::getMouseX()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return *(BW::BWDATA_MouseX);
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int  GameImpl::getMouseY()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return *(BW::BWDATA_MouseY);
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int  GameImpl::getScreenX()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return *(BW::BWDATA_ScreenX);
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int  GameImpl::getScreenY()
  {
    this->setLastError(Errors::None);
    if (this->isFlagEnabled(BWAPI::Flag::UserInput) == false)
    {
      this->setLastError(Errors::Access_Denied);
      return 0;
    }
    return *(BW::BWDATA_ScreenY);
  }
  //------------------------------------------- SET SCREEN POSITION ------------------------------------------
  void GameImpl::setScreenPosition(int x, int y)
  {
    this->setLastError(Errors::None);
    *(BW::BWDATA_ScreenX) = x;
    *(BW::BWDATA_ScreenY) = y;
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int GameImpl::_getMouseX() const
  {
    return *(BW::BWDATA_MouseX);
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int GameImpl::_getMouseY() const
  {
    return *(BW::BWDATA_MouseY);
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int GameImpl::_getScreenX() const
  {
    return *(BW::BWDATA_ScreenX);
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int GameImpl::_getScreenY() const
  {
    return *(BW::BWDATA_ScreenY);
  }
  //----------------------------------------------------------------------------------------------------------
  void GameImpl::refresh()
  {
    __asm
      call BW::BWFXN_Refresh
  }
  //----------------------------------------------------------------------------------------------------------
  UnitImpl* GameImpl::getUnit(int index)
  {
    return this->unitArray[index];
  }
  //--------------------------------------------- SAVE SELECTED ----------------------------------------------
  void GameImpl::saveSelected()
  {
    memcpy(&savedSelectionStates, BW::BWDATA_CurrentPlayerSelectionGroup, 4*12);
    savedSelectionStates[12] = NULL;
    int i = 0;
    selectedUnitSet.clear();
    while (savedSelectionStates[i] != NULL)
    {
      selectedUnitSet.insert(UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[i]));
      i++;
    }
  }
  //--------------------------------------------- LOAD SELECTED ----------------------------------------------
  void GameImpl::loadSelected()
  {
    int unitCount = 0;
    while (savedSelectionStates[unitCount] != NULL)
      unitCount ++;
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
  void GameImpl::onRemoveUnit(BWAPI::UnitImpl* unit)
  {
    int index = unit->getIndex();
    if (!unit->alive) return;
    this->units.erase(unit);
    deadUnits.push_back(unit);
    unitArray[index] = new UnitImpl(&BW::BWDATA_UnitNodeTable->unit[index],
                                    &unitArrayCopyLocal->unit[index],
                                    (u16)index);

    if (this->client != NULL)
    {
      bool isInUpdate = this->inUpdate;
      this->inUpdate = false;
      if (unit != NULL && unit->canAccessSpecial())
      {
        unit->makeVisible=true;
        if (unit->lastVisible)
        {
          this->client->onUnitHide(unit);
        }
        this->client->onUnitDestroy(unit);
        unit->makeVisible=false;
      }

      this->inUpdate = isInUpdate;
    }

    unit->die();
  }
  void GameImpl::onRemoveUnit(BW::Unit* unit)
  {
    int index = ((int)unit - (int)BW::BWDATA_UnitNodeTable) / 336;
    if (index < 0 || index >= BW::UNIT_ARRAY_MAX_LENGTH)
    {
      if (this->invalidIndices.find(index) == this->invalidIndices.end())
      {
        this->newUnitLog->log("Error: Found new invalid unit index: %d, broodwar address: 0x%x",index,unit);
        this->invalidIndices.insert(index);
      }
      return;
    }
    BWAPI::UnitImpl* deadUnit=unitArray[index];
    this->onRemoveUnit(deadUnit);
  }
  //---------------------------------------------- ON ADD UNIT -----------------------------------------------
  void GameImpl::onAddUnit(BWAPI::Unit* unit)
  {
    if (this->client != NULL)
    {
      this->inUpdate = false;
      if (unit!=NULL && ((UnitImpl*)unit)->canAccess())
        this->client->onUnitCreate(unit);

      this->inUpdate = true;
    }
  }
  //----------------------------------------- LOG UNKNOWN OR STRANGE -----------------------------------------
  void GameImpl::logUnknownOrStrange()
  {
    for each (UnitImpl* i in this->units)
    {
      if (i == NULL)
      {
        this->newUnitLog->log("Error: NULL pointer");
      }
      else
      {
        if (!i->getBWType().isValid())
          this->newUnitLog->log("%s", i->getName().c_str());
      }
    }
  }
  //--------------------------------------------- LOG UNIT LIST ----------------------------------------------
  void GameImpl::logUnitList()
  {
    this->unitSum->log("----------------------------------------");
    for each (UnitImpl* i in this->units)
      this->unitSum->log("%s", i->getName().c_str());
    this->unitSum->log("----------------------------------------");
  }
  //----------------------------------------------- GET FIRST ------------------------------------------------
  UnitImpl* GameImpl::getFirst()
  {
    return UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeTable_FirstElement);
  }
  //---------------------------------------------- GET LATENCY -----------------------------------------------
  BWAPI::Latency::Enum  GameImpl::getLatency()
  {
    this->setLastError(Errors::None);
    if (_isSinglePlayer())
      return BWAPI::Latency::SinglePlayer;

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
  //--------------------------------------- PRINT UNIT COUNT PER TILE ----------------------------------------
  void GameImpl::printUnitCountPerTile()
  {
    FILE* f = fopen("unit-counts.txt", "wt");
    for (int y = 0; y < Map::getHeight(); y++)
    {
      for (int x = 0; x < Map::getWidth(); x++)
      {
        if (this->map.buildable(x, y))
          fprintf_s(f, "%d", this->unitsOnTile(x, y).size());
        else
          fprintf_s(f, "X");
      }
      fprintf_s(f, "\n");
    }
    fclose(f);
  }
  //------------------------------------------ UPDATE UNITS ON TILE ------------------------------------------
  void GameImpl::updateUnits()
  {
    this->inUpdate = false;
    this->unitsOnTileData.resize(Map::getWidth(), Map::getHeight());
    for (int y = 0; y < Map::getHeight(); y++)
      for (int x = 0; x < Map::getWidth(); x++)
        this->unitsOnTileData[x][y].clear();

    this->allUnits.clear();
    this->minerals.clear();
    this->geysers.clear();
    this->myPylons.clear();

    for(std::set<Player*>::iterator i = this->playerSet.begin();i!=this->playerSet.end();i++)
      ((PlayerImpl*)(*i))->units.clear();

    for (std::set<UnitImpl*>::iterator i = this->units.begin(); i != this->units.end(); i++)
    {
      if ((*i)->canAccess())
      {
        int startX =   ((*i)->_getPosition().x() - (*i)->_getType().dimensionLeft()) / BW::TILE_SIZE;
        int endX   =   ((*i)->_getPosition().x() + (*i)->_getType().dimensionRight() + BW::TILE_SIZE - 1) / BW::TILE_SIZE; // Division - round up
        int startY =   ((*i)->_getPosition().y() - (*i)->_getType().dimensionUp()) / BW::TILE_SIZE;
        int endY =     ((*i)->_getPosition().y() + (*i)->_getType().dimensionDown() + BW::TILE_SIZE - 1) / BW::TILE_SIZE;
        for (int x = startX; x < endX; x++)
          for (int y = startY; y < endY; y++)
            this->unitsOnTileData[x][y].insert(*i);

        ((PlayerImpl*)((*i)->_getPlayer()))->units.insert(*i);

        this->allUnits.insert(*i);

        if ((*i)->_getPlayer()->isNeutral())
        {
          this->neutralUnits.insert(*i);
          if ((*i)->_getType()==UnitTypes::Resource_Mineral_Field)
            this->minerals.insert(*i);
          else
          {
            if ((*i)->_getType()==UnitTypes::Resource_Vespene_Geyser)
              this->geysers.insert(*i);
          }
        }
        else
        {
          if ((*i)->_getPlayer()==(Player*)this->BWAPIPlayer && (*i)->_getType()==UnitTypes::Protoss_Pylon && (*i)->_isCompleted())
            this->myPylons.push_back(*i);
        }
        if ((*i)->lastType!=(*i)->_getType() && (*i)->lastType!=UnitTypes::Unknown && (*i)->_getType()!=UnitTypes::Unknown)
        {
          if (this->client)
            this->client->onUnitMorph(*i);
        }
      }
      (*i)->lastType=(*i)->_getType();

      if (!(*i)->lastVisible && (*i)->isVisible())
      {
        (*i)->lastVisible=true;
        if (this->client)
          this->client->onUnitShow(*i);
      }
      else
      {
        if ((*i)->lastVisible && !(*i)->isVisible())
        {
          (*i)->lastVisible=false;
          if (this->client)
          {
            (*i)->makeVisible=true;
            this->client->onUnitHide(*i);
            (*i)->makeVisible=false;
          }
        }
      }
    }
    if (this->staticNeutralUnits.empty())
    {
      for (std::set<UnitImpl*>::iterator i = this->units.begin(); i != this->units.end(); i++)
      {
        if ((*i)->_getPlayer()->isNeutral())
        {
          (*i)->saveInitialInformation();
          this->staticNeutralUnits.insert(*i);
          if ((*i)->_getType()==UnitTypes::Resource_Mineral_Field)
            this->staticMinerals.insert(*i);
          else
          {
            if ((*i)->_getType()==UnitTypes::Resource_Vespene_Geyser)
              this->staticGeysers.insert(*i);
          }
        }
      }
    }
    this->inUpdate = true;
  }
  //--------------------------------------------- GET FRAME COUNT --------------------------------------------
  int  GameImpl::getFrameCount()
  {
    this->setLastError(Errors::None);
    return this->frameCount;
  }
  //--------------------------------------------- UNITS ON TILE ----------------------------------------------
  std::set<Unit*>& GameImpl::unitsOnTile(int x, int y)
  {
    this->setLastError(Errors::None);
    if (x < 0 || y < 0 || x >= this->mapWidth() || y >= this->mapHeight())
      return this->emptySet;

    if (!this->isFlagEnabled(Flag::CompleteMapInformation) && !visible(x,y))
    {
      this->setLastError(Errors::Access_Denied);
      return this->emptySet;
    }
    return unitsOnTileData[x][y];
  }
  //--------------------------------------------- GET LAST ERROR ---------------------------------------------
  Error  GameImpl::getLastError() const
  {
    return this->lastError;
  }
  //--------------------------------------------- SET LAST ERROR ---------------------------------------------
  void GameImpl::setLastError(BWAPI::Error e)
  {
    this->lastError = e;
  }
  //--------------------------------------------- IS FLAG ENABLED --------------------------------------------
  bool  GameImpl::isFlagEnabled(BWAPI::Flag::Enum flag)
  {
    this->setLastError(Errors::None);
    return this->flags[flag];
  }
  //----------------------------------------------- ENABLE FLAG ----------------------------------------------
  void  GameImpl::enableFlag(BWAPI::Flag::Enum flag)
  {
    this->setLastError(Errors::None);
    if (this->flagsLocked == true)
    {
      this->sendText("Flags can only be enabled at the start of a game.");
      return;
    }
    this->flags[flag] = true;
    if (flag == BWAPI::Flag::CompleteMapInformation)
    {
      this->sendText("Enabled Flag CompleteMapInformation");
    }
    if (flag == BWAPI::Flag::UserInput)
    {
      this->sendText("Enabled Flag UserInput");
    }
  }
  //-------------------------------------------------- LOCK FLAGS --------------------------------------------
  void GameImpl::lockFlags()
  {
    this->flagsLocked = true;
  }
  //----------------------------------------------------- SELF -----------------------------------------------
  Player*  GameImpl::self()
  {
    this->setLastError(Errors::None);
    return (Player*)this->BWAPIPlayer;
  }
  //----------------------------------------------------- ENEMY ----------------------------------------------
  Player*  GameImpl::enemy()
  {
    this->setLastError(Errors::None);
    return (Player*)this->opponent;
  }
  //----------------------------------------------------- DRAW -----------------------------------------------
  void GameImpl::addShape(Shape* s)
  {
    this->shapes.push_back(s);
  }
  void  GameImpl::drawBox(CoordinateType::Enum ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    if (!inScreen(ctype,left,top,right,bottom)) return;
    addShape(new ShapeBox(ctype, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom)) return;
    addShape(new ShapeBox(BWAPI::CoordinateType::Map, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom)) return;
    addShape(new ShapeBox(BWAPI::CoordinateType::Mouse, left, top, right, bottom, color.getID(), isSolid));
  }
  void  GameImpl::drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
  {
    if (!inScreen(BWAPI::CoordinateType::Screen,left,top,right,bottom)) return;
    addShape(new ShapeBox(BWAPI::CoordinateType::Screen, left, top, right, bottom, color.getID(), isSolid));
  }

  void  GameImpl::drawDot(CoordinateType::Enum ctype, int x, int y, Color color)
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

  void  GameImpl::drawCircle(CoordinateType::Enum ctype, int x, int y, int radius, Color color, bool isSolid)
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

  void  GameImpl::drawEllipse(CoordinateType::Enum ctype, int x, int y, int xrad, int yrad, Color color, bool isSolid)
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

  void  GameImpl::drawLine(CoordinateType::Enum ctype, int x1, int y1, int x2, int y2, Color color)
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

  void  GameImpl::drawTriangle(CoordinateType::Enum ctype, int ax, int ay, int bx, int by, int cx, int cy, Color color, bool isSolid)
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

  void  GameImpl::drawText(CoordinateType::Enum ctype, int x, int y, const char* text, ...)
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
      screen_x1 -= BWAPI::BroodwarImpl._getScreenX();
      screen_y1 -= BWAPI::BroodwarImpl._getScreenY();
    }
    else if (ctype == 3)
    {
      screen_x1 += BWAPI::BroodwarImpl._getMouseX();
      screen_y1 += BWAPI::BroodwarImpl._getMouseY();
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
      screen_x1 -= BWAPI::BroodwarImpl._getScreenX();
      screen_y1 -= BWAPI::BroodwarImpl._getScreenY();
      screen_x2 -= BWAPI::BroodwarImpl._getScreenX();
      screen_y2 -= BWAPI::BroodwarImpl._getScreenY();
    }
    else if (ctype == 3)
    {
      screen_x1 += BWAPI::BroodwarImpl._getMouseX();
      screen_y1 += BWAPI::BroodwarImpl._getMouseY();
      screen_x2 += BWAPI::BroodwarImpl._getMouseX();
      screen_y2 += BWAPI::BroodwarImpl._getMouseY();
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
      screen_x1 -= BWAPI::BroodwarImpl._getScreenX();
      screen_y1 -= BWAPI::BroodwarImpl._getScreenY();
      screen_x2 -= BWAPI::BroodwarImpl._getScreenX();
      screen_y2 -= BWAPI::BroodwarImpl._getScreenY();
      screen_x3 -= BWAPI::BroodwarImpl._getScreenX();
      screen_y3 -= BWAPI::BroodwarImpl._getScreenY();
    }
    else if (ctype == 3)
    {
      screen_x1 += BWAPI::BroodwarImpl._getMouseX();
      screen_y1 += BWAPI::BroodwarImpl._getMouseY();
      screen_x2 += BWAPI::BroodwarImpl._getMouseX();
      screen_y2 += BWAPI::BroodwarImpl._getMouseY();
      screen_x3 += BWAPI::BroodwarImpl._getMouseX();
      screen_y3 += BWAPI::BroodwarImpl._getMouseY();
    }
    if ((screen_x1 < 0 && screen_x2 < 0 && screen_x3 < 0) ||
        (screen_y1 < 0 && screen_y2 < 0 && screen_y3 < 0) ||
        (screen_x1 > 640 && screen_x2 > 640 && screen_x3 > 640) ||
        (screen_y1 > 480 && screen_y2 > 480 && screen_y3 > 480)) return false;
    return true;
  }
};
