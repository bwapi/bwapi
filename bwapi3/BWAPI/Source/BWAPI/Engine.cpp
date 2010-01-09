#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "Engine.h"
#include "Unit.h"   // BWAPI Unit
#include "Map.h"
#include "BridgeServer.h"
#include "Shape.h"

#include <DLLMain.h>

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <stdexcept>

#include <Util\Version.h>
#include <Util\FileLogger.h>
#include <Util\Exceptions.h>
#include <Util\Strings.h>
#include <Util\Foreach.h>
#include <Util\Gnu.h>
#include <Util\FlagArray.h>
#include <Util\LookupTable.h>
#include <Util\TypedPacketSwitch.h>

#include <BW/Broodwar.h>
#include <BW/Hook.h>
#include <BW/Unit.h>
#include <BW/UnitArray.h>
#include <BW/Offsets.h>
#include <BW/UnitID.h>
#include <BW/Command.h>
#include <BW/Latency.h>
#include <BW/TileType.h>
#include <BW/TileSet.h>
#include <BW/UnitType.h>
#include <BW/UnitTypeID.h>
#include <BW/GameType.h>
#include <BW/WeaponType.h>
#include <BW/CheatType.h>
#include <BW/RaceID.h>
#include <BW\OrderID.h>
#include <BW\OrderID.h>

#include <BWAPITypes\BuildPosition.h>
#include <BWAPITypes\WalkPosition.h>
#include <BWAPITypes\Position.h>
#include <BWAPITypes\UnitType.h>
#include <BWAPITypes\Latency.h>
#include <BWAPITypes\Flag.h>
#include <BWAPITypes\UnitCommandTypeId.h>
#include <BWAPITypes\UnitCommand.h>
#include <BWAPITypes\UnitState.h>

#include <BWAPITypes\UnitType.h>
#include <BWAPITypes\UnitTypeId.h>
#include <BWAPITypes\TechType.h>
#include <BWAPITypes\TechTypeId.h>
#include <BWAPITypes\UpgradeType.h>
#include <BWAPITypes\UpgradeTypeId.h>
#include <BWAPITypes\WeaponType.h>
#include <BWAPITypes\WeaponTypeId.h>
#include <BWAPITypes\DamageType.h>
#include <BWAPITypes\DamageTypeId.h>
#include <BWAPITypes\ExplosionType.h>
#include <BWAPITypes\ExplosionTypeId.h>
#include <BWAPITypes\Race.h>
#include <BWAPITypes\RaceId.h>
#include <BWAPITypes\UnitSizeType.h>
#include <BWAPITypes\UnitSizeTypeId.h>
#include <BWAPITypes\PlayerType.h>
#include <BWAPITypes\PlayerTypeId.h>
#include <BWAPIDatabase\UnitTypes.h>

#include <Bridge/PipeMessage.h>
#include <Bridge/SharedStuff.h>
#include <Bridge/Constants.h>

#include <BWAPIDatabase\UnitTypes.h>
#include <BWAPIDatabase\TechTypes.h>
#include <BWAPIDatabase\UpgradeTypes.h>
#include <BWAPIDatabase\WeaponTypes.h>
#include <BWAPIDatabase\DamageTypes.h>
#include <BWAPIDatabase\ExplosionTypes.h>
#include <BWAPIDatabase\Races.h>
#include <BWAPIDatabase\UnitSizeTypes.h>
#include <BWAPIDatabase\PlayerTypes.h>

#include <Tracer\Tracer.h>

namespace BWAPI
{
  namespace Engine
  {
//private:
    //----------------------------------- ENGINE STATE --------------------------------
    // broodwar game state
    enum GameState
    {
      Startup,
      InMenu,
      InMatch
    };
    GameState gameState = Startup;
    GameState lastState = Startup;

    // match state TODO: move some things to ::Map
    int frameCount;
    BW::Unit* savedSelectionStates[13];

    // reflects needed states from last frame to detect add and remove events.
    // is index correlated with BW::getUnitArray();
    BWAPI::Unit bwUnitArrayMirror[BW::UNIT_ARRAY_MAX_LENGTH];

    // index correlated with the unit array mirror. Used for performance optimisation
    Util::FlagArray bwUnitArrayMirrorFlags;

    // BWAPI state
    bool flags[Flags::count];

    // error handling
    Util::Logger* newUnitLog;
    Util::Logger* commandLog;   // All commands ordered from BWAPI
    //------------------------------- -------------------------------------------------
    // TODO: enlist all private functions defined in Engine here
    //------------------------------- -------------------------------------------------
    void init();
    bool parseText(const char* text);

