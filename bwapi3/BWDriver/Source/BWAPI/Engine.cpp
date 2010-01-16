#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "Engine.h"
#include "UnitMirror.h"
#include "KnownUnitMirror.h"
#include "UnitChainId.h"
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

#include <BW\Broodwar.h>
#include <BW\Hook.h>
#include <BW\Unit.h>
#include <BW\UnitArray.h>
#include <BW\Offsets.h>
#include <BW\UnitID.h>
#include <BW\Command.h>
#include <BW\Latency.h>
#include <BW\TileType.h>
#include <BW\TileSet.h>
#include <BW\UnitType.h>
#include <BW\UnitTypeID.h>
#include <BW\GameType.h>
#include <BW\WeaponType.h>
#include <BW\CheatType.h>
#include <BW\RaceID.h>
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

#include <Bridge\PipeMessage.h>
#include <Bridge\SharedStuff.h>
#include <Bridge\Constants.h>

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
    UnitMirror bwUnitArrayMirror[BW::UNIT_ARRAY_MAX_LENGTH];

    // index correlated with the known unit array as found in StaticGameData.
    // holds private data that the AI needn't to know about
    // hold when the known unit is invalid
    KnownUnitMirror knownUnitArrayMirror[BW::UNIT_ARRAY_MAX_LENGTH];

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
    int handleCommandEnableFlag(Bridge::CommandEntry::EnableFlag& packet)
    {
      int flag = packet.flag;
      // Enable the specified flag
      if (flag >= Flags::count)
      {
        BW::sendText("Invalid flag (%d).", flag);
      }
      else
      {
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
      return true;
    }
    int handleCommandPingMinimap(Bridge::CommandEntry::PingMinimap& packet)
    {
      BW::issueCommand(BW::Command::MinimapPing(packet.pos));
      return true;
    }
    int handleCommandSetScreenPosition(Bridge::CommandEntry::SetScreenPosition& packet)
    {
      BW::setScreenPosition(packet.pos);
      return true;
    }
    std::vector<Bridge::CommandEntry::UnitOrder*> commandOrderEntries;
    int handleCommandOrderUnit(Bridge::CommandEntry::UnitOrder& packet)
    {
      // for later processing in tighter loops
      commandOrderEntries.push_back(&packet);
      return true;
    }
    //------------------------------ ADD KNOWN UNIT ----------------------------------------------
    int shareAddKnownUnit(KnownUnit **out_pKnownUnit, UnitAddEventTypeId reason, int bwUnitId)
    {
      // check prerequisites
      _ASSERT(BridgeServer::isAgentConnected());

      // insert new known unit into the shared set
      int index = BridgeServer::addKnownUnit(out_pKnownUnit, reason);

      // init the mirror (index correlated)
      knownUnitArrayMirror[index].bwUnitId = bwUnitId;

      return index;
    }
    //------------------------------ REMOVE KNOWN UNIT -------------------------------------------
    void shareRemoveKnownUnit(int knownUnitId, UnitRemoveEventTypeId reason)
    {
      // remove the known unit from shared memory
      BridgeServer::removeKnownUnit(knownUnitId, reason);

      // mark the backreferenced bwUnitId as invalid
      knownUnitArrayMirror[knownUnitId].bwUnitId = -1;
    }
    //----------------------------------- SHARE CLEAR FRAME BY FRAME BUFFERS -----------------------------------
    void shareClearFrameByFrameBuffers()
    {
      // any data that gets accumulated over the frame and presented to the AI has
      // to be cleaned out, so the accumulation in the next frame can begin.
      BridgeServer::gameData->userInput.set("");
    }
    //----------------------------------- SHARE EACH BW UNIT ---------------------------------------------------
    bool shareEachBwUnit(BW::Unit &bwUnit, UnitChainId currentChain, bool isOnMatchStart)
    {
      // get corresponding index and mirror
      int bwUnitIndex = BW::BWDATA_UnitNodeTable->getIndexByUnit(&bwUnit); // get linear index
      UnitMirror &bwUnitMirror = bwUnitArrayMirror[bwUnitIndex];

      // process the chain states
      UnitChainId lastChain = bwUnitMirror.chain;     // we need these values for this iteration
      bwUnitMirror.chain = currentChain;              // init the value for next iteration

      // classify slot state
      bool isUsed = currentChain != UnitChainIds::Unused;
      bool wasUsed = lastChain != UnitChainIds::Unused;

      // if this slot is not used, it is not longer interesting
      if(!isUsed)
      {
        if(wasUsed)
        {
          // unit perished. If it had a known Unit associated with it, remove the
          // known unit too
          if(bwUnitMirror.knownUnit)
          {
            shareRemoveKnownUnit(bwUnitMirror.knownUnitIndex, UnitRemoveEventTypeIds::Died);
            bwUnitMirror.knownUnit = NULL;
            bwUnitMirror.knownUnitIndex = -1;
          }
          return true;
        }
        // this is a !wasUsed chain in a NotUsed Chain
        // the resturn value indicate that "we didn;t even touch it"
        return false;
      }

      // ASSUMPTION: the slot is being used
      _ASSERT(isUsed);

      if(!wasUsed)
      {
        // unit did not exist before. init the reflection for this unit
        bwUnitMirror.isDying = false;
        bwUnitMirror.isNoticed = false;
        bwUnitMirror.knownUnit = NULL;
        bwUnitMirror.knownUnitIndex = -1;
        bwUnitMirror.position = bwUnit.position;
      }

      // TODO: find out if the unit is dying. not urgent tho, only for a one in a million case
      // isDying should be identified as true the last frame(s) before it is being removed
      bool isDying = false; // change this false;
      bool wasDying = bwUnitMirror.isDying;
      bwUnitMirror.isDying = isDying;
      if(!isDying && wasDying)
      {
        // the unit is not dying anymore? It's a new one
        // remove known unit if associated, tell its dead.
        if(bwUnitMirror.knownUnit)
        {
          shareRemoveKnownUnit(bwUnitMirror.knownUnitIndex, UnitRemoveEventTypeIds::Died);
          bwUnitMirror.knownUnit = NULL;
          bwUnitMirror.knownUnitIndex = -1;
          bwUnitMirror.position = bwUnit.position;
        }
      }

      // calculate clearance level
      ClearanceLevel clearance = ClearanceLevels::None;

      // the unit is fully accessible if it's yours
      do
      {
        if(bwUnit.playerID == BW::selfPlayerId)
        {
          clearance = ClearanceLevels::Full;
          break;
        }

        // unit does not belong to you

        // visibility overrides
        bool isInVisibleArea = isOnMatchStart || flags[Flags::CompleteMapInformation];

        if(!isInVisibleArea)
        {
          // visibility of the unit
          BW::Position buildPosition = bwUnit.position / 32;
          isInVisibleArea = Map::visible(buildPosition.x, buildPosition.y);
        }

        // handle noticed state
        if(isInVisibleArea)
        {
          // notice the unit if it moves
          if(bwUnitMirror.position != bwUnit.position)
            bwUnitMirror.isNoticed = true;
        }
        else
        {
          bwUnitMirror.isNoticed = false;
        }

        // only known if its in the visible area
        if(!isInVisibleArea)
          break;

        bool isCloacked = bwUnit.status.getBit<BW::StatusFlags::Cloaked>();

        if(!isCloacked)
        {
          clearance = ClearanceLevels::Visible;
          break;
        }

        bool isDetected = false;
        if(bwUnit.sprite)
        {
          if(BW::isInReplay())
            isDetected = bwUnit.sprite->visibilityFlags > 0;
          else
            // TODO: find a isDetected expression that works, this one just returns if the unit is in visible area
            isDetected = !!(bwUnit.sprite->visibilityFlags & (1 << BW::selfPlayerId));
        }

        if(isDetected)
        {
          // visible if it's detected
          clearance = ClearanceLevels::Visible;
          break;
        }
        
        if(bwUnitMirror.isNoticed)
        {
          // can be noticed if not already detected
          clearance = ClearanceLevels::Noticed;
          break;
        }
      } while(false);

      // handle noticed state
      bwUnitMirror.position = bwUnit.position;

      // classify clarance level
      bool isKnown = clearance != ClearanceLevels::None;

      // if knownability changed
      if(!!bwUnitMirror.knownUnit != isKnown)
      {
        if(isKnown) // unit becomes known
        {
          // reserve a KnownUnitEntry and store it's address so it gets filled
          bwUnitMirror.knownUnitIndex = shareAddKnownUnit(&bwUnitMirror.knownUnit, UnitAddEventTypeIds::Created, bwUnitIndex);
        }
        else // unit becomes not known
        {
          // release KnownUnit address
          shareRemoveKnownUnit(bwUnitMirror.knownUnitIndex, UnitRemoveEventTypeIds::Died);
          bwUnitMirror.knownUnit = NULL;
          bwUnitMirror.knownUnitIndex = -1;
        }
      }

      if(isKnown)
      {
        // transfer recent data about this particular BW unit
        KnownUnit &knownUnit = *bwUnitMirror.knownUnit;

        // TODO: implement clearance limit
        UnitTypeId type = (UnitTypeId)bwUnit.unitID.id;
        UnitType& typeData = UnitTypes::unitTypeData[type];

        knownUnit.clearanceLevel          = clearance;
        knownUnit.debug                   = bwUnit.sprite->visibilityFlags;

        knownUnit.position                = Position(bwUnit.position);
        knownUnit.type                    = type;
        knownUnit.velocity                = Util::Point<double>(bwUnit.current_speedX, bwUnit.current_speedY) / 256.0;
        {
          int d = bwUnit.currentDirection;
          // turn so zero faces  east
          d -= 64;
          if (d < 0) d += 256;
          // convert to radians
          double a = (double)d*3.14159265358979323846/128.0;
          // store
          knownUnit.angle                 = a;
        }

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

        // fill action
        {
          BW::Unit* bwTargetUnit          = bwUnit.orderTargetUnit;
          if(bwTargetUnit)
          {
            int targetUnitIndex           = BW::BWDATA_UnitNodeTable->getIndexByUnit(bwTargetUnit);
            knownUnit.order.targetUnit    = bwUnitArrayMirror[targetUnitIndex].knownUnitIndex;
          }
          else
            knownUnit.order.targetUnit    = -1;
          knownUnit.order.targetPosition  = bwUnit.orderTargetPos;
          knownUnit.order.type            = (OrderTypeId)bwUnit.orderID;
        }
        {
          BW::Unit* bwTargetUnit          = bwUnit.targetUnit;
          if(bwTargetUnit)
          {
            int targetUnitIndex           = BW::BWDATA_UnitNodeTable->getIndexByUnit(bwTargetUnit);
            knownUnit.movementTargetUnit  = bwUnitArrayMirror[targetUnitIndex].knownUnitIndex;
          }
          else
            knownUnit.movementTargetUnit  = -1;
        }
        knownUnit.movementTargetPosition  = bwUnit.moveToPos;
        knownUnit.movementNextWaypoint    = bwUnit.nextWaypoint;

        {
          BW::Unit* bwBuildUnit           = bwUnit.currentBuildUnit;
          if(bwBuildUnit)
          {
            int buildUnitIndex            = BW::BWDATA_UnitNodeTable->getIndexByUnit(bwBuildUnit);
            knownUnit.buildUnit           = bwUnitArrayMirror[buildUnitIndex].knownUnitIndex;
            knownUnit.trainTimer          = bwBuildUnit->remainingBuildTime;
          }
          else
            knownUnit.buildUnit           = -1;
        }

        // fill training queue
        {
          knownUnit.trainingQueue.clear();
          int index = bwUnit.buildQueueSlot;
          int count = 0;
          while(count < 5)
          {
            BW::UnitType unitType = bwUnit.buildQueue[index];
            if(unitType.id == BW::UnitTypeIDs::None)
              break;
            knownUnit.trainingQueue.push_back((UnitTypeId)unitType.id);
            index = (index + 1) % 5;
            count++;
          }
        }
      } //if(isKnown)
      return true;
    }
    //----------------------------------- SHARE MATCH FRAME ----------------------------------------------------
    void shareMatchFrame(bool isOnMatchStart)
    {
      // fill buffers with recent world state data
      {
        BWAPI::StaticGameData &staticData = *BridgeServer::gameData;

        // match data
        {
          staticData.getLatency     = BW::getLatency();
          staticData.frameCount     = frameCount;
          staticData.isPaused       = BW::isPaused();
        }

        // user input data
        {
          staticData.mousePosition  = Position(BW::getMouseX(), BW::getMouseY());
          staticData.screenPosition = Position(BW::getScreenX(), BW::getScreenY());
          for (int x=0;x<Map::getWidth();x++)
          {
            for (int y=0;y<Map::getHeight();y++)
            {
              staticData.isVisible[x][y] = Map::visible(x, y);
              staticData.isExplored[x][y] = Map::isExplored(x, y);
              staticData.hasCreep[x][y] = Map::hasCreep(x, y);
            }
          }
        }

        // fresh player data
        for each(int playerId in staticData.players)
        {
          // players container is index correlated with bw's playerids
          int bwPlayerId = playerId;
          Player& player = staticData.players[playerId];

          if(playerId == BW::selfPlayerId
            || BW::isInReplay() || flags[Flags::CompleteMapInformation])
          {
            player.minerals           = BW::BWDATA_PlayerResources->minerals          .player[bwPlayerId];
            player.gas                = BW::BWDATA_PlayerResources->gas               .player[bwPlayerId];
            player.cumulativeMinerals = BW::BWDATA_PlayerResources->cumulativeMinerals.player[bwPlayerId];
            player.cumulativeGas      = BW::BWDATA_PlayerResources->cumulativeGas     .player[bwPlayerId];
            // for all 3 races
            for(int r = 0; r < 3; r++)
            {
              player.suppliesAvailable[r] = BW::BWDATA_Supplies->race[r].available.player[bwPlayerId];
              player.suppliesUsed[r]      = BW::BWDATA_Supplies->race[r].used     .player[bwPlayerId];
            }
            // for all other players
            for each(int otherPlayerId in staticData.players)
            {
              int bwOtherPlayerId = otherPlayerId;
              player.attitudes[otherPlayerId].ally = BW::BWDATA_Alliance->alliance[bwPlayerId].player[bwOtherPlayerId] != 0;
            }
          }
        }

        // traverse all broodwar's unit chains
        for(BW::Unit *bwUnit = *BW::BWDATA_UnitNodeChain_UnusedUnit_First; bwUnit; bwUnit = bwUnit->nextUnit)
        {
          shareEachBwUnit(*bwUnit, UnitChainIds::Unused, isOnMatchStart);
        }
        for(BW::Unit *bwUnit = *BW::BWDATA_UnitNodeChain_VisibleUnit_First; bwUnit; bwUnit = bwUnit->nextUnit)
        {
          shareEachBwUnit(*bwUnit, UnitChainIds::Visible, isOnMatchStart);
        }
        for(BW::Unit *bwUnit = *BW::BWDATA_UnitNodeChain_ScannerSweep_First; bwUnit; bwUnit = bwUnit->nextUnit)
        {
          shareEachBwUnit(*bwUnit, UnitChainIds::ScannerSweep, isOnMatchStart);
        }
        for(BW::Unit *bwUnit = *BW::BWDATA_UnitNodeChain_HiddenUnit_First; bwUnit; bwUnit = bwUnit->nextUnit)
        {
          shareEachBwUnit(*bwUnit, UnitChainIds::Hidden, isOnMatchStart);
        }

        // save selection group
        staticData.selectedUnits.clear();
        for(BW::Unit** bwUnitRunner = BW::BWDATA_CurrentPlayerSelectionGroup; *bwUnitRunner; bwUnitRunner++)
        {
          int bwUnitIndex = BW::BWDATA_UnitNodeTable->getIndexByUnit(*bwUnitRunner);
          UnitId knownUnitIndex = bwUnitArrayMirror[bwUnitIndex].knownUnitIndex;
          // if no known unit associated
          if(knownUnitIndex == -1)
            continue;
          staticData.selectedUnits.push_back(knownUnitIndex);
        }
      } //fill buffers with recend world state data

      // update remote shared memry
      BridgeServer::updateRemoteSharedMemory();

      // call OnFrame RPC
      BridgeServer::invokeOnFrame();

      // frame by frame data lifetime expired now
      shareClearFrameByFrameBuffers();

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
          packetSwitch.addHandler(handleCommandEnableFlag);
          packetSwitch.addHandler(handleCommandPingMinimap);
          packetSwitch.addHandler(handleCommandSetScreenPosition);

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
              break;
            // remove this outdated entry
            latencyCommandQueue.erase(it);
          }
        }
        // from here on these entries are fresh enough

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
      // set all the flags to the default of disabled
      for (int i = 0; i < Flags::count; i++)
        flags[i] = false;

      // reset known unit container and its mirror
      BridgeServer::gameData->units.clear();
      for each(KnownUnitMirror &mirror in knownUnitArrayMirror)
      {
        mirror.bwUnitId = -1;
      }

      // fill the const part of static data, for the rest of the match
      BWAPI::StaticGameData &staticData = *BridgeServer::gameData;

      // fill the map data
      for (int x=0;x<Map::getWidth()*4;x++)
        for (int y=0;y<Map::getHeight()*4;y++)
        {
          staticData.getGroundHeight[x][y] = Map::groundHeight(x, y);
          staticData.isWalkable[x][y] = Map::walkable(x, y);
        }
      for (int x=0;x<Map::getWidth();x++)
        for (int y=0;y<Map::getHeight();y++)
          staticData.isBuildable[x][y] = Map::buildable(x, y);

      staticData.mapFilename.set(Map::getFileName());

      staticData.mapName.set(Map::getName());

      staticData.mapSize       = BuildPosition(Map::getWidth(), Map::getHeight());
      staticData.mapHash       = Map::getMapHash();

      // get the start locations
      {
        BridgeServer::gameData->startLocations.clear();

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
//      BridgeServer::gameData->forces.clear();
      for(int i = 0; i < 4; i++)
      {
        Force &force = BridgeServer::gameData->forces[i];
        force.name.set(BW::BWDATA_ForceNames[i].name);
      }

      // find all players
      staticData.self = BW::selfPlayerId;
      staticData.players.clear();
      for (int i = 0; i < BW::PLAYER_COUNT; i++)
      {
        BW::Players::PlayerInfo &bwPlayer = BW::BWDATA_Players->player[i];

        // is this a valid player slot?
        if(!bwPlayer.isValid())
          continue;

        // transfer data
        Player &player = BridgeServer::gameData->players.allocate(i);
        player.force = bwPlayer.force;
        player.type = (PlayerTypeId)bwPlayer.type;
        player.name.set(bwPlayer.name);
        player.race = (RaceId)bwPlayer.race;
      }

      // some other const data
      staticData.isMultiplayer = BW::isMultiplayer();
      staticData.isReplay      = BW::isInReplay();

      // clear frame by frame data, since this is the first frame
      shareClearFrameByFrameBuffers();

      // invoke onFrame RPC, but with match-start flags
      {
        staticData.isOnMatchStart = true;
        staticData.isFromBeginning = fromStart;
        shareMatchFrame(true);
        staticData.isOnMatchStart = false;
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

      // reset frame count
      frameCount = 0;

      // load map data
      Map::load();

      // init bwUnitTableMirror
      {
        // mark all array as unused
        for(int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
        {
          UnitMirror& mirror = bwUnitArrayMirror[i];
          mirror.chain          = UnitChainIds::Unused;
          mirror.isDying        = false;
          mirror.isNoticed      = false;
          mirror.knownUnit      = NULL;
          mirror.knownUnitIndex = -1;
        }
      }

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
        shareMatchFrame(false);
#ifdef _DEBUG
      Tracer::onMatchFrame();
#endif

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
      if (parseText(text))
        return;

      if(BridgeServer::isAgentConnected())
      {
        if(!flags[Flags::UserInput])
          return;

        // save text as userInput
        BridgeServer::gameData->userInput.set(text);
      }
      else
      {
        BW::sendText("%s", text);
      }
    }
    //---------------------------------------- EACH DRAW SHAPE -------------------------------------------------
    Position inline translateFromBase(Position pos, CoordinateBase base)
    {
      switch(base)
      {
      case CoordinateBases::Map:
        return pos - BW::getScreenPos();
      case CoordinateBases::Screen:
        return pos;
      case CoordinateBases::Mouse:
        return pos + BW::getMousePos();
      }
      throw GeneralException("invalid coordinate base specfied");
    }
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
        Position pos = translateFromBase(text.pos, text.base);
        BW::drawText(pos.x, pos.y, shapePacket.beginAs<char>());
      }
      if(type == Bridge::DrawShape::Line::_typeId)
      {
        Bridge::DrawShape::Line line;
        shapePacket.readTo(line);
        Position from = translateFromBase(line.from, line.base);
        Position to = translateFromBase(line.to, line.base);
        drawLine(from.x, from.y, to.x, to.y, line.color);
      }
      if(type == Bridge::DrawShape::Rectangle::_typeId)
      {
        Bridge::DrawShape::Rectangle rect;
        shapePacket.readTo(rect);
        Position pos = translateFromBase(rect.pos, rect.base);
        drawRectangle(pos.x, pos.y, rect.size.x, rect.size.y, rect.color, rect.isSolid);
      }
      if(type == Bridge::DrawShape::Circle::_typeId)
      {
        Bridge::DrawShape::Circle circle;
        shapePacket.readTo(circle);
        Position center = translateFromBase(circle.center, circle.base);
        drawCircle(center.x, center.y, circle.radius, circle.color, circle.isSolid);
      }
      if(type == Bridge::DrawShape::Ellipse::_typeId)
      {
        Bridge::DrawShape::Ellipse ellipse;
        shapePacket.readTo(ellipse);
        Position pos = translateFromBase(ellipse.pos, ellipse.base);
        drawEllipse(pos.x, pos.y, ellipse.sizex, ellipse.sizey, ellipse.color, ellipse.isSolid);
      }
      if(type == Bridge::DrawShape::Dot::_typeId)
      {
        Bridge::DrawShape::Dot dot;
        shapePacket.readTo(dot);
        Position pos = translateFromBase(dot.pos, dot.base);
        BW::drawDot(pos.x, pos.y, dot.color);
      }
      if(type == Bridge::DrawShape::Triangle::_typeId)
      {
        Bridge::DrawShape::Triangle triangle;
        shapePacket.readTo(triangle);
        Position posa = translateFromBase(triangle.posa, triangle.base);
        Position posb = translateFromBase(triangle.posb, triangle.base);
        Position posc = translateFromBase(triangle.posc, triangle.base);
        drawTriangle(posa.x, posa.y, posb.x, posb.y, posc.x, posc.y, triangle.color, triangle.isSolid);
      }
    }
    //---------------------------------------- ON MATCH DRAW HIGH ----------------------------------------------
    void onMatchDrawHigh() throw()
    {
#ifdef _DEBUG
      Tracer::onDraw();
#endif
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
    bool _onIssueCommand(int commandID) throw()
    {
      //decide if we should let the command go through
      if ( !BridgeServer::isAgentConnected() || flags[Flags::UserInput]
        || !BW::isInGame() || BW::isInReplay()
        //If user input is disabled, only allow the following commands to go through:
        || commandID == 0x00 // Game Chat
        || commandID == 0x05 // Keep Alive
        || commandID == 0x06 // Save Game
        || commandID == 0x07 // Load Game
        || commandID == 0x08 // Restart Game
        || commandID == 0x09 // Select         example: BW::Command::SelectSingle::_typeId
        || commandID == 0x0A // Shift Select
        || commandID == 0x10 // Pause Game
        || commandID == 0x11 // Resume Game
        || commandID == 0x37 // Game Hash
        || commandID == 0x3C // Start Game
        || commandID == 0x3D // Map Download %
        || commandID == 0x3E // Game Slot Modification
        || commandID == 0x3F // Unknown
        || commandID == 0x40 // Join Game
        || commandID == 0x41 // Race Change
        || commandID == 0x42 // Melee Force Change
        || commandID == 0x43 // UMS   Force Change
        || commandID == 0x44 // Slot Change
        || commandID == 0x45 // Swap Players
        || commandID == 0x48 // Game Init (Random Seed)
        || commandID == 0x49 // Info Request
        || commandID == 0x4A // Force Data Transfer
        || commandID == 0x4B // Force Name Transfer
        || commandID == 0x4C // Lobby Chat
        || commandID == 0x4E // Boot Player
        || commandID == 0x4F // Map Transfer
        || commandID == 0x54 // Mission Briefing Start
        || commandID == 0x55 // Set Latency
        || commandID == 0x56 // Change Replay Speed
        || commandID == 0x57 // Leave Game
        || commandID == 0x58 // Minimap Ping
        || commandID == 0x5B // Make Game Public
        || commandID == 0x5C // Replay Game Chat
        )
        return true;
      return false;
    }
    //------------------------------------------------- ON DLL LOAD --------------------------------------------
    void _onDllLoad() throw()
    {
      // stop!!!
      // please reconsider, this thread is not the starcraft thread. hardly foreseeable future bugs can
      // be avoided altogether by moving initialisation code to update()
      // throw anything here to unload the dll and signal dll initialisation as failed
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
    //---------------------------------------- ORDER SIMULATION ------------------------------------------------
    void simulateUnitStop(const BWAPI::UnitCommand& command, BWAPI::UnitState& state)
    {
      // TODO: add alternation of state
    }
    void simulateUnknownUnitCommand(const BWAPI::UnitCommand& command, BWAPI::UnitState& state)
    {
      throw GeneralException("Simulating unknown unit order " + Util::Strings::intToString(command.commandId));
    }
    void simulateUnitCommand(const BWAPI::UnitCommand& command, BWAPI::UnitState& state)
    {
      // TODO: add order latency-time simulation
      /*
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
      */
    }
    //---------------------------------------- ORDER EXECUTION -------------------------------------------------
    void executeSelectOrder(int bwUnitIndex)
    {
      BW::Unit* select;
      select = &BW::BWDATA_UnitNodeTable->unit[bwUnitIndex];
      BW::selectUnits(1, &select);
    }
    int findBwIndexByKnownUnitIndex(UnitId knownUnitId)
    {
      // lookup the bwunit index in the reflected knownunit entry
      int bwUnitIndex = knownUnitArrayMirror[knownUnitId].bwUnitId;
      if(bwUnitIndex == -1)
        throw GeneralException("Executing order on not existing unit id");

      // do a check
      _ASSERT(knownUnitId == bwUnitArrayMirror[bwUnitIndex].knownUnitIndex);

      return bwUnitIndex;
    }
    void executeUnitAttackPosition(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Attack(command.position, BW::OrderIDs::AttackMove));
    }
    void executeUnitAttackUnit(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Attack(findBwIndexByKnownUnitIndex(command.targetIndex), BW::OrderIDs::Attack1));
    }
    void executeUnitRightClickPosition(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::RightClick(command.position));
    }
    void executeUnitRightClickUnit(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::RightClick(findBwIndexByKnownUnitIndex(command.targetIndex)));
    }
    void executeUnitTrain(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      BW::UnitType type((u16)command.unitType);
      executeSelectOrder(bwUnitIndex);
      if (bwUnit.unitID == BW::UnitTypeIDs::Zerg_Larva ||
          bwUnit.unitID == BW::UnitTypeIDs::Zerg_Mutalisk ||
          bwUnit.unitID == BW::UnitTypeIDs::Zerg_Hydralisk)
      {
        BW::issueCommand(BW::Command::UnitMorph(type));
      }
      else if (bwUnit.unitID == BW::UnitTypeIDs::Zerg_Hatchery ||
               bwUnit.unitID == BW::UnitTypeIDs::Zerg_Lair ||
               bwUnit.unitID == BW::UnitTypeIDs::Zerg_Spire ||
               bwUnit.unitID == BW::UnitTypeIDs::Zerg_CreepColony)
      {
        BW::issueCommand(BW::Command::BuildingMorph(type));
      }
      else if (bwUnit.unitID == BW::UnitTypeIDs::Protoss_Carrier ||
               bwUnit.unitID == BW::UnitTypeIDs::Protoss_Reaver)
      {
        BW::issueCommand(BW::Command::TrainFighter());
      }
      else
      {
        BW::issueCommand(BW::Command::TrainUnit(type));
      }
    }
    void executeUnitBuild(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      BW::UnitType type((u16)command.unitType);
      executeSelectOrder(bwUnitIndex);
      if (bwUnit.unitID.isAddon())
        BW::issueCommand(BW::Command::MakeAddon(command.position, type));
      else
        BW::issueCommand(BW::Command::MakeBuilding(command.position, type));
    }
    void executeUnitBuildAddon(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      if (!bwUnit.unitID.isAddon())
        return;
      executeUnitBuild(bwUnitIndex, bwUnit, command);
    }
    void executeUnitResearch(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      u8 techenum = (u8)command.tech;
      BW::issueCommand(BW::Command::Invent(BW::TechType(techenum)));
    }
    void executeUnitUpgrade(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      u8 upgradeenum = (u8)command.upgrade;
      BW::issueCommand(BW::Command::Upgrade(BW::UpgradeType(upgradeenum)));
    }
    void executeUnitStop(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      if (bwUnit.unitID == BW::UnitTypeIDs::Protoss_Reaver)
        BW::issueCommand(BW::Command::ReaverStop());
      else if (bwUnit.unitID == BW::UnitTypeIDs::Protoss_Carrier)
        BW::issueCommand(BW::Command::CarrierStop());
      else
        BW::issueCommand(BW::Command::Stop(0));
    }
    void executeUnitHoldPosition(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::HoldPosition(0));
    }
    void executeUnitPatrol(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Attack(findBwIndexByKnownUnitIndex(command.targetIndex), BW::OrderIDs::Patrol));
    }
    void executeUnitFollow(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Attack(findBwIndexByKnownUnitIndex(command.targetIndex), BW::OrderIDs::Follow));
    }
    void executeUnitSetRallyPosition(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Attack(command.position, BW::OrderIDs::RallyPoint2));
    }
    void executeUnitSetRallyUnit(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Attack(findBwIndexByKnownUnitIndex(command.targetIndex), BW::OrderIDs::RallyPoint1));
    }
    void executeUnitRepair(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Attack(findBwIndexByKnownUnitIndex(command.targetIndex), BW::OrderIDs::Repair1));
    }
    void executeUnitReturnCargo(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::ReturnCargo(0));
    }
    void executeUnitMorph(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::UnitType rawtype((u16)command.unitType);
      if (bwUnit.unitID.isBuilding())
        BW::issueCommand(BW::Command::BuildingMorph(rawtype));
      else
        BW::issueCommand(BW::Command::UnitMorph(rawtype));
    }
    void executeUnitBurrow(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      if (!bwUnit.status.getBit<BW::StatusFlags::Burrowed>())
      {
        executeSelectOrder(bwUnitIndex);
        BW::issueCommand(BW::Command::Burrow());
      }
    }
    void executeUnitUnburrow(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      if (bwUnit.status.getBit<BW::StatusFlags::Burrowed>())
      {
        executeSelectOrder(bwUnitIndex);
        BW::issueCommand(BW::Command::Unburrow());
      }
    }
    void executeUnitSiege(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      if (bwUnit.unitID == BW::UnitTypeIDs::Terran_SiegeTankTankMode)
      {
        executeSelectOrder(bwUnitIndex);
        BW::issueCommand(BW::Command::Siege());
      }
    }
    void executeUnitUnsiege(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      if (bwUnit.unitID == BW::UnitTypeIDs::Terran_SiegeTankSiegeMode)
      {
        executeSelectOrder(bwUnitIndex);
        BW::issueCommand(BW::Command::Unsiege());
      }
    }
    void executeUnitCloak(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      if (!bwUnit.status.getBit<BW::StatusFlags::Cloaked>())
      {
        executeSelectOrder(bwUnitIndex);
        BW::issueCommand(BW::Command::Cloak());
      }
    }
    void executeUnitDecloak(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      if (bwUnit.status.getBit<BW::StatusFlags::Cloaked>())
      {
        executeSelectOrder(bwUnitIndex);
        BW::issueCommand(BW::Command::Decloak());
      }
    }
    void executeUnitLift(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      if (bwUnit.unitID.isBuilding())
      {
        if (!bwUnit.status.getBit<BW::StatusFlags::InAir>())
        {
          executeSelectOrder(bwUnitIndex);
          BW::issueCommand(BW::Command::Lift());
        }
      }
    }
    void executeUnitLand(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      if (bwUnit.unitID.isBuilding())
      {
        if (bwUnit.status.getBit<BW::StatusFlags::InAir>())
        {
          executeSelectOrder(bwUnitIndex);
          BW::issueCommand(BW::Command::Land(command.position,bwUnit.unitID));
        }
      }
    }
    void executeUnitLoad(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);

      int bwTargetUnitIndex = findBwIndexByKnownUnitIndex(command.targetIndex);
      const BW::Unit& bwTargetUnit = BW::BWDATA_UnitNodeTable->unit[bwTargetUnitIndex];

      if (bwUnit.unitID == BW::UnitTypeIDs::Terran_Bunker)
        BW::issueCommand(BW::Command::Attack(bwTargetUnitIndex, BW::OrderIDs::Pickup3));
      else if (bwUnit.unitID == BW::UnitTypeIDs::Terran_Dropship ||
               bwUnit.unitID == BW::UnitTypeIDs::Protoss_Shuttle ||
               bwUnit.unitID == BW::UnitTypeIDs::Zerg_Overlord)
        BW::issueCommand(BW::Command::Attack(bwTargetUnitIndex, BW::OrderIDs::Pickup2));
      else if (bwTargetUnit.unitID == BW::UnitTypeIDs::Terran_Bunker ||
               bwTargetUnit.unitID == BW::UnitTypeIDs::Terran_Dropship ||
               bwTargetUnit.unitID == BW::UnitTypeIDs::Protoss_Shuttle ||
               bwTargetUnit.unitID == BW::UnitTypeIDs::Zerg_Overlord)
        BW::issueCommand(BW::Command::RightClick(bwTargetUnitIndex));
    }
    void executeUnitUnload(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::UnloadUnit(findBwIndexByKnownUnitIndex(command.targetIndex)));
    }
    void executeUnitUnloadAll(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::UnloadAll());
    }
    void executeUnitUnloadAllPosition(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Attack(command.position,BW::OrderIDs::MoveUnload));
    }
    void executeUnitCancelConstruction(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::CancelConstruction());
    }
    void executeUnitHaltConstruction(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::Stop(0));
    }
    void executeUnitCancelMorph(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      if (bwUnit.unitID.isBuilding())
        BW::issueCommand(BW::Command::CancelConstruction());
      else
        BW::issueCommand(BW::Command::CancelUnitMorph());
    }
    void executeUnitCancelTrain(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::CancelTrainLast());
    }
    void executeUnitCancelTrainSlot(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::CancelTrain(command.extra));
    }
    void executeUnitCancelAddon(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::CancelAddon());
    }
    void executeUnitCancelResearch(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::CancelResearch());
    }
    void executeUnitCancelUpgrade(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      BW::issueCommand(BW::Command::CancelUpgrade());
    }
    void executeUnitUseTech(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);
      switch (command.tech)
      {
      case BW::TechIDs::Stimpacks:
        BW::issueCommand(BW::Command::UseStimPack());
        break;
      case BW::TechIDs::TankSiegeMode:
        if (bwUnit.unitID == BW::UnitTypeIDs::Terran_SiegeTankSiegeMode)
          executeUnitUnsiege(bwUnitIndex, bwUnit, command);
        else
          executeUnitSiege(bwUnitIndex, bwUnit, command);
        break;
      case BW::TechIDs::PersonnelCloaking:
      case BW::TechIDs::CloakingField:
        if(bwUnit.status.getBit<BW::StatusFlags::Cloaked>())
          executeUnitDecloak(bwUnitIndex, bwUnit, command);
        else
          executeUnitCloak(bwUnitIndex, bwUnit, command);
        break;
      case BW::TechIDs::Burrowing:
        if(bwUnit.status.getBit<BW::StatusFlags::Burrowed>())
          executeUnitUnburrow(bwUnitIndex, bwUnit, command);
        else
          executeUnitBurrow(bwUnitIndex, bwUnit, command);
        break;
      case BW::TechIDs::ArchonWarp:
        BW::issueCommand(BW::Command::MergeArchon());
        break;
      case BW::TechIDs::DarkArchonMeld:
        BW::issueCommand(BW::Command::MergeDarkArchon());
        break;
      default:
        return;
      }
    }
    void executeUnitUseTechPosition(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);

      BW::OrderID attackSubOrder;

      switch (command.tech)
      {
      case BW::TechIDs::DarkSwarm:
        attackSubOrder = BW::OrderIDs::DarkSwarm;
        break;
      case BW::TechIDs::DisruptionWeb:
        attackSubOrder = BW::OrderIDs::CastDisruptionWeb;
        break;
      case BW::TechIDs::EMPShockwave:
        attackSubOrder = BW::OrderIDs::EmpShockwave;
        break;
      case BW::TechIDs::Ensnare:
        attackSubOrder = BW::OrderIDs::Ensnare;
        break;
      case BW::TechIDs::Maelstorm:
        attackSubOrder = BW::OrderIDs::CastMaelstrom;
        break;
      case BW::TechIDs::NuclearStrike:
        attackSubOrder = BW::OrderIDs::NukePaint;
        break;
      case BW::TechIDs::Plague:
        attackSubOrder = BW::OrderIDs::Plague;
        break;
      case BW::TechIDs::PsionicStorm:
        attackSubOrder = BW::OrderIDs::PsiStorm;
        break;
      case BW::TechIDs::Recall:
        attackSubOrder = BW::OrderIDs::Teleport;
        break;
      case BW::TechIDs::ScannerSweep:
        attackSubOrder = BW::OrderIDs::PlaceScanner;
        break;
      case BW::TechIDs::SpiderMines:
        attackSubOrder = BW::OrderIDs::PlaceMine;
        break;
      case BW::TechIDs::StasisField:
        attackSubOrder = BW::OrderIDs::StasisField;
        break;
      default:
        return;
      }

      BW::issueCommand(BW::Command::Attack(command.position, attackSubOrder));
    }
    void executeUnitUseTechUnit(int bwUnitIndex, const BW::Unit& bwUnit, const BWAPI::UnitCommand& command)
    {
      executeSelectOrder(bwUnitIndex);

      BW::OrderID attackSubOrder;

      switch (command.tech)
      {
      case BW::TechIDs::Consume:
        attackSubOrder = BW::OrderIDs::Consume;
        break;
      case BW::TechIDs::DefensiveMatrix:
        attackSubOrder = BW::OrderIDs::DefensiveMatrix;
        break;
      case BW::TechIDs::Feedback:
        attackSubOrder = BW::OrderIDs::CastFeedback;
        break;
      case BW::TechIDs::Hallucination:
        attackSubOrder = BW::OrderIDs::Hallucination1;
        break;
      case BW::TechIDs::Healing:
        attackSubOrder = BW::OrderIDs::MedicHeal1;
        break;
      case BW::TechIDs::Infestation:
        attackSubOrder = BW::OrderIDs::InfestMine2;
        break;
      case BW::TechIDs::Irradiate:
        attackSubOrder = BW::OrderIDs::Irradiate;
        break;
      case BW::TechIDs::Lockdown:
        attackSubOrder = BW::OrderIDs::MagnaPulse;
        break;
      case BW::TechIDs::MindControl:
        attackSubOrder = BW::OrderIDs::CastMindControl;
        break;
      case BW::TechIDs::OpticalFlare:
        attackSubOrder = BW::OrderIDs::CastOpticalFlare;
        break;
      case BW::TechIDs::Parasite:
        attackSubOrder = BW::OrderIDs::CastParasite;
        break;
      case BW::TechIDs::Restoration:
        attackSubOrder = BW::OrderIDs::Restoration;
        break;
      case BW::TechIDs::SpawnBroodlings:
        attackSubOrder = BW::OrderIDs::SummonBroodlings;
        break;
      case BW::TechIDs::YamatoGun:
        attackSubOrder = BW::OrderIDs::FireYamatoGun1;
        break;
      default:
        return;
      }

      BW::issueCommand(BW::Command::Attack(findBwIndexByKnownUnitIndex(command.targetIndex), attackSubOrder));
    }
    void executeUnknownUnitCommand(int bwUnitIndex, const BW::Unit&, const BWAPI::UnitCommand& command)
    {
      throw GeneralException("Executing unknown unit command " + Util::Strings::intToString(command.commandId));
    }
    void executeUnitCommand(const BWAPI::UnitCommand& command)
    {
      typedef void(*EXECUTOR)(int unitId, const BW::Unit& bwUnit, const BWAPI::UnitCommand&);
      static Util::LookupTable<EXECUTOR> executors;
      static bool isExecutorsTableInitialized = false;

      // initialize lookup table
      if(!isExecutorsTableInitialized)
      {
        executors.setDefaultValue(&executeUnknownUnitCommand);
        executors.setValue(UnitCommandTypeIds::AttackPosition,      &executeUnitAttackPosition);
        executors.setValue(UnitCommandTypeIds::AttackUnit,          &executeUnitAttackUnit);
        executors.setValue(UnitCommandTypeIds::RightClickPosition,  &executeUnitRightClickPosition);
        executors.setValue(UnitCommandTypeIds::RightClickUnit,      &executeUnitRightClickUnit);
        executors.setValue(UnitCommandTypeIds::Train,               &executeUnitTrain);
        executors.setValue(UnitCommandTypeIds::Build,               &executeUnitBuild);
        executors.setValue(UnitCommandTypeIds::BuildAddon,          &executeUnitBuildAddon);
        executors.setValue(UnitCommandTypeIds::Research,            &executeUnitResearch);
        executors.setValue(UnitCommandTypeIds::Upgrade,             &executeUnitUpgrade);
        executors.setValue(UnitCommandTypeIds::Stop,                &executeUnitStop);
        executors.setValue(UnitCommandTypeIds::HoldPosition,        &executeUnitHoldPosition);
        executors.setValue(UnitCommandTypeIds::Patrol,              &executeUnitPatrol);
        executors.setValue(UnitCommandTypeIds::Follow,              &executeUnitFollow);
        executors.setValue(UnitCommandTypeIds::SetRallyPosition,    &executeUnitSetRallyPosition);
        executors.setValue(UnitCommandTypeIds::SetRallyUnit,        &executeUnitSetRallyUnit);
        executors.setValue(UnitCommandTypeIds::Repair,              &executeUnitRepair);
        executors.setValue(UnitCommandTypeIds::ReturnCargo,         &executeUnitReturnCargo);
        executors.setValue(UnitCommandTypeIds::Morph,               &executeUnitMorph);
        executors.setValue(UnitCommandTypeIds::Burrow,              &executeUnitBurrow);
        executors.setValue(UnitCommandTypeIds::Unburrow,            &executeUnitUnburrow);
        executors.setValue(UnitCommandTypeIds::Siege,               &executeUnitSiege);
        executors.setValue(UnitCommandTypeIds::Unsiege,             &executeUnitUnsiege);
        executors.setValue(UnitCommandTypeIds::Cloak,               &executeUnitCloak);
        executors.setValue(UnitCommandTypeIds::Decloak,             &executeUnitDecloak);
        executors.setValue(UnitCommandTypeIds::Lift,                &executeUnitLift);
        executors.setValue(UnitCommandTypeIds::Land,                &executeUnitLand);
        executors.setValue(UnitCommandTypeIds::Load,                &executeUnitLoad);
        executors.setValue(UnitCommandTypeIds::Unload,              &executeUnitUnload);
        executors.setValue(UnitCommandTypeIds::UnloadAll,           &executeUnitUnloadAll);
        executors.setValue(UnitCommandTypeIds::UnloadAllPosition,   &executeUnitUnloadAllPosition);
        executors.setValue(UnitCommandTypeIds::CancelConstruction,  &executeUnitCancelConstruction);
        executors.setValue(UnitCommandTypeIds::HaltConstruction,    &executeUnitHaltConstruction);
        executors.setValue(UnitCommandTypeIds::CancelMorph,         &executeUnitCancelMorph);
        executors.setValue(UnitCommandTypeIds::CancelTrain,         &executeUnitCancelTrain);
        executors.setValue(UnitCommandTypeIds::CancelTrainSlot,     &executeUnitCancelTrainSlot);
        executors.setValue(UnitCommandTypeIds::CancelAddon,         &executeUnitCancelAddon);
        executors.setValue(UnitCommandTypeIds::CancelResearch,      &executeUnitCancelResearch);
        executors.setValue(UnitCommandTypeIds::CancelUpgrade,       &executeUnitCancelUpgrade);
        executors.setValue(UnitCommandTypeIds::UseTech,             &executeUnitUseTech);
        executors.setValue(UnitCommandTypeIds::UseTechPosition,     &executeUnitUseTechPosition);
        executors.setValue(UnitCommandTypeIds::UseTechUnit,         &executeUnitUseTechUnit);
        isExecutorsTableInitialized = true;
      }

      // get command simulator function
      EXECUTOR executor = executors.lookUp(command.commandId);

      // find out the unitId.
      int bwUnitIndex = findBwIndexByKnownUnitIndex(command.unitIndex);

      const BW::Unit& bwUnit = BW::BWDATA_UnitNodeTable->unit[bwUnitIndex];

      bool isUnitOwnedBySelf = bwUnit.playerID == BW::selfPlayerId;
      if(!isUnitOwnedBySelf)
        return;

      executor(bwUnitIndex, bwUnit, command);
    }
    //---------------------------------------- -----------------------------------------------------------------
  }
};
