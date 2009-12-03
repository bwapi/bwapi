#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "Engine.h"

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>

#include <Util/Version.h>
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
#include "BridgeServer.h"

#include <Bridge/PipeMessage.h>
#include <Bridge/SharedStuff.h>
#include <Bridge/Constants.h>

#include "ShapeBox.h"
#include "ShapeCircle.h"
#include "ShapeEllipse.h"
#include "ShapeDot.h"
#include "ShapeLine.h"
#include "ShapeTriangle.h"
#include "ShapeText.h"

#include <BWAPITypes/UnitCommands.h>

namespace BWAPI
{
  namespace Engine
  {
    //----------------------------------- ENGINE STATE --------------------------------
    // broodwar game state
    enum GameState
    {
      Startup,
      InMenu,
      InMatch
    };
    GameState gameState = Startup;

    // match state
    int frameCount;
    //----------------------------------- ---------------------------------------------
    /** @todo Doesn't work */
    void refresh();
    void loadSelected();
    void printEx(s32 pID, const char* text, ...);

    UnitImpl* getFirst();
    std::set<UnitImpl*> units;
    PlayerImpl* BWAPIPlayer;
    PlayerImpl* opponent;

    void addShape(Shape* s);
    std::vector<Shape*> shapes;
    /** Unknown unitID's */
    bool flagsLocked;
    bool inUpdate;
    std::list<std::string > interceptedMessages;

    HMODULE hMod;
    void saveSelected();
    Map map;
    std::set<BWAPI::Unit*> selectedUnitSet;
    std::set<BWAPI::Unit*> emptySet;
    std::set<TilePosition> startLocations;
    std::set< BW::UnitType> unitTypes;

    std::set<BWAPI::Force*> forces;
    std::set<BWAPI::Player*> playerSet;

    std::set<BWAPI::Unit*> allUnits;
    std::set<BWAPI::Unit*> minerals;
    std::set<BWAPI::Unit*> geysers;
    std::set<BWAPI::Unit*> neutralUnits;
    std::list<BWAPI::UnitImpl*> myPylons;
    Util::RectangleArray<std::set<Unit*> > unitsOnTileData;

    std::set<BWAPI::Unit*> staticMinerals;
    std::set<BWAPI::Unit*> staticGeysers;
    std::set<BWAPI::Unit*> staticNeutralUnits;

    /** Count of game-frames passed from game start. */
    bool onStartCalled;
    BW::UnitArray* unitArrayCopyLocal;
    UnitImpl* unitArray[BW::UNIT_ARRAY_MAX_LENGTH];
    std::vector<std::vector<Command *> > commandBuffer;
    /** All commands ordered from BWAPI */
    Util::Logger* commandLog;
    /** Will update the unitsOnTile content, should be called every frame. */
    void updateUnits();
    /**
     * Specifies if some order was given, so the loadSelect function will have
     * to be called.
     */
    bool flags[BWAPI::FLAG_COUNT];
    BW::Unit* savedSelectionStates[13];
    void refreshSelectionStates();
    bool startedClient;
    BWAPI::Error lastError;
    std::list<UnitImpl*> deadUnits;
    u32 cheatFlags;

    
    PlayerImpl* players[12];
    bool enabled;
    HANDLE hcachedShapesMutex;
    std::set<int> invalidIndices;
    std::vector<Shape*> cachedShapes;
    Util::Logger* newUnitLog;

    void executeUnitCommand(UnitCommand& c);