    void simulateUnitCommand(const BWAPI::UnitCommand &simulatedCommand, BWAPI::UnitState &stateToAlter);
    void executeUnitCommand(const BWAPI::UnitCommand& command);
    //---------------------------------------------- INITIALIZATION --------------------------------------------
    void init()
    {
      /* initialize the unit types */
      BW::UnitType::initialize();

      try
      {
        // create log handles
        commandLog = new Util::FileLogger(std::string(logPath) + "\\commands", Util::LogLevel::MicroDetailed);
        newUnitLog = new Util::FileLogger(std::string(logPath) + "\\new_unit_id", Util::LogLevel::MicroDetailed);

        // iterate through unit types and create UnitType for each
        /*
        for (int i = 0; i < BW::UNIT_TYPE_COUNT; i++)
          unitTypes.insert(BW::UnitType((u16)i));
        */
      }
      catch (GeneralException& exception)
      {
        FILE*f = fopen("bwapi-error", "a+");
        fprintf(f, "Exception caught inside Game constructor: %s", exception.getMessage().c_str());
        fclose(f);
      }
    }
    //----------------------------------------------- ENABLE FLAG ----------------------------------------------
    void enableFlag(int flag)
    {
      // Enable the specified flag
      if (flag >= Flags::count)
      {
        BW::sendText("Invalid flag (%d).", flag);
        return;
      }

      // Modify flag state
      flags[flag] = true;
      switch(flag)
      {
      case Flags::CompleteMapInformation:
        BW::sendText("Enabled Flag CompleteMapInformation");
        break;
      case Flags::UserInput:
        BW::sendText("Enabled Flag UserInput");
        break;
      }
    }
    //--------------------------------------------- SAVE SELECTED ----------------------------------------------
    void saveSelected()
    {
      memcpy(&savedSelectionStates, BW::BWDATA_CurrentPlayerSelectionGroup, 4*12);
      savedSelectionStates[12] = NULL;
    }
    //--------------------------------------------- LOAD SELECTED ----------------------------------------------
    void loadSelected()
    {
      int unitCount = 0;
      while (savedSelectionStates[unitCount] != NULL)
        unitCount++;
      BW::selectUnits(unitCount, savedSelectionStates);
    }
    //-------------------------- COMMAND ENTRY HANDLERS -----------------------------------------
    int handleCommandSendText(Bridge::CommandEntry::SendText& packet, Util::MemoryFrame dynamicData)
    {
      // check for correctness of received c string
      if(dynamicData.endAs<char>()[-1] != 0)
        throw GeneralException("received CommandEntry::SendText's text data is not null terminated");

      if(packet.printOnly)
        BW::printf(packet.text);
      else
        BW::sendText(packet.text);

      return true;
    }
    int handleCommandStartGame(Bridge::CommandEntry::StartGame& packet)
    {
      BW::issueCommand(BW::Command::StartGame());
      return false;
    }
    int handleCommandPauseGame(Bridge::CommandEntry::PauseGame& packet)
    {
      BW::issueCommand(BW::Command::PauseGame());
      return false;
    }
    int handleCommandResumeGame(Bridge::CommandEntry::ResumeGame& packet)
    {
      BW::issueCommand(BW::Command::ResumeGame());
      return false;
    }
    int handleCommandLeaveGame(Bridge::CommandEntry::LeaveGame& packet)
    {
      *BW::BWDATA_GameState = 0;
      *BW::BWDATA_GamePosition = 6;
      return false;
    }
    int handleCommandRestartGame(Bridge::CommandEntry::RestartGame& packet)
    {
      *BW::BWDATA_GameState = 0;
      *BW::BWDATA_GamePosition = 5;
      return false;
    }
    int handleCommandSetLocalSpeed(Bridge::CommandEntry::SetLocalSpeed& packet)
    {
      BW::setLocalSpeed(packet.speed);
      return true;
    }
    std::vector<Bridge::CommandEntry::UnitOrder*> commandOrderEntries;
    int handleCommandOrderUnit(Bridge::CommandEntry::UnitOrder& packet)
    {
      // for later processing in tighter loops
      commandOrderEntries.push_back(&packet);
      return true;
    }
    //----------------------------------- SHARE MATCH FRAME ----------------------------------------------------
    void shareMatchFrame()
    {
      // fill buffers with recent world state data
      {
        BWAPI::StaticGameData &staticData = *BridgeServer::gameData;
        staticData.getLatency     = BW::getLatency();
        staticData.frameCount     = frameCount;
        staticData.mousePosition  = Position(BW::getMouseX(), BW::getMouseY());
        staticData.screenPosition = Position(BW::getScreenX(), BW::getScreenY());
        for (int x=0;x<Map::getWidth();x++)
        {
          for (int y=0;y<Map::getHeight();y++)
          {
            staticData.isVisible[x][y] = Map::visible(x,y);
            staticData.isExplored[x][y] = Map::isExplored(x,y);
            staticData.hasCreep[x][y] = Map::hasCreep(x,y);
          }
        }
        staticData.isPaused      = BW::isPaused();
        staticData.unitCount=0;

        // traverse all game unit chains
        for(BW::Unit *bwUnit = *BW::BWDATA_UnitNodeChain_VisibleUnit_First; bwUnit; bwUnit = bwUnit->nextUnit)
        {
          int linear = BW::BWDATA_UnitNodeTable->getIndexByUnit(bwUnit); // get linear index
          Unit &mirror = bwUnitArrayMirror[linear];

          mirror.isInChain = true;
          bwUnitArrayMirrorFlags.setFlag(linear, true); // mark slot for processing

          // not interesting if this unit existed before
          if(mirror.wasInChain)
            continue;

          // unit did not exist before. init the reflection for this unit
          mirror.isDying = false;
          mirror.knownUnit = NULL;
        }

        // flag based search
        for(int w = -1;;)
        {
          // find all non-zero words
          w = bwUnitArrayMirrorFlags.findNextWord(w+1);
          if(w == -1)
            break;  // reached end

          // check each unit reflection that corresponds to a set flag
          int word = bwUnitArrayMirrorFlags.getWord(w);
          int baseIndex = w * Util::FlagArray::flagsInWord;
          for(int f = 0; f < Util::FlagArray::flagsInWord; f++)
          {
            if(!(word & (1 << f)))
              continue;
            // flag is set => this unit slot is used.

            // get corresponding BW::Unit and mirror
            int linear = baseIndex + f;
            BW::Unit &bwUnit = BW::BWDATA_UnitNodeTable->unit[linear];
            Unit &mirror = bwUnitArrayMirror[linear];

            // process the chain states
            bool wasInChain = mirror.wasInChain;  // we need these values for this iteration
            bool isInChain = mirror.isInChain;
            mirror.wasInChain = isInChain;        // init the values for next iteration
            mirror.isInChain = false;

            // remove flag when unit gets removed
            if(!isInChain)
            {
              // slot not used anymore
              bwUnitArrayMirrorFlags.setFlag(linear, false);
            }

            // if flag was set but slot is not used
            if(!isInChain && !wasInChain)
            {
              // optimization not working, debug me!
              __debugbreak();
              continue;
            }

            if(!isInChain && wasInChain)
            {
              // unit perished
              if(mirror.knownUnit)
              {
                BridgeServer::removeKnownUnit(mirror.knownUnitIndex, UnitRemoveEventTypeIds::Died);
                mirror.knownUnit = NULL;
              }
              continue;
            }

            // TODO: find out if the unit is dying. not urgent tho
            bool isDying = false;
            if(!isDying && mirror.isDying)
            {
              // the unit is not dying anymore? It's a new one
              // remove previous.
              if(mirror.knownUnit)
              {
                BridgeServer::removeKnownUnit(mirror.knownUnitIndex, UnitRemoveEventTypeIds::Died);
                mirror.knownUnit = NULL;
              }
            }

            // check the new knownability of this unit
            // TODO: extend knowability criteria
            bool isKnown = Map::visible(bwUnit.position.x/32, bwUnit.position.y/32);

            // if knownability changed
            if(!!mirror.knownUnit != isKnown)
            {
              if(isKnown)
              {
                // unit becomes known

                // reserve a KnownUnitEntry and store it's address so it gets filled
                mirror.knownUnitIndex = BridgeServer::addKnownUnit(&mirror.knownUnit, UnitAddEventTypeIds::Created);
              }
              else
              {
                // unit becomes not known

                // release KnownUnit address
                BridgeServer::removeKnownUnit(mirror.knownUnitIndex, UnitRemoveEventTypeIds::Died);
                mirror.knownUnit = NULL;
              }
            }

            if(isKnown)
            {
              // transfer recent data about this particular BW unit
              KnownUnit &knownUnit = *mirror.knownUnit;

              // TODO: implement clearance limit
              ClearanceLevel clearance;
              UnitTypeId type = (UnitTypeId)bwUnit.unitID.id;
              UnitType& typeData = UnitTypes::unitTypeData[type];

              knownUnit.position                = Position(bwUnit.position);
              knownUnit.type                    = type;

//              knownUnit.id                      = (int)&knownUnit;
              knownUnit.player                  = bwUnit.playerID;
              knownUnit.hitPoints               = bwUnit.healthPoints/256;
              knownUnit.shields                 = bwUnit.shieldPoints/256;
              knownUnit.energy                  = bwUnit.energy/256;
              if  (type == BW::UnitTypeIDs::Resource_MineralPatch1
                || type == BW::UnitTypeIDs::Resource_MineralPatch2
                || type == BW::UnitTypeIDs::Resource_MineralPatch3
                || type == BW::UnitTypeIDs::Resource_VespeneGeyser
                || type == BW::UnitTypeIDs::Terran_Refinery
                || type == BW::UnitTypeIDs::Protoss_Assimilator
                || type == BW::UnitTypeIDs::Zerg_Extractor)
                knownUnit.resources             = bwUnit.unitUnion1.unitUnion1Sub.resourceUnitUnionSub.resourceContained;
              knownUnit.killCount               = bwUnit.killCount;
              knownUnit.groundWeaponCooldown    = bwUnit.groundWeaponCooldown;
              knownUnit.airWeaponCooldown       = bwUnit.airWeaponCooldown;
              knownUnit.spellCooldown           = bwUnit.spellCooldown;
              knownUnit.defenseMatrixPoints     = bwUnit.defenseMatrixDamage/256;

              knownUnit.defenseMatrixTimer      = bwUnit.defenseMatrixTimer;
              knownUnit.ensnareTimer            = bwUnit.ensnareTimer;
              knownUnit.irradiateTimer          = bwUnit.irradiateTimer;
              knownUnit.lockdownTimer           = bwUnit.lockdownTimer;
              knownUnit.maelstromTimer          = bwUnit.maelstromTimer;
              knownUnit.plagueTimer             = bwUnit.plagueTimer;
              knownUnit.removeTimer             = bwUnit.removeTimer;
              knownUnit.stasisTimer             = bwUnit.stasisTimer;
              knownUnit.stimTimer               = bwUnit.stimTimer;

              bool isCompleted                  = bwUnit.status.getBit<BW::StatusFlags::Completed>();
              knownUnit.isCompleted             = isCompleted;
              knownUnit.isAccelerating          = bwUnit.movementFlags.getBit<BW::MovementFlags::Accelerating>();
              if(!isCompleted)
              {
                if(typeData.isBuilding)
                  knownUnit.isBeingConstructed  = bwUnit.currentBuildUnit != NULL || typeData.race != RaceIds::Terran;
                else
                if(type == UnitTypeIds::Zerg_Egg
                || type == UnitTypeIds::Zerg_Lurker_Egg
                || type == UnitTypeIds::Zerg_Cocoon)
                  knownUnit.isBeingConstructed  = true;
              }
              if(typeData.isResourceContainer)
                knownUnit.isBeingGathered       = bwUnit.unitUnion1.unitUnion1Sub.resourceUnitUnionSub.isBeingGathered != 0;
              else
                knownUnit.isBeingGathered       = false;
              knownUnit.isBeingHealed           = bwUnit.isBeingHealed != 0;
              knownUnit.isBlind                 = bwUnit.isBlind != 0;
              knownUnit.isBraking               = bwUnit.movementFlags.getBit<BW::MovementFlags::Braking>();
              knownUnit.isBurrowed              = bwUnit.status.getBit<BW::StatusFlags::Burrowed>();
              if(!typeData.isWorker)
              {
                knownUnit.isCarryingGas         = bwUnit.resourceType == 1;
                knownUnit.isCarryingMinerals    = bwUnit.resourceType == 2;
                knownUnit.isGatheringGas        = false;  // TODO: copy these, warning they;re big ones
                knownUnit.isGatheringMinerals   = false;
              }
              else
              {
                knownUnit.isCarryingGas         = false;
                knownUnit.isCarryingMinerals    = false;
                knownUnit.isGatheringGas        = false;
                knownUnit.isGatheringMinerals   = false;
              }
              knownUnit.isCloaked               = bwUnit.status.getBit<BW::StatusFlags::Cloaked>();

              BW::OrderID orderId = bwUnit.orderID;
              knownUnit.isConstructing          =  orderId == BW::OrderIDs::ConstructingBuilding
                                                || orderId == BW::OrderIDs::BuildTerran
                                                || orderId == BW::OrderIDs::DroneBuild
                                                || orderId == BW::OrderIDs::DroneStartBuild
                                                || orderId == BW::OrderIDs::DroneLand
                                                || orderId == BW::OrderIDs::BuildProtoss1
                                                || orderId == BW::OrderIDs::BuildProtoss2;

              knownUnit.isDefenseMatrixed       = bwUnit.defenseMatrixTimer > 0;
              knownUnit.isEnsnared              = bwUnit.ensnareTimer > 0;
              knownUnit.isFollowing             = orderId == BW::OrderIDs::Follow;

              knownUnit.isHallucination         = bwUnit.status.getBit<BW::StatusFlags::IsHallucination>();
              bool isResearching                = orderId == BW::OrderIDs::ResearchTech;
              knownUnit.isResearching           = isResearching;
              bool hasEmptyBuildQueue           =  bwUnit.buildQueueSlot < 5
                                                && bwUnit.buildQueue[bwUnit.buildQueueSlot] == BW::UnitTypeIDs::None;
              bool isTraining                   = (  type == UnitTypeIds::Terran_Nuclear_Silo
                                                  && bwUnit.secondaryOrderID == BW::OrderIDs::Train)
                                                || ( typeData.canProduce
                                                  && !hasEmptyBuildQueue);
              knownUnit.isTraining              = isTraining;
              bool isUpgrading                  = orderId == BW::OrderIDs::Upgrade;
              knownUnit.isUpgrading             = isUpgrading;
              if (isTraining || isResearching || isUpgrading)
                knownUnit.isIdle                = false;
              else
                knownUnit.isIdle                =  orderId == BW::OrderIDs::PlayerGuard
                                                || orderId == BW::OrderIDs::Guard
                                                || orderId == BW::OrderIDs::Stop
                                                || orderId == BW::OrderIDs::Pickup1
                                                || orderId == BW::OrderIDs::Nothing2
                                                || orderId == BW::OrderIDs::Medic
                                                || orderId == BW::OrderIDs::Carrier
                                                || orderId == BW::OrderIDs::Critter
                                                || orderId == BW::OrderIDs::NukeTrain
                                                || orderId == BW::OrderIDs::Larva;
              knownUnit.isIrradiated            = bwUnit.irradiateTimer > 0;
              knownUnit.isLifted                = bwUnit.status.getBit<BW::StatusFlags::InAir>() && typeData.isBuilding;
              knownUnit.isLoaded                =  bwUnit.status.getBit<BW::StatusFlags::InTransport>()
                                                || bwUnit.status.getBit<BW::StatusFlags::InBuilding>()
                                                ||
                                                ( //if
                                                    (type == UnitTypeIds::Protoss_Interceptor
                                                  || type == UnitTypeIds::Protoss_Scarab)
                                                  &&
                                                  (
                                                    bwUnit.childUnitUnion3.inHanger != 0
                                                  )
                                                );
              knownUnit.isLockedDown            = bwUnit.lockdownTimer > 0;
              knownUnit.isMaelstrommed          = bwUnit.maelstromTimer > 0;
              knownUnit.isMorphing              =  orderId == BW::OrderIDs::Morph1
                                                || orderId == BW::OrderIDs::Morph2
                                                || orderId == BW::OrderIDs::ZergBuildSelf;
              knownUnit.isMoving                = bwUnit.movementFlags.getBit<BW::MovementFlags::Moving>();
              knownUnit.isParasited             = bwUnit.parasiteFlags.value != 0;
              knownUnit.isPatrolling            = orderId == BW::OrderIDs::Patrol;
              knownUnit.isPlagued               = bwUnit.plagueTimer > 0;
              knownUnit.isRepairing             =  orderId == BW::OrderIDs::Repair1
                                                || orderId == BW::OrderIDs::Repair2;
             if(flags[Flags::CompleteMapInformation])
                knownUnit.isSelected            = false;  // TODO: implement this into the bw unit mirror
              knownUnit.isSieged                = type == UnitTypeIds::Terran_Siege_Tank_Siege_Mode;
              knownUnit.isStasised              = bwUnit.stasisTimer > 0;
              knownUnit.isStimmed               = bwUnit.stimTimer > 0;

              knownUnit.isUnderStorm            = bwUnit.isUnderStorm != 0;
              knownUnit.isUnpowered             =  typeData.race == RaceIds::Protoss
                                                && typeData.isBuilding
                                                && bwUnit.status.getBit<BW::StatusFlags::DoodadStatesThing>();
            } //if(isKnown)
          } //foreach flag
        } //foeach flagarray word
      } //fill buffers with recend world state data

      // update remote shared memry
      BridgeServer::updateRemoteSharedMemory();

      // call OnFrame RPC
      BridgeServer::invokeOnFrame();

      // iterate over commands that were issued this frame
      // sort them into the correspondant command entry arrays
      {
        // callback based handling, bool indicates whether to proceed
        static Util::TypedPacketSwitch<int> packetSwitch;
        if(!packetSwitch.getHandlerCount())
        {
          // init packet switch
          packetSwitch.addHandler(handleCommandSendText);
          packetSwitch.addHandler(handleCommandStartGame);
          packetSwitch.addHandler(handleCommandPauseGame);
          packetSwitch.addHandler(handleCommandLeaveGame);
          packetSwitch.addHandler(handleCommandResumeGame);
          packetSwitch.addHandler(handleCommandRestartGame);
          packetSwitch.addHandler(handleCommandSetLocalSpeed);
          packetSwitch.addHandler(handleCommandOrderUnit);
        }
        // clear containers to receive next batch
        commandOrderEntries.clear();

        for(Bridge::SharedStuff::CommandStack::Index index = BridgeServer::sharedStuff.commands.begin();
            index.isValid();
            index = BridgeServer::sharedStuff.commands.getNext(index))
        {
          bool proceed = packetSwitch.handlePacket(BridgeServer::sharedStuff.commands.get(index));
          if(!proceed)
            break;
        }
      }

      // process issued unit commands
      {
        struct LatencyCommandEntry
        {
          bool valid;
          int startFrame;
          BWAPI::UnitCommand command;
        };
        static std::deque<LatencyCommandEntry> latencyCommandQueue;

        // execute and recruit new commands into the latency simulation queue
        {
          saveSelected();

          for each(Bridge::CommandEntry::UnitOrder* order in commandOrderEntries)
          {
            BWAPI::UnitCommand& command = order->unitCommand;

            // TODO: validate command before executing it
            executeUnitCommand(command);

            LatencyCommandEntry entry;
            entry.valid = true;
            entry.startFrame = frameCount;
            entry.command = command;
            latencyCommandQueue.push_back(entry);
          }

          loadSelected();
        }

        // Remove commands that are too old
        {
          int ageLimit = BW::getLatency();

          for(std::deque<LatencyCommandEntry>::iterator it;
            latencyCommandQueue.begin() != latencyCommandQueue.end();
            )
          {
            it = latencyCommandQueue.begin();

            // check current entry
            LatencyCommandEntry &entry = (*it);
            int age = (frameCount - entry.startFrame);
            if(age < ageLimit)
              // from here on these entries are fresh enough
              break;
            // remove this outdated entry
            latencyCommandQueue.erase(it);
          }
        }

        // run simulation on the rest
        {
          std::deque<LatencyCommandEntry>::iterator it = latencyCommandQueue.begin();
          for(; it != latencyCommandQueue.end(); it++)
          {
            // check current entry
            LatencyCommandEntry &entry = (*it);

            // skip invalidated entries (removing them costs too much)
            if(!entry.valid)
              continue;

            // simulate this command
            KnownUnit &unitEntry = BridgeServer::gameData->units.at(entry.command.unitIndex);
            simulateUnitCommand(entry.command, unitEntry);
          }
        }
      }
    }
    //-------------------------------------- ON GAME START -----------------------------------------------------
    void onGameStart()
    {
      try
      {
        BridgeServer::initConnectionServer();
      }
      catch(GeneralException &e)
      {
        e.append("initializing bridge server");
        throw;
      }
    }
    //----------------------------------- SHARE MATCH START ----------------------------------------------------
    void shareMatchStart(bool fromStart)
    {
      // reset shared memory areas
      BridgeServer::sharedStuff.events.clear();
      BridgeServer::gameData->units.clear();
      BridgeServer::gameData->forces.clear();
      BridgeServer::gameData->players.clear();
      BridgeServer::gameData->startLocations.clear();

      // fill the const part of static data, for the rest of the match
      BWAPI::StaticGameData &staticData = *BridgeServer::gameData;

      // fill the map data
      for (int x=0;x<Map::getWidth()*4;x++)
        for (int y=0;y<Map::getHeight()*4;y++)
        {
          staticData.getGroundHeight[x][y] = Map::groundHeight(x,y);
          staticData.isWalkable[x][y] = Map::walkable(x,y);
        }
      for (int x=0;x<Map::getWidth();x++)
        for (int y=0;y<Map::getHeight();y++)
          staticData.isBuildable[x][y] = Map::buildable(x,y);

      staticData.mapFilename.set(Map::getFileName());

      staticData.mapName.set(Map::getName());

      staticData.mapSize       = BuildPosition(Map::getWidth(), Map::getHeight());
      staticData.mapHash       = Map::getMapHash();

      // get the start locations
      {
        BW::Positions* posptr = BW::BWDATA_startPositions;
        while (posptr->x != 0 || posptr->y != 0)
        {
          BridgeServer::gameData->startLocations.push_back(Position(
            posptr->x - BW::TILE_SIZE*2,
            posptr->y - (int)(BW::TILE_SIZE*1.5)) / 32);
          posptr++;
        }
      }

      // init all 4 forces
      for(int i = 0; i < 4; i++)
      {
        Force &force = BridgeServer::gameData->forces.push_back();
        force.name.set(BW::BWDATA_ForceNames[i].name);
      }

      // find all players
      for (int i = 0; i < BW::PLAYER_COUNT; i++)
      {
        BW::Players::PlayerInfo &bwPlayer = BW::BWDATA_Players->player[i];

        // is this a valid player slot?
        if(bwPlayer.type != BW::PlayerTypeIds::Human
          && bwPlayer.type != BW::PlayerTypeIds::Computer
          && i != 11)
        continue;

        // transfer data
        Player &player = BridgeServer::gameData->players.allocate(i);
        player.force = bwPlayer.force;
        player.type = (PlayerTypeId)bwPlayer.type;
        player.name.set(bwPlayer.name);
      }

      // some other const data
      staticData.isMultiplayer = BW::isMultiplayer();
      staticData.isReplay      = BW::isInReplay();

      // invoke onStartGame RPC
      try
      {
        BridgeServer::invokeOnStartMatch(fromStart);
      }
      catch(GeneralException &exception)
      {
        BridgeServer::disconnect();
        BW::printf("disconnected. %s\n", exception.getMessage().c_str());
      }
    }
    //-------------------------------------- ON MATCH FRAME FIRST ----------------------------------------------
    void onMatchFrameFirst()
    {
      // init BW's interface
      BW::onMatchInit();

      // init database
      UnitTypes::init();
      TechTypes::init();
      UpgradeTypes::init();
      WeaponTypes::init();
      DamageTypes::init();
      ExplosionTypes::init();
      Races::init();
      UnitSizeTypes::init();

      // enable user input as long as no agent is in charge
      //if(!BridgeServer::isAgentConnected()) TODO: uncomment
        Engine::enableFlag(Flags::UserInput);

      // reset frame count
      frameCount = 0;

      // load map data
      Map::load();

      // init bwUnitTableMirror
      {
        // mark all array as unused
        for(int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
        {
          bwUnitArrayMirror[i].isInChain = false;
          bwUnitArrayMirror[i].wasInChain = false;
          bwUnitArrayMirror[i].isDying = false;
          bwUnitArrayMirror[i].knownUnit = NULL;
        }
      }

      // init flag array
      bwUnitArrayMirrorFlags.setSize(BW::UNIT_ARRAY_MAX_LENGTH);
      bwUnitArrayMirrorFlags.setAllFlags(false);

      // set all the flags to the default of disabled
      for (int i = 0; i < Flags::count; i++)
        flags[i] = false;

      // init shared memory, if agent is connected during the first match frame
      if(BridgeServer::isAgentConnected())
        shareMatchStart(true);
    }
    //-------------------------------------- HANDLE AGENT CONNECTION -------------------------------------------
    void acceptIncomingConnections()
    {
      if(BridgeServer::isBridgeInitialized()
        && !BridgeServer::isAgentConnected())
      {
        BridgeServer::acceptIncomingConnections();
        if(BridgeServer::isAgentConnected() && gameState == InMatch)
        {
          // in the event of the agent connecting during a match
          shareMatchStart(false);
        }
      }
    }
    //---------------------------------------------- ON MENU FRAME ---------------------------------------------
    void onMenuFrame()
    {
      lastState = gameState;
      gameState = InMenu;

      // events
      if(lastState != InMenu)
      {
        if(lastState == Startup)
          onGameStart();
      }

      acceptIncomingConnections();

      Tracer::onMatchFrame();
    }
    //---------------------------------------------- ON MATCH FRAME --------------------------------------------
    void onMatchFrame()
    {
      lastState = gameState;
      gameState = InMatch;

      // events
      if(lastState != InMatch)
      {
        onMatchFrameFirst();
      }

      acceptIncomingConnections();

      if(BridgeServer::isAgentConnected())
        shareMatchFrame();

      // count frames
      frameCount++;
    }
    //---------------------------------------- ON MATCH END ----------------------------------------------------
    void onMatchEnd()
    {
      // we're not technically in match anymore.
      // This is needed to reinit a match when restarting a game, since then
      // onMenuFrame is not called
      gameState = InMenu;
    }
    //---------------------------------------- ON MESSAGE INTERCEPTED ------------------------------------------
    void onMessageIntercepted(const char* text)
    {
      if (parseText(text) || !flags[Flags::UserInput])
        return;

      if(BridgeServer::isAgentConnected())
      {
        // save text as userInput
        BridgeServer::gameData->userInput.set(text);
      }
      else
      {
        BW::sendText("%s", text);
      }
    }
    //---------------------------------------- EACH DRAW SHAPE -------------------------------------------------
    void eachDrawShape(Util::MemoryFrame shapePacket)
    {
      int type = shapePacket.getAs<int>();
      if(type == Bridge::DrawShape::Text::_typeId)
      {
        Bridge::DrawShape::Text text;
        shapePacket.readTo(text);
        if(!shapePacket.size())
        {
          // packet too small
          throw GeneralException(__FUNCTION__ ": text shape packet too small for text");
        }
        if(shapePacket.endAs<char>()[-1] != 0)
        {
          // not null terminated
          throw GeneralException(__FUNCTION__ ": text shape packet text not null terminated");
        }
        BW::drawText(text.pos.x, text.pos.y, shapePacket.beginAs<char>());
      }
      if(type == Bridge::DrawShape::Line::_typeId)
      {
        Bridge::DrawShape::Line line;
        shapePacket.readTo(line);
        drawLine(line.from.x, line.from.y, line.to.x, line.to.y, line.color);
      }
      if(type == Bridge::DrawShape::Rectangle::_typeId)
      {
        Bridge::DrawShape::Rectangle rect;
        shapePacket.readTo(rect);
        drawRectangle(rect.pos.x, rect.pos.y, rect.size.x, rect.size.y, rect.color, rect.isSolid);
      }
      if(type == Bridge::DrawShape::Circle::_typeId)
      {
        Bridge::DrawShape::Circle circle;
        shapePacket.readTo(circle);
        drawCircle(circle.center.x, circle.center.y, circle.radius, circle.color, circle.isSolid);
      }
      if(type == Bridge::DrawShape::Dot::_typeId)
      {
        Bridge::DrawShape::Dot dot;
        shapePacket.readTo(dot);
        BW::drawDot(dot.pos.x, dot.pos.y, dot.color);
      }
    }
    //---------------------------------------- ON MATCH DRAW HIGH ----------------------------------------------
    void onMatchDrawHigh() throw()
    {
      Tracer::onDraw();
      if(BridgeServer::isAgentConnected())
        BridgeServer::enumAllDrawShapes(eachDrawShape);
    }
    //---------------------------------------------- GLOBAL EXCEPTION HANDLER ----------------------------------
    void onException(const char* text)
    {
      bool wasConnected = BridgeServer::isAgentConnected();
      BridgeServer::disconnect();
      if(BW::isInGame())
      {
        if(wasConnected)
          BW::printf("disconnected: %s\n", text);
        else
          BW::printf("exception: %s\n", text);
      }
    }
    void callAndHandleExceptions(void (*function)()) throw()
    {
      try
      {
        function();
      }
      catch(GeneralException &e)
      {
        onException(e.getMessage().c_str());
      }
      catch(std::exception &e)
      {
        onException(e.what());
      }
    }
    template<typename T> void callAndHandleExceptions(void (*function)(T), T parameter) throw()
    {
      try
      {
        function(parameter);
      }
      catch(GeneralException &e)
      {
        onException(e.getMessage().c_str());
      }
      catch(std::exception &e)
      {
        onException(e.what());
      }
    }
    //---------------------------------------------- HOOK HANDLERS ---------------------------------------------
    void _onMenuFrame() throw()
    {
      callAndHandleExceptions(onMenuFrame);
    }
    void _onMatchFrame() throw()
    {
      callAndHandleExceptions(onMatchFrame);
    }
    void _onMatchEnd() throw()
    {
      callAndHandleExceptions(onMatchEnd);
    }
    void _onUnitKilled(BW::Unit* unit) throw()
    {
      // only called when the unit is killed, not if it's simply removed or other death reasons
    }
    void _onMessageIntercepted(const char* text) throw()
    {
      callAndHandleExceptions(onMessageIntercepted, text);
    }
    void _onMatchDrawHigh() throw()
    {
      callAndHandleExceptions(onMatchDrawHigh);
    }
    //------------------------------------------------- ON DLL LOAD --------------------------------------------
    void _onDllLoad() throw()
    {
      // stop!!!
      // please reconsider, this thread is not the starcraft thread. hardly foreseeable future bugs can
      // be avoided altogether by moving initialisation code to update()
      // throw something here to unload the dll and signal dll initialisation as failed
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
        BW::leaveGame();
        return true;
      }
      else if (parsed[0] == "/latency")
      {
        BW::printf("latency: %d", BW::getLatency());
        return true;
      }
      else if (parsed[0] == "/speed")
      {
        if (parsed[1] != "")
          BW::setLocalSpeed(atoi(parsed[1].c_str()));
        else
          BW::resetLocalSpeed();
        return true;
      }
      else if (parsed[0] == "/restart")
      {
        BW::restartGame();
        return true;
      }
      else if (parsed[0] == "/tracer")
      {
        if(!Tracer::command(parsed))
        {
          BW::printf("unknown tracer command");
        }
        return true;
      }
      return false;
    }
    //----------------------------------------------------------------------------------------------------------
    void refresh()
    {
      /* Unusued
          TODO: find out what "Unused" mean? what does this function do? decide whether move to BW:: or remove
      #ifdef __GNUC__
        __asm__("call [BW::BWFXN_Refresh]");
      #else
        __asm call BW::BWFXN_Refresh
      #endif
      */
    }
    //---------------------------------------- ORDER SIMULATION ------------------------------------------------
    void simulateUnitStop(const BWAPI::UnitCommand& command, BWAPI::UnitState& state)
    {
      // TODO: add alternation of state
    }

    void simulateUnknownUnitCommand(const BWAPI::UnitCommand& command, BWAPI::UnitState& state)
    {
      throw GeneralException("Unknown command " + Util::Strings::intToString(command.commandId));
    }

    void simulateUnitCommand(const BWAPI::UnitCommand& command, BWAPI::UnitState& state)
    {
      typedef void(*SIMULATOR)(const BWAPI::UnitCommand&, BWAPI::UnitState&);
      static Util::LookupTable<SIMULATOR> simulators;
      static bool isSimulatorsTableInitialized = false;

      // initialize lookup table
      if(!isSimulatorsTableInitialized)
      {
        simulators.setDefaultValue(&simulateUnknownUnitCommand);
        simulators.setValue(UnitCommandTypeIds::Stop, &simulateUnitStop);
        isSimulatorsTableInitialized = true;
      }

      // get command simulator function and call it
      SIMULATOR simulator = simulators.lookUp(command.commandId);
      simulator(command, state);
    }
    //---------------------------------------- ORDER EXECUTION -------------------------------------------------
    void executeSelectOrder(int bwUnitIndex)
    {
      BW::Unit* select;
      select = &BW::BWDATA_UnitNodeTable->unit[bwUnitIndex];
      BW::selectUnits(1, &select);
    }

    void executeUnitStop(int bwUnitIndex, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Stop(0));
    }