    //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
    void init()
    {
      onStartCalled=false;
      unitsOnTileData.resize(0, 0);
      enabled=true;
      startedClient=false;
      hcachedShapesMutex=::CreateMutex(NULL, FALSE, _T("cachedShapesVector"));
      inUpdate=false;

      /* initialize the unit types */
      BW::UnitType::initialize();

      try
      {

        /* create log handles */
        commandLog = new Util::FileLogger(std::string(logPath) + "\\commands", Util::LogLevel::MicroDetailed);
        newUnitLog = new Util::FileLogger(std::string(logPath) + "\\new_unit_id", Util::LogLevel::MicroDetailed);

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
        FILE*f = fopen("bwapi-error", "a+");
        fprintf(f, "Exception caught inside Game constructor: %s", exception.getMessage().c_str());
        fclose(f);
      }
    }
    //----------------------------------------------- MAP WIDTH ------------------------------------------------
    int mapWidth()
    {
      /* Get the width of the map */
      setLastError(Errors::None);
      return Map::getWidth();
    }
    //----------------------------------------------- MAP HEIGHT -----------------------------------------------
    int mapHeight()
    {
      /* Get the height of the map */
      setLastError(Errors::None);
      return Map::getHeight();
    }
    //---------------------------------------------- MAP FILENAME ----------------------------------------------
    std::string mapFilename()
    {
      /* Get the map file name */
      setLastError(Errors::None);
      return Map::getFileName();
    }
    //------------------------------------------------ MAP NAME ------------------------------------------------
    std::string mapName()
    {
      /* Get the name of the map */
      setLastError(Errors::None);
      return Map::getName();
    }
    //---------------------------------------------- GROUND HEIGHT ---------------------------------------------
    int getGroundHeight(int x, int y)
    {
      /* Return the ground height */
      setLastError(Errors::None);
      return map.groundHeight(x, y);
    }
    //------------------------------------------------ BUILDABLE -----------------------------------------------
    bool isBuildable(int x, int y)
    {
      /* Check if the specified coordinates are buildable */
      setLastError(Errors::None);
      return map.buildable(x, y);
    }
    //------------------------------------------------ BUILDABLE -----------------------------------------------
    bool isBuildable(TilePosition position)
    {
      /* Check if the specified coordinates are buildable */
      setLastError(Errors::None);
      return map.buildable(position.x(), position.y());
    }
    //------------------------------------------------ WALKABLE ------------------------------------------------
    bool isWalkable(int x, int y)
    {
      /* Check if the specified coordinates are walkable */
      setLastError(Errors::None);
      return map.walkable(x, y);
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool isVisible(int x, int y)
    {
      /* Check if the specified coordinates are visible */
      setLastError(Errors::None);
      return map.visible(x, y);
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool isVisible(TilePosition position)
    {
      /* Check if the specified coordinates are visible */
      setLastError(Errors::None);
      return map.visible(position.x(), position.y());
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool isExplored(int x, int y)
    {
      /* Check if the specified coordinates are visible */
      setLastError(Errors::None);
      return map.isExplored(x, y);
    }
    //------------------------------------------------- VISIBLE ------------------------------------------------
    bool isExplored(TilePosition position)
    {
      /* Check if the specified coordinates are visible */
      setLastError(Errors::None);
      return map.isExplored(position.x(), position.y());
    }
    //------------------------------------------------ HAS CREEP -----------------------------------------------
    bool hasCreep(int x, int y)
    {
      /* Check if the specified coordinates have creep */
      setLastError(Errors::None);
      /* Deny this information if you don't have complete map information */
      if (!isFlagEnabled(Flag::CompleteMapInformation) && !isVisible(x, y))
      {
        setLastError(Errors::Access_Denied);
        return false;
      }
      return map.hasCreep(x, y);
    }
    //------------------------------------------------ HAS CREEP -----------------------------------------------
    bool hasCreep(TilePosition position)
    {
      return hasCreep(position.x(), position.y());
    }
    //------------------------------------------------ HAS POWER -----------------------------------------------
    bool hasPower(int x, int y, int tileWidth, int tileHeight)
    {
      /* Check if the specified coordinates have power */
      setLastError(Errors::None);
      if (!(tileWidth == 2 && tileHeight == 2) && !(tileWidth == 3 && tileHeight == 2) && !(tileWidth == 4 && tileHeight == 3))
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
    bool hasPower(TilePosition position, int tileWidth, int tileHeight)
    {
      return hasPower(position.x(),position.y(),tileWidth,tileHeight);
    }
    //---------------------------------------------- CAN BUILD HERE --------------------------------------------
    bool  canBuildHere(Unit* builder, TilePosition position, UnitType type)
    {
      setLastError(Errors::Unbuildable_Location);
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
        foreach (Unit* g, getGeysers())
        {
          if (g->getTilePosition() == position)
          {
            setLastError(Errors::None);
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
          setLastError(Errors::None);
          return true;
        }
        return false;
      }

      if (type.isResourceDepot())
      {
        foreach (BWAPI::Unit* m, getMinerals())
        {
          if (m->getTilePosition().x()>position.x()-5 &&
              m->getTilePosition().y()>position.y()-4 &&
              m->getTilePosition().x()<position.x()+7 &&
              m->getTilePosition().y()<position.y()+6)
          {
            return false;
          }
        }

        foreach (BWAPI::Unit* g, getGeysers())
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
      setLastError(Errors::None);
      return true;
    }
    //------------------------------------------------- CAN MAKE -----------------------------------------------
    bool  canMake(Unit* builder, UnitType type)
    {
      /* Error checking */
      setLastError(Errors::None);
      if (self() == NULL)
      {
        setLastError(Errors::Unit_Not_Owned);
        return false;
      }

      if (builder != NULL)
      {
        /* Check if the owner of the unit is you */
        if (builder->getPlayer() != self())
        {
          setLastError(Errors::Unit_Not_Owned);
          return false;
        }

        /* Check if this unit can actually build the unit type */
        if (builder->getType() != *(type.whatBuilds().first))
        {
          setLastError(Errors::Incompatible_UnitType);
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
            setLastError(Errors::Insufficient_Space);
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
            setLastError(Errors::Insufficient_Space);
            return false;
          }
        }
      }

      /* Check if player has enough minerals */
      if (self()->minerals() < type.mineralPrice())
      {
        setLastError(Errors::Insufficient_Minerals);
        return false;
      }

      /* Check if player has enough gas */
      if (self()->gas() < type.gasPrice())
      {
        setLastError(Errors::Insufficient_Gas);
        return false;
      }
      
      /* Check if player has enough supplies */
      if (type.supplyRequired() > 0)
        if (self()->supplyTotal() < self()->supplyUsed() + type.supplyRequired() - type.whatBuilds().first->supplyRequired())
        {
          setLastError(Errors::Insufficient_Supply);
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
          setLastError(Errors::Insufficient_Tech);
          return false;
        }
      }

      if (*type.requiredTech() != TechTypes::None)
        if (!self()->hasResearched(*(type.requiredTech())))
        {
          setLastError(Errors::Insufficient_Tech);
          return false;
        }

      if (builder != NULL)
        if (addon != UnitTypes::None && addon.whatBuilds().first==type.whatBuilds().first)
          if (builder->getAddon() == NULL || builder->getAddon()->getType() != addon)
          {
            setLastError(Errors::Insufficient_Tech);
            return false;
          }
      return true;
    }
    //----------------------------------------------- CAN RESEARCH ---------------------------------------------
    bool  canResearch(Unit* unit, TechType type)
    {
      /* Error checking */
      setLastError(Errors::None);
      if (self() == NULL)
      {
        setLastError(Errors::Unit_Not_Owned);
        return false;
      }

      if (unit != NULL)
      {
        if (unit->getPlayer()!=self())
        {
          setLastError(Errors::Unit_Not_Owned);
          return false;
        }
        if (unit->getType() != *(type.whatResearches()))
        {
          setLastError(Errors::Incompatible_UnitType);
          return false;
        }
      }
      if (self()->hasResearched(type))
      {
        setLastError(Errors::Already_Researched);
        return false;
      }
      if (self()->minerals() < type.mineralPrice())
      {
        setLastError(Errors::Insufficient_Minerals);
        return false;
      }
      if (self()->gas() < type.gasPrice())
      {
        setLastError(Errors::Insufficient_Gas);
        return false;
      }
      return true;
    }
    //----------------------------------------------- CAN UPGRADE ----------------------------------------------
    bool  canUpgrade(Unit* unit, UpgradeType type)
    {
      setLastError(Errors::None);
      if (self() == NULL)
      {
        setLastError(Errors::Unit_Not_Owned);
        return false;
      }

      if (unit != NULL)
      {
        if (unit->getPlayer()!=self())
        {
          setLastError(Errors::Unit_Not_Owned);
          return false;
        }
        if (unit->getType() != *(type.whatUpgrades()))
        {
          setLastError(Errors::Incompatible_UnitType);
          return false;
        }
      }
      if (self()->getUpgradeLevel(type)>=type.maxRepeats())
      {
        setLastError(Errors::Fully_Upgraded);
        return false;
      }
      if (self()->minerals() < type.mineralPriceBase()+type.mineralPriceFactor()*(self()->getUpgradeLevel(type)-1))
      {
        setLastError(Errors::Insufficient_Minerals);
        return false;
      }
      if (self()->gas() < type.gasPriceBase()+type.gasPriceFactor()*(self()->getUpgradeLevel(type)-1))
      {
        setLastError(Errors::Insufficient_Gas);
        return false;
      }
      return true;
    }
    //--------------------------------------------- GET START LOCATIONS ----------------------------------------
    std::set< TilePosition >& getStartLocations()
    {
      /* Return the set of Start Locations */
      setLastError(Errors::None);
      return startLocations;
    }
    //----------------------------------------------- GET MAP HASH ---------------------------------------------
    int  getMapHash()
    {
      /* Return a hash of the map's terrain */
      setLastError(Errors::None);
      return BWAPI::Map::getMapHash();
    }
    //----------------------------------------------- GET FORCES -----------------------------------------------
    std::set< Force* >& getForces()
    {
      /* Return a set of forces */
      setLastError(Errors::None);
      return forces;
    }
    //----------------------------------------------- GET PLAYERS ----------------------------------------------
    std::set< Player* >& getPlayers()
    {
      setLastError(Errors::None);
      return playerSet;
    }
    //------------------------------------------------- GET UNITS ----------------------------------------------
    std::set< Unit* >& getAllUnits()
    {
      setLastError(Errors::None);
      return allUnits;
    }
    //---------------------------------------------- GET MINERALS ----------------------------------------------
    std::set< Unit* >& getMinerals()
    {
      setLastError(Errors::None);
      return minerals;
    }
    //---------------------------------------------- GET GEYSERS -----------------------------------------------
    std::set< Unit* >& getGeysers()
    {
      setLastError(Errors::None);
      return geysers;
    }
    //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
    std::set< Unit* >& getNeutralUnits()
    {
      setLastError(Errors::None);
      return neutralUnits;
    }
    //---------------------------------------------- GET MINERALS ----------------------------------------------
    std::set< Unit* >& getStaticMinerals()
    {
      setLastError(Errors::None);
      return staticMinerals;
    }
    //---------------------------------------------- GET GEYSERS -----------------------------------------------
    std::set< Unit* >& getStaticGeysers()
    {
      setLastError(Errors::None);
      return staticGeysers;
    }
    //------------------------------------------- GET NEUTRAL UNITS --------------------------------------------
    std::set< Unit* >& getStaticNeutralUnits()
    {
      setLastError(Errors::None);
      return staticNeutralUnits;
    }
    //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
    void IssueCommand(PBYTE pbBuffer, u32 iSize)
    {
  #ifdef __MINGW32__
      __asm__("mov %ecx, _pbBuffer\n"
              "mov %edx, _iSize"
             );
  #else
      __asm
      {
        mov ecx, pbBuffer
        mov edx, iSize
      }
  #endif
      NewIssueCommand();
    }
    //------------------------------------------------- UPDATE -------------------------------------------------
    void update(GameState nextState)
    {
      // prerequisites
      if(nextState == Startup)
      {
        return;
      }

      // state transitions
      GameState lastState = gameState;
      gameState = nextState;

      // init bridge server
      if(lastState == Startup && nextState != Startup)
      {
        if(!BridgeServer::initConnectionServer())
        {
          printf("could not initialize bridge server: %s\n", BridgeServer::getLastError().c_str());
        }
      }

      // init shared memory
      if(gameState == InMatch
        && BridgeServer::isAgentConnected()
        &&!BridgeServer::isSharedMemoryInitialized())
      {

        // create and export static data
        if(!BridgeServer::createSharedMemory())
        {
          printf("failed to publish shared memory: %s\n", BridgeServer::getLastError().c_str());
          BridgeServer::disconnect();
        }
        if(BridgeServer::sharedStaticData)
        {
          Bridge::StaticGameDataStructure &staticData = *BridgeServer::sharedStaticData;
          for (int x=0;x<mapWidth()*4;x++)
            for (int y=0;y<mapHeight()*4;y++)
            {
              staticData.getGroundHeight[x][y] = getGroundHeight(x,y);
              staticData.isWalkable[x][y] = isWalkable(x,y);
            }
          for (int x=0;x<mapWidth();x++)
            for (int y=0;y<mapHeight();y++)
              staticData.isBuildable[x][y] = isBuildable(x,y);

          strncpy(staticData.mapFilename,mapFilename().c_str(),260);
          staticData.mapFilename[259]='\0';

          strncpy(staticData.mapName,mapName().c_str(),32);
          staticData.mapName[31]='\0';
          staticData.lastFreeCommandSlot=0;

          Engine::enableFlag(Flag::UserInput); //temp

        }
        if(!BridgeServer::publishSharedMemory())
        {
          printf("failed to publish shared memory: %s\n", BridgeServer::getLastError().c_str());
          BridgeServer::disconnect();
        }


      }

      // reset frame count
      if(nextState == InMatch && lastState != InMatch)
      {
        frameCount = 0;
      }

      // onFrame
      if(gameState == InMatch && BridgeServer::isAgentConnected())
      {
        if(BridgeServer::sharedStaticData)
        {
          Bridge::StaticGameDataStructure &staticData = *BridgeServer::sharedStaticData;
          staticData.getLatency    = getLatency();
          staticData.frameCount    = frameCount;
          staticData.getMouseX     = getMouseX();
          staticData.getMouseY     = getMouseY();
          staticData.getScreenX    = getScreenX();
          staticData.getScreenY    = getScreenY();
          staticData.mapWidth      = mapWidth();
          staticData.mapHeight     = mapHeight();
          staticData.mapHash       = getMapHash();
          for (int x=0;x<mapWidth();x++)
            for (int y=0;y<mapHeight();y++)
            {
              staticData.isVisible[x][y] = isVisible(x,y);
              staticData.isExplored[x][y] = isExplored(x,y);
              staticData.hasCreep[x][y] = hasCreep(x,y);
            }
          staticData.isMultiplayer = isMultiplayer();
          staticData.isReplay      = isReplay();
          staticData.isPaused      = isPaused();
          staticData.unitCount=0;
          int i=0;
          foreach(Unit* u,Engine::allUnits)
          {
            staticData.unitData[i].getID=(int)u;
            staticData.unitData[i].getType=u->getType().getID();
            staticData.unitData[i].x=u->getPosition().x();
            staticData.unitData[i].y=u->getPosition().y();
            i++;
          }
          staticData.unitCount=i;
        }
        if(!BridgeServer::invokeOnFrame())
        {
          BridgeServer::disconnect();
          printf("disconnected: %s\n", BridgeServer::getLastError().c_str());
        }
        if(BridgeServer::sharedStaticData)
        {
          Bridge::StaticGameDataStructure &staticData = *BridgeServer::sharedStaticData;
          for(int i=0;i<staticData.lastFreeCommandSlot;i++)
          {
            Engine::executeUnitCommand(staticData.commandQueue[i]);
          }
          staticData.lastFreeCommandSlot = 0;
        }
      }

      // handle incoming connections
      if(BridgeServer::isBridgeInitialized()
        &&!BridgeServer::isAgentConnected())
      {
        if(!BridgeServer::acceptIncomingConnections())
        {
          printf("problem accepting connections: %s", BridgeServer::getLastError().c_str());
          BridgeServer::disconnect();
        }
        if(BridgeServer::isAgentConnected())
        {
          printf("connected");
        }
      }

      // count frames
      if(gameState == InMatch)
      {
        frameCount++;
      }
    }
    //---------------------------------------------- ON MENU FRAME ---------------------------------------------
    void onMenuFrame()
    {
      update(InMenu);
    }
    //------------------------------------------------- UPDATE -------------------------------------------------
    void _update()
    {
      try
      {
        inUpdate = true;
        if (!isOnStartCalled())
          onGameStart();

        // make a local copy of the unit array
        memcpy(unitArrayCopyLocal, BW::BWDATA_UnitNodeTable, sizeof(BW::UnitArray));
        refreshSelectionStates();

        for (int i = 0; i < BW::PLAYER_COUNT; i++)
        {
          bool prevLeftGame=players[i]->leftGame();
          players[i]->update();
          if (!prevLeftGame && players[i]->leftGame())
          {
            //client->onPlayerLeft((Player*)players[i]);
          }
        }

        std::list<UnitImpl*> unitList;
        for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
          getUnit(i)->buildUnit = NULL;

        for (UnitImpl* i = getFirst(); i != NULL; i = i->getNext())
          unitList.push_back(i);

  
//        for(BW::AttackType *curritem = *BW::BWDATA_AttackNodeTable_FirstElement ; curritem; curritem = curritem->next)
//        {
//          Broodwar->drawTextMap(curritem->pos_x, curritem->pos_y, "%s frames: %d", AttackType(curritem->type).getName().c_str(), curritem->time_left>>8);
//          Broodwar->drawCircle(BWAPI::CoordinateType::Map, curritem->pos_x, curritem->pos_y, 4, BWAPI::Colors::White, false);
//        }
  

        foreach (UnitImpl* i, unitList)
        {
          if (units.find(i) == units.end())
          {
            i->alive=true;
            units.insert(i);
            onAddUnit(i);
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
          if (i->getType()==UnitTypes::Terran_Nuclear_Missile && i->nukeDetected == false && i->getRawDataLocal()->connectedUnit->unitID==BW::UnitID::Terran_Ghost)
          {
            i->nukeDetected=true;
            BW::Position bwtarget=i->getRawDataLocal()->orderTargetPos;
            Position target(bwtarget.x,bwtarget.y);
            //if (isFlagEnabled(Flag::CompleteMapInformation) || isVisible(target.x()/32,target.y()/32))
              //client->onNukeDetect(target);
            //else
              //client->onNukeDetect(Positions::Unknown);
          }
        }

        while ((int)(commandBuffer.size()) > getLatency())
        {
          for (unsigned int i = 0; i < commandBuffer[0].size(); i++)
            delete commandBuffer[0][i];

          commandBuffer.erase(commandBuffer.begin());
        }

        commandBuffer.push_back(std::vector<Command *>());
        for (unsigned int i = 0; i < commandBuffer.size(); i++)
          for (unsigned int j = 0; j < commandBuffer[i].size(); j++)
            commandBuffer[i][j]->execute();

        updateUnits();

        if (WAIT_OBJECT_0 == ::WaitForSingleObject(hcachedShapesMutex, INFINITE))
        {
          for (unsigned int i = 0; i < cachedShapes.size(); i++)
            delete cachedShapes[i];

          cachedShapes = shapes;
          ::ReleaseMutex(hcachedShapesMutex);
        }
        shapes.clear();

        inUpdate = false;
      }
      catch (GeneralException& exception)
      {
        FILE*f = fopen("bwapi-error", "a+");
        fprintf(f, "Exception caught inside Game::update: %s", exception.getMessage().c_str());
        fclose(f);
      }

      //client->onFrame();
      for(std::list< std::string >::iterator i=interceptedMessages.begin();i!=interceptedMessages.end();i++)
      {
        bool send=!onSendText(i->c_str());
        if (send)
          sendText(i->c_str());
      }
      interceptedMessages.clear();

      update(InMatch);
      loadSelected();
    }
    //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
    void refreshSelectionStates()
    {
      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
        unitArray[i]->setSelected(false);

      saveSelected();
      for (int i = 0; savedSelectionStates[i] != NULL; i++)
        BWAPI::UnitImpl::BWUnitToBWAPIUnit(savedSelectionStates[i])->setSelected(true);
    }
    //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
    bool isMultiplayer()
    {
      setLastError(Errors::None);
      return (*BW::BWDATA_IsMultiplayer != 0);
    }
    //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
    bool _isSinglePlayer()
    {
      return (*BW::BWDATA_IsMultiplayer == 0);
    }
    //------------------------------------------- IS ON START CALLED -------------------------------------------
    bool isOnStartCalled()
    {
      return onStartCalled;
    }
    //------------------------------------------ SET ON START CALLED -------------------------------------------
    void setOnStartCalled(bool onStartCalled)
    {
      Engine::onStartCalled = onStartCalled;
    }
    //------------------------------------------------ IN GAME -------------------------------------------------
    bool inGame()
    {
      return *(BW::BWDATA_InGame) != 0;
    }
    //--------------------------------------------- IS IN LOBBY ----------------------------------------------
    bool isInLobby()
    {
      return *BW::BWDATA_NextMenu==3;
    }
    //----------------------------------------------- IS PAUSED ------------------------------------------------
    bool isPaused()
    {
      setLastError(Errors::None);
      return *BW::BWDATA_IsNotPaused == 0;
    }
    //----------------------------------------------- IN REPLAY ------------------------------------------------
    bool  isReplay()
    {
      setLastError(Errors::None);
      return *(BW::BWDATA_InReplay) != 0;
    }
    //----------------------------------------------- IN REPLAY ------------------------------------------------
    bool  _isReplay()
    {
      return *(BW::BWDATA_InReplay) != 0;
    }
    const int BUFFER_SIZE = 2048;
    char buffer[BUFFER_SIZE];

    //---------------------------------------------- PRINT WITH PLAYER ID --------------------------------------
    void printEx(s32 pID, const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);

      char* txtout = buffer;
      if (inGame() || _isReplay())
      {
  #ifdef __MINGW32__
        __asm__("pushad\n"
                "push 0\n"
                "mov %eax, _pID\n"
                "push _txtout\n"
                "call dword ptr [BW::BWFXN_PrintText]\n"
                "popad"
               );
  #else
        __asm
        {
          pushad
          push 0       // Unknown
          mov eax, pID   // Player ID (-1 for notification area)
          push txtout  // Text
          call dword ptr [BW::BWFXN_PrintText]
          popad
        }
  #endif
      }
      else
        printf(txtout); // until lobby print private text is found
    }
    //------------------------------------------------- PRINTF -------------------------------------------------
    void  printf(const char* text, ...)
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
      if (!inGame() && isInLobby())
  #ifdef __MINGW32__
        __asm__("pushad\n"
                "mov %edi, _txtout\n"
                "call [BW::BWFXN_SendLobbyCallTarget]\n"
                "popad"
               );
  #else
        __asm
        {
          pushad
          mov edi, txtout
          call [BW::BWFXN_SendLobbyCallTarget]
          popad
        }
  #endif
    }

    void  sendText(const char* text, ...)
    {
      va_list ap;
      va_start(ap, text);
      vsnprintf_s(buffer, BUFFER_SIZE, BUFFER_SIZE, text, ap);
      va_end(ap);
      char* txtout = buffer;

      if (_isReplay())
      {
        printEx(8, buffer);
        return;
      }

      if (inGame() && _isSinglePlayer())
      {
        BW::CheatFlags::Enum cheatID=BW::getCheatFlag(text);
        if (cheatID!=BW::CheatFlags::None)
        {
          cheatFlags ^= cheatID;
          IssueCommand((PBYTE)&BW::Orders::UseCheat(cheatFlags), sizeof(BW::Orders::UseCheat));
          if (cheatID==BW::CheatFlags::ShowMeTheMoney ||
              cheatID==BW::CheatFlags::BreateDeep ||
              cheatID==BW::CheatFlags::WhatsMineIsMine)
            cheatFlags ^= cheatID;
        }
        else
        {
          printEx(BWAPIPlayer->getID(), buffer);
        }
        return;
      }

      if (inGame())
      {
        memset(BW::BWDATA_SendTextRequired, 0xFF, 2);
  #ifdef __MINGW32__
        __asm__("pushad\n"
                "mov %esi, _txtout\n"
                "call [BW::BWFXN_SendPublicCallTarget]\n"
                "popad"
               );
      }
      else
        __asm__("pushad\n"
                "mov %edi, _txtout\n"
                "call [BW::BWFXN_SendLobbyCallTarget]\n"
                "popad"
               );
  #else
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
  #endif
    }
    //---------------------------------------------- CHANGE SLOT -----------------------------------------------
    void changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
    {
      IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID), 3);
    }
    //---------------------------------------------- CHANGE RACE -----------------------------------------------
    void  changeRace(BWAPI::Race race)
    {
      setLastError(Errors::None);
      IssueCommand((PBYTE)&BW::Orders::ChangeRace(static_cast<u8>(race.getID()), (u8)BWAPIPlayer->getID()), 3);
    }
    //----------------------------------------- ADD TO COMMAND BUFFER ------------------------------------------
    void addToCommandBuffer(Command* command)
    {
      command->execute();
      commandBuffer[commandBuffer.size() - 1].push_back(command);
      commandLog->log("(%4d) %s", frameCount, command->describe().c_str());
    }
    //--------------------------------------------- ON GAME START ----------------------------------------------
    void onGameStart()
    {
      /* initialize the variables */
      frameCount = 0;
      setOnStartCalled(true);
      BWAPIPlayer = NULL;
      opponent = NULL;

      /* set all the flags to the default of disabled */
      for (int i = 0; i < FLAG_COUNT; i++)
        flags[i] = false;
      flagsLocked = false;

      /* load the map data */
      map.load();

      if (*(BW::BWDATA_InReplay)) /* set replay flags */
      {
        for (int i = 0; i < FLAG_COUNT; i++)
          flags[i] = true;
        flagsLocked = false;
      }
      else
      {
        /* find the current player by name */
        for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
          if (strcmp(BW::BWDATA_CurrentPlayer, players[i]->getName().c_str()) == 0)
            BWAPIPlayer = players[i];

        /* error if player not found */
        if (BWAPIPlayer == NULL)
        {
          commandLog->log("Error: Could not locate BWAPI player.");
          return;
        }

        /* find the opponent player */
        for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
          if ((players[i]->playerType() == BW::PlayerType::Computer ||
               players[i]->playerType() == BW::PlayerType::Human ||
               players[i]->playerType() == BW::PlayerType::ComputerSlot) &&
              opponent == NULL &&
              BWAPIPlayer->isEnemy(players[i]))
            opponent = players[i];

        /* error if opponent not found */
        if (opponent == NULL)
          commandLog->log("Warning: Could not find any opponent");
      }

      /* get the start locations */
      BW::Positions* posptr = BW::BWDATA_startPositions;
      startLocations.clear();
      playerSet.clear();
      forces.clear();
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
        if (players[i] != NULL && players[i]->getName().length() > 0)
        {
          force_names.insert(std::string(players[i]->getForceName()));
          playerSet.insert(players[i]);
        }

      /* create ForceImpl for force names */
      foreach (std::string i, force_names)
      {
        ForceImpl* newforce = new ForceImpl(i);
        forces.insert((Force*)newforce);
        force_name_to_forceimpl.insert(std::make_pair(i, newforce));
      }

      /* create ForceImpl for players */
      for (int i = 0; i < BW::PLAYER_COUNT; i++)
        if (players[i] != NULL && players[i]->getName().length() > 0)
        {
          ForceImpl* force = force_name_to_forceimpl.find(std::string(players[i]->getForceName()))->second;
          force->players.insert(players[i]);
          players[i]->force = force;
        }
      unitsOnTileData.resize(Map::getWidth(), Map::getHeight());
    }

    //---------------------------------------------- ON SEND TEXT ----------------------------------------------
    bool onSendText(const char* text)
    {
      /* prep onSendText */
      if (parseText(text) || !isFlagEnabled(BWAPI::Flag::UserInput))
        return true;
      else
      {
        if(BridgeServer::isAgentConnected())
        {
          //push text onto stack
          if(!BridgeServer::pushSendText())
            BridgeServer::disconnect();
        }
      }
      return false;
    }
    //----------------------------------------------- PARSE TEXT -----------------------------------------------
    bool parseText(const char* text)
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
        leaveGame();
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
    void onGameEnd()
    {
      setOnStartCalled(false);
      /*
      if (client != NULL)
      {
        bool win=true;
        if (_isReplay())
          win=false;
        else
        {
          for (UnitImpl* i = getFirst(); i != NULL; i = i->getNext())
          {
            if (self()->isEnemy(i->_getPlayer()))
              win=false;
          }
        }
        client->onEnd(win);
        delete client;
        client=NULL;
      }
      */
      units.clear();
      forces.clear();
      playerSet.clear();
      allUnits.clear();
      minerals.clear();
      geysers.clear();
      neutralUnits.clear();
      myPylons.clear();

      staticMinerals.clear();
      staticGeysers.clear();
      staticNeutralUnits.clear();

      commandBuffer.clear();
      for (int i = 0; i < 13; i++) // Why is this 13? There can only be 12 units selected.
        savedSelectionStates[i] = NULL;

      invalidIndices.clear();
      selectedUnitSet.clear();
      startedClient = false;
      foreach (UnitImpl* d, deadUnits)
        delete d;

      deadUnits.clear();
      if (WAIT_OBJECT_0 == ::WaitForSingleObject(hcachedShapesMutex, INFINITE))
      {
        for (unsigned int i = 0; i < cachedShapes.size(); i++)
          delete cachedShapes[i];

        cachedShapes.clear();
        ::ReleaseMutex(hcachedShapesMutex);
      }

      for (unsigned int i = 0; i < shapes.size(); i++)
        delete shapes[i];

      shapes.clear();

      for(int i = 0 ; i < BW::PLAYER_COUNT; i++)
        if (players[i] != NULL)
          players[i]->onGameEnd();
      setLocalSpeed(-1);


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
        unitArray[i]->lastPlayer=NULL;
        unitArray[i]->nukeDetected=false;
      }
      cheatFlags=0;
    }
    //----------------------------------------------- START GAME -----------------------------------------------
    void  startGame()
    {
      /* Starts the game as a lobby host */
      setLastError(Errors::None);
      IssueCommand((PBYTE)&BW::Orders::StartGame(), sizeof(BW::Orders::StartGame));
    }
    //----------------------------------------------- PAUSE GAME -----------------------------------------------
    void  pauseGame()
    {
      /* Pauses the game */
      setLastError(Errors::None);
      IssueCommand((PBYTE)&BW::Orders::PauseGame(), sizeof(BW::Orders::PauseGame));
    }
    //---------------------------------------------- RESUME GAME -----------------------------------------------
    void  resumeGame()
    {
      /* Resumes the game */
      setLastError(Errors::None);
      IssueCommand((PBYTE)&BW::Orders::ResumeGame(), sizeof(BW::Orders::ResumeGame));
    }
    //---------------------------------------------- LEAVE GAME ------------------------------------------------
    void  leaveGame()
    {
      /* Leaves the current game. Moves directly to the post-game score screen */
      setLastError(Errors::None);
      *BW::BWDATA_GameState = 0;
      *BW::BWDATA_GamePosition = 6;
    }
    //--------------------------------------------- RESTART GAME -----------------------------------------------
    void  restartGame()
    {
      /* Restarts the current match 
         Does not work on Battle.net */
      setLastError(Errors::None);
      *BW::BWDATA_GameState = 0;
      *BW::BWDATA_GamePosition = 5;
    }
    //---------------------------------------------- GET MOUSE X -----------------------------------------------
    int  getMouseX()
    {
      /* Retrieves the mouse's X coordinate */
      setLastError(Errors::None);
      if (isFlagEnabled(BWAPI::Flag::UserInput) == false)
      {
        setLastError(Errors::Access_Denied);
        return 0;
      }
      return *(BW::BWDATA_MouseX);
    }
    //---------------------------------------------- GET MOUSE Y -----------------------------------------------
    int  getMouseY()
    {
      /* Retrieves the mouse's Y coordinate */
      setLastError(Errors::None);
      if (isFlagEnabled(BWAPI::Flag::UserInput) == false)
      {
        setLastError(Errors::Access_Denied);
        return 0;
      }
      return *(BW::BWDATA_MouseY);
    }
    //---------------------------------------------- GET SCREEN X ----------------------------------------------
    int  getScreenX()
    {
      /* Retrieves the screen's X coordinate in relation to the map */
      setLastError(Errors::None);
      if (isFlagEnabled(BWAPI::Flag::UserInput) == false)
      {
        setLastError(Errors::Access_Denied);
        return 0;
      }
      return *(BW::BWDATA_ScreenX);
    }
    //---------------------------------------------- GET SCREEN Y ----------------------------------------------
    int  getScreenY()
    {
      /* Retrieves the screen's Y coordinate in relation to the map */
      setLastError(Errors::None);
      if (isFlagEnabled(BWAPI::Flag::UserInput) == false)
      {
        setLastError(Errors::Access_Denied);
        return 0;
      }
      return *(BW::BWDATA_ScreenY);
    }
    //------------------------------------------- SET SCREEN POSITION ------------------------------------------
    void setScreenPosition(int x, int y)
    {
      /* Sets the screen's position in relation to the map */
      setLastError(Errors::None);
      *(BW::BWDATA_ScreenX) = x;
      *(BW::BWDATA_ScreenY) = y;
    }
    //----------------------------------------------------------------------------------------------------------
    void refresh()
    {
  #ifdef __MINGW32__
      __asm__("call BW::BWFXN_Refresh");
  #else
      __asm call BW::BWFXN_Refresh
  #endif
    }
    //----------------------------------------------------------------------------------------------------------
    UnitImpl* getUnit(int index)
    {
      return unitArray[index];
    }
    //--------------------------------------------- SAVE SELECTED ----------------------------------------------
    void saveSelected()
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
    void loadSelected()
    {
      int unitCount = 0;
      while (savedSelectionStates[unitCount] != NULL)
        unitCount++;
      BW::selectUnits(unitCount, savedSelectionStates);
    }
    //------------------------------------------ GET SELECTED UNITS --------------------------------------------
    std::set<BWAPI::Unit*>& getSelectedUnits()
    {
      setLastError(Errors::None);
      if (isFlagEnabled(BWAPI::Flag::UserInput) == false)
      {
        setLastError(Errors::Access_Denied);
        return emptySet;
      }
      return selectedUnitSet;
    }
    //--------------------------------------------- ON REMOVE UNIT ---------------------------------------------
    void onUnitDeath(BWAPI::UnitImpl* unit)
    {
      /* Called when a unit dies(death animation), not when it is removed */
      int index = unit->getIndex();
      if (!unit->alive) return;
      units.erase(unit);
      deadUnits.push_back(unit);
      unitArray[index] = new UnitImpl(&BW::BWDATA_UnitNodeTable->unit[index],
                                      &unitArrayCopyLocal->unit[index],
                                      (u16)index);
/*
      if (client != NULL)
      {
        bool isInUpdate = inUpdate;
        inUpdate = false;
        if (unit != NULL && unit->canAccessSpecial())
        {
          unit->makeVisible = true;
          //if (unit->lastVisible)
            //client->onUnitHide(unit);

          /* notify the client that the units in the transport died *//*
          std::list<Unit*> loadedList = unit->getLoadedUnits();
		      foreach(Unit* loaded, loadedList)
			      onUnitDeath((UnitImpl*)loaded);

          //client->onUnitDestroy(unit);

          unit->makeVisible = false;
        }

        inUpdate = isInUpdate;
      }
*/
      unit->die();
    }
    void onUnitDeath(BW::Unit* unit)
    {
      /* index as seen in Starcraft */
      u16 index = (u16)( ((u32)unit - (u32)BW::BWDATA_UnitNodeTable) / 336) & 0x7FF;
      if (index > BW::UNIT_ARRAY_MAX_LENGTH)
      {
        if (invalidIndices.find(index) == invalidIndices.end())
        {
          newUnitLog->log("Error: Found new invalid unit index: %d, broodwar address: 0x%x", index, unit);
          invalidIndices.insert(index);
        }
        return;
      }
      BWAPI::UnitImpl* deadUnit = unitArray[index];
      onUnitDeath(deadUnit);
    }
    //---------------------------------------------- ON ADD UNIT -----------------------------------------------
    void onAddUnit(BWAPI::Unit* unit)
    {
      /*
      if (client != NULL)
      {
        inUpdate = false;
        if (unit != NULL && ((UnitImpl*)unit)->canAccess())
          client->onUnitCreate(unit);

        inUpdate = true;
      }
      */
    }
    //----------------------------------------------- GET FIRST ------------------------------------------------
    UnitImpl* getFirst()
    {
      return UnitImpl::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeTable_FirstElement);
    }
    //---------------------------------------------- GET LATENCY -----------------------------------------------
    int getLatency()
    {
      /* Returns the real latency values */

      /* Error checking */
      setLastError(Errors::None);
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
    void updateUnits()
    {
      inUpdate = false;
      /* Clear all units on tile data */
      for (int y = 0; y < Map::getHeight(); y++)
        for (int x = 0; x < Map::getWidth(); x++)
          unitsOnTileData[x][y].clear();

      /* Clear other stuff */
      allUnits.clear();
      minerals.clear();
      geysers.clear();
      myPylons.clear();
      std::list<BWAPI::UnitImpl*> morphUnits;
      std::list<BWAPI::UnitImpl*> showUnits;
      std::list<BWAPI::UnitImpl*> hideUnits;
      std::list<BWAPI::UnitImpl*> renegadeUnits;

      foreach (Player* i, playerSet)
        ((PlayerImpl*)i)->units.clear();

      foreach (UnitImpl* i, units)
      {
        if (i->canAccess())
        {
          int startX = (i->_getPosition().x() - i->_getType().dimensionLeft()) / BW::TILE_SIZE;
          int endX   = (i->_getPosition().x() + i->_getType().dimensionRight() + BW::TILE_SIZE - 1) / BW::TILE_SIZE; // Division - round up
          int startY = (i->_getPosition().y() - i->_getType().dimensionUp()) / BW::TILE_SIZE;
          int endY   = (i->_getPosition().y() + i->_getType().dimensionDown() + BW::TILE_SIZE - 1) / BW::TILE_SIZE;
          for (int x = startX; x < endX; x++)
            for (int y = startY; y < endY; y++)
              unitsOnTileData[x][y].insert(i);

          ((PlayerImpl*)(i->_getPlayer()))->units.insert(i);

          allUnits.insert(i);

          if (i->_getPlayer()->isNeutral())
          {
            neutralUnits.insert(i);
            if (i->_getType()==UnitTypes::Resource_Mineral_Field)
              minerals.insert(i);
            else
            {
              if (i->_getType()==UnitTypes::Resource_Vespene_Geyser)
                geysers.insert(i);
            }
          }
          else
          {
            if (i->_getPlayer() == (Player*)BWAPIPlayer && i->_getType() == UnitTypes::Protoss_Pylon && i->_isCompleted())
              myPylons.push_back(i);
          }
          if (i->lastType != i->_getType() && i->lastType != UnitTypes::Unknown && i->_getType() != UnitTypes::Unknown)
            morphUnits.push_back(i);

          if (i->lastPlayer != i->_getPlayer() && i->lastPlayer != NULL && i->_getPlayer() != NULL)
            renegadeUnits.push_back(i);
        }
        i->startingAttack           = i->getAirWeaponCooldown() > i->lastAirWeaponCooldown || i->getGroundWeaponCooldown() > i->lastGroundWeaponCooldown;
        i->lastAirWeaponCooldown    = i->getAirWeaponCooldown();
        i->lastGroundWeaponCooldown = i->getGroundWeaponCooldown();
        i->lastType                 = i->_getType();
        i->lastPlayer               = i->_getPlayer();

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
      if (staticNeutralUnits.empty())
      {
        foreach (UnitImpl* i, units)
        {
          if (i->_getPlayer()->isNeutral())
          {
            i->saveInitialInformation();
            staticNeutralUnits.insert(i);
            if (i->_getType() == UnitTypes::Resource_Mineral_Field)
              staticMinerals.insert(i);
            else
            {
              if (i->_getType() == UnitTypes::Resource_Vespene_Geyser)
                staticGeysers.insert(i);
            }
          }
        }
      }

      /* Pass all renegade units to the AI client */
//      foreach (BWAPI::UnitImpl* i, renegadeUnits)
//        if (client)
//          client->onUnitRenegade(i);

      /* Pass all morphing units to the AI client */
//      foreach (BWAPI::UnitImpl* i, morphUnits)
//        if (client)
//          client->onUnitMorph(i);
/*
      foreach (BWAPI::UnitImpl* i, showUnits)
        if (client)
          client->onUnitShow(i);

      foreach (BWAPI::UnitImpl* i, hideUnits)
      {
        if (client)
        {
          i->makeVisible = true;
          client->onUnitHide(i);
          i->makeVisible = false;
        }
      }
*/
      inUpdate = true;
    }
    //--------------------------------------------- UNITS ON TILE ----------------------------------------------
    std::set<Unit*>& unitsOnTile(int x, int y)
    {
      /* Retrieves a set of units that are on the specified tile */
      setLastError(Errors::None);
      if (x < 0 || y < 0 || x >= mapWidth() || y >= mapHeight())
        return emptySet;

      if (!isFlagEnabled(Flag::CompleteMapInformation) && !isVisible(x,y))
      {
        setLastError(Errors::Access_Denied);
        return emptySet;
      }
      return unitsOnTileData[x][y];
    }
    //--------------------------------------------- GET LAST ERROR ---------------------------------------------
    Error  getLastError()
    {
      /* returns the last error encountered in BWAPI */
      return lastError;
    }
    //--------------------------------------------- SET LAST ERROR ---------------------------------------------
    void setLastError(BWAPI::Error e)
    {
      /* implies that an error has occured */
      lastError = e;
    }
    //--------------------------------------------- IS FLAG ENABLED --------------------------------------------
    bool  isFlagEnabled(int flag)
    {
      /* checks if a BWAPI flag is enabled */
      setLastError(Errors::None);
      return flags[flag];
    }
    //----------------------------------------------- ENABLE FLAG ----------------------------------------------
    void  enableFlag(int flag)
    {
      /* Enable the specified flag */

      /* Error checking */
      setLastError(Errors::None);
      if (flagsLocked == true)
      {
        sendText("Flags can only be enabled at the start of a game.");
        return;
      }

      if (flag >= BWAPI::FLAG_COUNT)
      {
        sendText("Invalid flag (%d).", flag);
        return;
      }

      /* Modify flag state */
      flags[flag] = true;
      switch(flag)
      {
      case BWAPI::Flag::CompleteMapInformation:
        sendText("Enabled Flag CompleteMapInformation");
        break;
      case BWAPI::Flag::UserInput:
        sendText("Enabled Flag UserInput");
        break;
      }
    }
    //-------------------------------------------------- LOCK FLAGS --------------------------------------------
    void lockFlags()
    {
      /* Prevent BWAPI flags from being modified */
      flagsLocked = true;
    }
    //----------------------------------------------------- SELF -----------------------------------------------
    Player*  self()
    {
      /* Retrieves the class for the current player */
      setLastError(Errors::None);
      return (Player*)BWAPIPlayer;
    }
    //----------------------------------------------------- ENEMY ----------------------------------------------
    Player*  enemy()
    {
      /* Retrieves the class for the first opponent player */
      setLastError(Errors::None);
      return (Player*)opponent;
    }
    //----------------------------------------------------- DRAW -----------------------------------------------
    void addShape(Shape* s)
    {
      /* Adds a shape to the draw queue */
      shapes.push_back(s);
    }
    void  drawBox(int ctype, int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      /* Draws a box */
      if (!inScreen(ctype,left,top,right,bottom)) return;
      addShape(new ShapeBox(ctype, left, top, right, bottom, color.getID(), isSolid));
    }
    void  drawBoxMap(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      /* Draws a box in relation to the map */
      if (!inScreen(BWAPI::CoordinateType::Map,left,top,right,bottom)) return;
      addShape(new ShapeBox(BWAPI::CoordinateType::Map, left, top, right, bottom, color.getID(), isSolid));
    }
    void  drawBoxMouse(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      /* Draws a box in relation to the mouse */
      if (!inScreen(BWAPI::CoordinateType::Mouse,left,top,right,bottom)) return;
      addShape(new ShapeBox(BWAPI::CoordinateType::Mouse, left, top, right, bottom, color.getID(), isSolid));
    }
    void  drawBoxScreen(int left, int top, int right, int bottom, Color color, bool isSolid)
    {
      /* Draws a box in relation to the screen */
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

    //--------------------------------------------------- GAME SPEED -------------------------------------------
    void  setLocalSpeed(int speed)
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

    bool inScreen(int ctype, int x, int y)
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
        screen_x1 += *(BW::BWDATA_MouseX);
        screen_y1 += *(BW::BWDATA_MouseY);
      }
      if (screen_x1 < 0   || screen_y1 < 0 ||
          screen_x1 > 640 || screen_y1 > 480) return false;
      return true;
    }

    bool inScreen(int ctype, int x1, int y1, int x2, int y2)
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
        screen_x1 += *(BW::BWDATA_MouseX);
        screen_y1 += *(BW::BWDATA_MouseY);
        screen_x2 += *(BW::BWDATA_MouseX);
        screen_y2 += *(BW::BWDATA_MouseY);
      }
      if ((screen_x1 < 0 && screen_x2 < 0) ||
          (screen_y1 < 0 && screen_y2 < 0) ||
          (screen_x1 > 640 && screen_x2 > 640) ||
          (screen_y1 > 480 && screen_y2 > 480)) return false;
      return true;
    }

    bool inScreen(int ctype, int x1, int y1, int x2, int y2, int x3, int y3)
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
        screen_x1 += *(BW::BWDATA_MouseX);
        screen_y1 += *(BW::BWDATA_MouseY);
        screen_x2 += *(BW::BWDATA_MouseX);
        screen_y2 += *(BW::BWDATA_MouseY);
        screen_x3 += *(BW::BWDATA_MouseX);
        screen_y3 += *(BW::BWDATA_MouseY);
      }
      if ((screen_x1 < 0 && screen_x2 < 0 && screen_x3 < 0) ||
          (screen_y1 < 0 && screen_y2 < 0 && screen_y3 < 0) ||
          (screen_x1 > 640 && screen_x2 > 640 && screen_x3 > 640) ||
          (screen_y1 > 480 && screen_y2 > 480 && screen_y3 > 480)) return false;
      return true;
    }

    bool gluMessageBox(char* message, int type)
    {
      bool rval = false;
      switch(type)
      {
      case MB_OKCANCEL:
  #ifdef __MINGW32__
        __asm__("mov %eax, _message\n"
                "call BW::BWFXN_gluPOKCancel_MBox\n"
                "mov rval, %al"
               );
        break;
      default:  // MB_OK
        __asm__("mov %eax, _message\n"
                "call BW::BWFXN_gluPOK_MBox"
               );
  #else
        __asm
        {
          mov eax, message
          call BW::BWFXN_gluPOKCancel_MBox
          mov rval, al
        }
        break;
      default:  // MB_OK
        __asm
        {
          mov eax, message
            call BW::BWFXN_gluPOK_MBox
        }
  #endif
        return false;
      }
      return rval;
    }

    bool gluEditBox(char* message, char* dest, size_t destsize, char* restricted)
    {
      bool rval;
  #ifdef __MINGW32__
      __asm__("push _restricted\n"
              "push _destsize\n"
              "push _dest\n"
              "push _message\n"
              "call BW::BWFXN_gluPEdit_MBox\n"
              "mov  _rval, %al"
             );
  #else
      __asm
      {
        push restricted
        push destsize
        push dest
        push message
        call BW::BWFXN_gluPEdit_MBox
        mov  rval, al
      }
  #endif
      return rval;
    }
    void addInterceptedMessage(const char* message)
    {
      interceptedMessages.push_back(std::string(message));
    }
    bool flagsAreLocked()
    {
      return flagsLocked;
    }
    bool isInUpdate()
    {
      return inUpdate;
    }

    void executeUnitCommand(UnitCommand& c)
    {
      Unit* u=(Unit*)c.unitID;
      Position targetPosition(c.x,c.y);
      TilePosition targetTilePosition(c.x,c.y);
      Unit* targetUnit=(Unit*)c.targetID;

      if (!u) return;

      switch(c.commandID)
      {
      case CommandID::AttackPosition:
        u->attackMove(targetPosition);
        break;
      case CommandID::AttackUnit:
        if (targetUnit==NULL) break;
        u->attackUnit(targetUnit);
        break;
      case CommandID::RightClickPosition:
        u->rightClick(targetPosition);
        break;
      case CommandID::RightClickUnit:
        if (targetUnit==NULL) break;
        u->rightClick(targetUnit);
        break;
      case CommandID::Train:
        u->train(UnitType(c.specialID));
        break;
      case CommandID::Build:
        u->build(targetTilePosition,UnitType(c.specialID));
        break;
      case CommandID::BuildAddon:
        u->buildAddon(UnitType(c.specialID));
        break;
      case CommandID::Research:
        u->research(TechType(c.specialID));
        break;
      case CommandID::Upgrade:
        u->upgrade(UpgradeType(c.specialID));
        break;
      case CommandID::Stop:
        u->stop();
        break;
      case CommandID::HoldPosition:
        u->holdPosition();
        break;
      case CommandID::Patrol:
        u->patrol(targetPosition);
        break;
      case CommandID::Follow:
        if (targetUnit==NULL) break;
        u->follow(targetUnit);
        break;
      case CommandID::SetRallyPosition:
        u->setRallyPosition(targetPosition);
        break;
      case CommandID::SetRallyUnit:
        if (targetUnit==NULL) break;
        u->setRallyUnit(targetUnit);
        break;
      case CommandID::Repair:
        if (targetUnit==NULL) break;
        u->repair(targetUnit);
        break;
      case CommandID::Morph:
        u->morph(UnitType(c.specialID));
        break;
      case CommandID::Burrow:
        u->burrow();
        break;
      case CommandID::Unburrow:
        u->unburrow();
        break;
      case CommandID::Siege:
        u->siege();
        break;
      case CommandID::Unsiege:
        u->unsiege();
        break;
      case CommandID::Cloak:
        u->cloak();
        break;
      case CommandID::Decloak:
        u->decloak();
        break;
      case CommandID::Lift:
        u->lift();
        break;
      case CommandID::Land:
        u->land(targetTilePosition);
        break;
      case CommandID::Load:
        if (targetUnit==NULL) return;
        u->load(targetUnit);
        break;
      case CommandID::Unload:
        if (targetUnit==NULL) return;
        u->unload(targetUnit);
        break;
      case CommandID::UnloadAll:
        u->unloadAll();
        break;
      case CommandID::CancelConstruction:
        u->cancelConstruction();
        break;
      case CommandID::HaltConstruction:
        u->haltConstruction();
        break;
      case CommandID::CancelMorph:
        u->cancelMorph();
        break;
      case CommandID::CancelTrain:
        u->cancelTrain();
        break;
      case CommandID::CancelTrainSlot:
        u->cancelTrain(c.specialID);
        break;
      case CommandID::CancelAddon:
        u->cancelAddon();
        break;
      case CommandID::CancelResearch:
        u->cancelResearch();
        break;
      case CommandID::CancelUpgrade:
        u->cancelUpgrade();
        break;
      case CommandID::UseTech:
        u->useTech(TechType(c.specialID));
        break;
      case CommandID::UseTechPosition:
        u->useTech(TechType(c.specialID),targetPosition);
        break;
      case CommandID::UseTechUnit:
        if (targetUnit==NULL) break;
        u->useTech(TechType(c.specialID),targetUnit);
        break;
      default:
        break;
      }
    }
  }
};