    void executeUnknownUnitCommand(int bwUnitIndex, const BWAPI::UnitCommand& command)
    {
      throw GeneralException("Unknown command " + Util::Strings::intToString(command.commandId));
    }

    void executeUnitCommand(const BWAPI::UnitCommand& command)
    {
      typedef void(*EXECUTOR)(int unitId, const BWAPI::UnitCommand&);
      static Util::LookupTable<EXECUTOR> executors;
      static bool isExecutorsTableInitialized = false;

      // initialize lookup table
      if(!isExecutorsTableInitialized)
      {
        executors.setDefaultValue(&executeUnknownUnitCommand);
        executors.setValue(UnitCommandTypeIds::Stop, &executeUnitStop);
        isExecutorsTableInitialized = true;
      }

      // get command simulator function
      EXECUTOR executor = executors.lookUp(command.commandId);

      // find out the unitId.
      // TODO: add a knownunit mirror, lookup the bwunit index in the reflected knownunit entry
      // but for now just search the bwunit mirror for a reference to this knownunit
      int bwUnitIndex = -1;
      for(int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      {
        if(!bwUnitArrayMirror[i].wasInChain)
          continue;
        if(bwUnitArrayMirror[i].knownUnitIndex == command.unitIndex)
        {
          bwUnitIndex = i;
          break;
        }
      }
      if(bwUnitIndex == -1)
        return; // the unit does not exist anymore;

      executor(bwUnitIndex, command);
    }
    //---------------------------------------- -----------------------------------------------------------------
  }
};
