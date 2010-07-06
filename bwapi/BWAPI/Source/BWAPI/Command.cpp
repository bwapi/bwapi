#include "Command.h"
#include <BWAPI/Game.h>
#include <BWAPI/Position.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Game.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/TechType.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/UnitCommandType.h>
namespace BWAPI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Command::Command(UnitCommand command) : command(command)
  {
    startFrame = Broodwar->getFrameCount();
    savedExtra = -1;
  }
  int getUnitID(Unit* unit)
  {
    if (unit==NULL) return -1;
    return unit->getID();
  }
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void Command::execute()
  {
    Unit* unit = command.unit;
    Unit* target = command.target;
    Position position(command.x,command.y);
    TilePosition tileposition(command.x,command.y);
    UnitType unitType(command.extra);
    UpgradeType upgradeType(command.extra);
    TechType techType(command.extra);

    if (command.type == UnitCommandTypes::Attack_Position)
    {
      if (!unit->getData()->exists) return;
      if (!unit->getType().canMove()) return;
      unit->getData()->order  = Orders::AttackMove.getID();
      unit->getData()->targetPositionX = position.x();
      unit->getData()->targetPositionY = position.y();
    }
    else if (command.type == UnitCommandTypes::Attack_Unit)
    {
      if (target==NULL || !target->getData()->exists) return;
      if (!unit->getData()->exists) return;
      if (!unit->getType().canAttack()) return;
      unit->getData()->order = Orders::AttackUnit.getID();
      unit->getData()->target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Build)
    {
      if (!unit->getData()->exists) return;
      if (unit->getType() == UnitTypes::Zerg_Drone)
        unit->getData()->order = Orders::DroneLand.getID();
      else if (unit->getType() == UnitTypes::Protoss_Probe)
        unit->getData()->order = Orders::BuildProtoss1.getID();
      else if (unit->getType() == UnitTypes::Terran_SCV)
        unit->getData()->order = Orders::BuildTerran.getID();
      unit->getData()->isConstructing = true;
      unit->getData()->isIdle         = false;
      unit->getData()->buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Build_Addon)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->secondaryOrder = Orders::BuildAddon.getID();
      unit->getData()->isConstructing = true;
      unit->getData()->isIdle         = false;
      unit->getData()->buildType      = command.extra;
    }
    else if (command.type == UnitCommandTypes::Burrow)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Burrow.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Addon)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Nothing.getID();
      unit->getData()->isIdle = true;
      unit->getData()->isConstructing = false;
      unit->getData()->buildType = UnitTypes::None.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Construction)
    {
      if (savedExtra==-1)
        savedExtra = unit->getData()->type;
      unitType = UnitType(savedExtra);

      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Nothing.getID();
    }
    else if (command.type == UnitCommandTypes::Cancel_Morph)
    {
      if (savedExtra==-1)
        savedExtra = unit->getData()->buildType;
      unitType = UnitType(savedExtra);

      Player* p = unit->getPlayer();
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      {
        p->getData()->minerals+=unitType.mineralPrice();
        p->getData()->gas+=unitType.gasPrice();
      }
      if (unitType.isTwoUnitsInOneEgg())
        p->getData()->supplyUsed[Races::Zerg.getID()]-=unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired();
      else
        p->getData()->supplyUsed[Races::Zerg.getID()]-=unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired();

      unit->getData()->buildType = UnitTypes::None.getID();
      unit->getData()->remainingBuildTime = 0;
      unit->getData()->isMorphing = false;
      unit->getData()->isConstructing = false;
      unit->getData()->order = Orders::Nothing.getID();
      unit->getData()->type = unitType.whatBuilds().first.getID();
      unit->getData()->isIdle = true;
    }
    else if (command.type == UnitCommandTypes::Cancel_Research)
    {
      if (savedExtra==-1)
        savedExtra = unit->getData()->tech;
      techType = TechType(savedExtra);

      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Nothing.getID();
      unit->getData()->tech = TechTypes::None.getID();
      unit->getData()->isIdle = true;
      unit->getData()->remainingResearchTime = 0;
      Player* p = unit->getPlayer();
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      {
        p->getData()->minerals += techType.mineralPrice();
        p->getData()->gas      += techType.gasPrice();
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->trainingQueueCount--;
      if (unit->getData()->trainingQueueCount<0)
        unit->getData()->trainingQueueCount=0;
      if (unit->getData()->trainingQueueCount==0)
      {
        unit->getData()->isTraining = false;
        unit->getData()->order = Orders::Nothing.getID();
        unit->getData()->isIdle = true;
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
    {
      if (!unit->getData()->exists) return;
      for(int i=command.extra;i<4;i++)
        unit->getData()->trainingQueue[i]=unit->getData()->trainingQueue[i+1];
      unit->getData()->trainingQueueCount--;
      if (unit->getData()->trainingQueueCount<0)
        unit->getData()->trainingQueueCount=0;
      if (unit->getData()->trainingQueueCount==0)
      {
        unit->getData()->isTraining = false;
        unit->getData()->order = Orders::Nothing.getID();
        unit->getData()->isIdle = true;
      }
    }
    else if (command.type == UnitCommandTypes::Cancel_Upgrade)
    {
      if (savedExtra==-1)
        savedExtra = unit->getData()->upgrade;
      upgradeType = UpgradeType(savedExtra);

      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Nothing.getID();
      int level = unit->getPlayer()->getUpgradeLevel(upgradeType);
      unit->getData()->upgrade = UpgradeTypes::None.getID();
      unit->getData()->isIdle = true;
      unit->getData()->remainingUpgradeTime = 0;
      Player* p = unit->getPlayer();
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      {
        p->getData()->minerals += upgradeType.mineralPriceBase() + upgradeType.mineralPriceFactor()*level;
        p->getData()->gas      += upgradeType.gasPriceBase()     + upgradeType.gasPriceFactor()*level;
      }
    }
    else if (command.type == UnitCommandTypes::Cloak)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Cloak.getID();
      if (unit->getData()->type==UnitTypes::Terran_Ghost.getID())
        unit->getData()->energy -= BWAPI::TechTypes::Personnel_Cloaking.energyUsed();
      else
        unit->getData()->energy -= BWAPI::TechTypes::Cloaking_Field.energyUsed();
    }
    else if (command.type == UnitCommandTypes::Decloak)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Decloak.getID();
    }
    else if (command.type == UnitCommandTypes::Follow)
    {
      if (!unit->getData()->exists) return;
      if (!unit->getType().canMove()) return;
      unit->getData()->order = Orders::Follow.getID();
      unit->getData()->target = getUnitID(target);
      unit->getData()->isIdle = false;
      unit->getData()->isMoving = true;
    }
    else if (command.type == UnitCommandTypes::Halt_Construction)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::PlayerGuard.getID();
      unit->getData()->isConstructing = false;
      unit->getData()->isIdle = true;
    }
    else if (command.type == UnitCommandTypes::Hold_Position)
    {
      if (!unit->getData()->exists) return;
      if (!unit->getType().canMove()) return;
      if (unit->getType()==UnitTypes::Protoss_Carrier)
        unit->getData()->order = Orders::CarrierHoldPosition.getID();
      else if (unit->getType()==UnitTypes::Zerg_Queen)
        unit->getData()->order = Orders::QueenHoldPosition.getID();
      else if (unit->getType()==UnitTypes::Zerg_Infested_Terran || unit->getType()==UnitTypes::Zerg_Scourge)
        unit->getData()->order = Orders::SuicideHoldPosition.getID();
      else if (unit->getType()==UnitTypes::Terran_Medic)
        unit->getData()->order = Orders::MedicHoldPosition.getID();
      else if (unit->getType()==UnitTypes::Protoss_Reaver)
        unit->getData()->order = Orders::ReaverHoldPosition.getID();
      else
        unit->getData()->order = Orders::HoldPosition.getID();
    }
    else if (command.type == UnitCommandTypes::Land)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::BuildingLand.getID();
      unit->getData()->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Lift)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::BuildingLiftOff.getID();
      unit->getData()->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Load)
    {
      if (!unit->getData()->exists) return;
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->getData()->order = Orders::PickupBunker.getID();
        unit->getData()->target = getUnitID(target);
      }
      else if (unit->getType() == UnitTypes::Terran_Dropship
            || unit->getType() == UnitTypes::Protoss_Shuttle
            || unit->getType() == UnitTypes::Zerg_Overlord)
      {
        unit->getData()->order = Orders::PickupTransport.getID();
        unit->getData()->target = getUnitID(target);
      }
      else if (target->getType() == UnitTypes::Terran_Bunker
            || target->getType() == UnitTypes::Terran_Dropship
            || target->getType() == UnitTypes::Protoss_Shuttle
            || target->getType() == UnitTypes::Zerg_Overlord)
      {
        unit->getData()->order = Orders::EnterTransport.getID();
        unit->getData()->target = getUnitID(target);
      }
      unit->getData()->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Morph)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->buildType = unitType.getID();
      unit->getData()->remainingBuildTime = unitType.buildTime();
      unit->getData()->isMorphing = true;
      unit->getData()->isConstructing = true;
      unit->getData()->isIdle = false;
      if (unitType.isBuilding())
      {
        unit->getData()->order = Orders::ZergBuildingMorph.getID();
        Player* p = unit->getPlayer();
        p->getData()->minerals -= unitType.mineralPrice();
        p->getData()->gas      -= unitType.gasPrice();
        p->getData()->allUnitCount[unitType.getID()]++;
      }
      else
      {
        unit->getData()->order = Orders::ZergUnitMorph.getID();
        Player* p = unit->getPlayer();
        if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
        {
          p->getData()->minerals -= unitType.mineralPrice();
          p->getData()->gas      -= unitType.gasPrice();
          p->getData()->allUnitCount[unitType.getID()]++;
        }
        if (unitType.isTwoUnitsInOneEgg())
          p->getData()->supplyUsed[Races::Zerg.getID()] += unitType.supplyRequired()*2-unitType.whatBuilds().first.supplyRequired();
        else
          p->getData()->supplyUsed[Races::Zerg.getID()] += unitType.supplyRequired()-unitType.whatBuilds().first.supplyRequired();

        if (unitType == UnitTypes::Zerg_Lurker)
          unit->getData()->type = UnitTypes::Zerg_Lurker_Egg.getID();
        else if (unitType == UnitTypes::Zerg_Devourer ||
                 unitType == UnitTypes::Zerg_Guardian)
          unit->getData()->type = UnitTypes::Zerg_Cocoon.getID();
        else
          unit->getData()->type = UnitTypes::Zerg_Egg.getID();
      }
    }
    else if (command.type == UnitCommandTypes::Patrol)
    {
      if (!unit->getData()->exists) return;
      if (!unit->getType().canMove()) return;
      unit->getData()->order = Orders::Patrol.getID();
      unit->getData()->isIdle = false;
      unit->getData()->isMoving = true;
      unit->getData()->targetPositionX = position.x();
      unit->getData()->targetPositionY = position.y();
    }
    else if (command.type == UnitCommandTypes::Repair)
    {
      if (!unit->getData()->exists) return;
      if (unit->getType()!=UnitTypes::Terran_SCV) return;
      unit->getData()->order = Orders::Repair1.getID();
      unit->getData()->target = getUnitID(target);
      unit->getData()->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Research)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::ResearchTech.getID();
      unit->getData()->tech = techType.getID();
      unit->getData()->isIdle = false;
      unit->getData()->remainingResearchTime = techType.researchTime();
      Player* p = unit->getPlayer();
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      {
        p->getData()->minerals -= techType.mineralPrice();
        p->getData()->gas      -= techType.gasPrice();
      }
    }
    else if (command.type == UnitCommandTypes::Return_Cargo)
    {
      if (!unit->getData()->exists) return;
      if (unit->getData()->carryResourceType!=1 && unit->getData()->carryResourceType!=2) return;
      if (unit->isCarryingGas())
        unit->getData()->order = Orders::ReturnGas.getID();
      else
        unit->getData()->order = Orders::ReturnMinerals.getID();
      unit->getData()->isIdle = false;
    }
    else if (command.type == UnitCommandTypes::Right_Click_Position)
    {
      if (!unit->getData()->exists) return;
      if (unit->getType().canMove())
        unit->getData()->order = Orders::Move.getID();
    }
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->target = getUnitID(target);
      unit->getData()->isIdle = false;
      unit->getData()->isMoving = true;
      if (unit->getType().isWorker() && target->getType()==UnitTypes::Resource_Mineral_Field)
        unit->getData()->order = Orders::MoveToMinerals.getID();
      else if (unit->getType().isWorker() && target->getType().isRefinery())
        unit->getData()->order = Orders::MoveToGas.getID();
      else if (unit->getType().isWorker() &&
               target->getType().getRace()==Races::Terran &&
               target->getType().whatBuilds().first==unit->getType() &&
               !target->isCompleted())
      {
        unit->getData()->order = Orders::ConstructingBuilding.getID();
        unit->getData()->buildUnit = getUnitID(target);
        target->getData()->buildUnit = getUnitID(unit);
        unit->getData()->isConstructing = true;
        target->getData()->isConstructing = true;
      }
      else if (unit->getType().canAttack() && target->getPlayer() != unit->getPlayer() && !target->getType().isNeutral())
        unit->getData()->order = Orders::AttackUnit.getID();
      else if (unit->getType().canMove())
        unit->getData()->order = Orders::Follow.getID();
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Position)
    {
      if (!unit->getData()->exists) return;
      if (!unit->getType().canProduce()) return;
      unit->getData()->order = Orders::RallyPointTile.getID();
      unit->getData()->rallyPositionX = position.x();
      unit->getData()->rallyPositionY = position.y();
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
    {
      if (!unit->getData()->exists) return;
      if (!unit->getType().canProduce()) return;
      if (target == NULL || !target->getData()->exists) return;
      unit->getData()->order = Orders::RallyPointUnit.getID();
      unit->getData()->rallyUnit = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Siege)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::SiegeMode.getID();
    }
    else if (command.type == UnitCommandTypes::Stop)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Stop.getID();
      unit->getData()->isIdle = true;
    }
    else if (command.type == UnitCommandTypes::Train)
    {
      if (!unit->getData()->exists) return;
      Player* p = unit->getPlayer();
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      {
        unit->getData()->trainingQueue[unit->getData()->trainingQueueCount] = unitType.getID();
        unit->getData()->trainingQueueCount++;
        p->getData()->minerals -= unitType.mineralPrice();
        p->getData()->gas      -= unitType.gasPrice();
        p->getData()->allUnitCount[unitType.getID()]++;
      }
      unit->getData()->remainingTrainTime = unitType.buildTime();
      unit->getData()->isTraining = true;
      unit->getData()->isIdle = false;
      if (unitType == UnitTypes::Terran_Nuclear_Missile.getID())
        unit->getData()->secondaryOrder = Orders::Train.getID();
      p->getData()->supplyUsed[unitType.getRace().getID()] += unitType.supplyRequired();
    }
    else if (command.type == UnitCommandTypes::Unburrow)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Unburrow.getID();
    }
    else if (command.type == UnitCommandTypes::Unload)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Unload.getID();
      unit->getData()->target = getUnitID(target);
    }
    else if (command.type == UnitCommandTypes::Unload_All)
    {
      if (!unit->getData()->exists) return;
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->getData()->order = Orders::Unload.getID();
      }
      else
      {
        unit->getData()->order = Orders::MoveUnload.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
    }
    else if (command.type == UnitCommandTypes::Unload_All_Position)
    {
      if (!unit->getData()->exists) return;
      if (unit->getType() == UnitTypes::Terran_Bunker)
      {
        unit->getData()->order = Orders::Unload.getID();
      }
      else
      {
        unit->getData()->order = Orders::MoveUnload.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
    }
    else if (command.type == UnitCommandTypes::Unsiege)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::TankMode.getID();
    }
    else if (command.type == UnitCommandTypes::Upgrade)
    {
      if (!unit->getData()->exists) return;
      unit->getData()->order = Orders::Upgrade.getID();
      unit->getData()->upgrade = upgradeType.getID();
      unit->getData()->isIdle = false;
      int level = unit->getPlayer()->getUpgradeLevel(upgradeType.getID());
      unit->getData()->remainingUpgradeTime = upgradeType.upgradeTimeBase()+upgradeType.upgradeTimeFactor()*level;
      Player* p = unit->getPlayer();
      if (Broodwar->getFrameCount()-startFrame<Broodwar->getLatency())
      {
        p->getData()->minerals -= upgradeType.mineralPriceBase() + upgradeType.mineralPriceFactor()*level;
        p->getData()->gas      -= upgradeType.gasPriceBase()     + upgradeType.gasPriceFactor()*level;
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech)
    {
      if (!unit->getData()->exists) return;
      if (techType == TechTypes::Stim_Packs)
      {
        unit->getData()->hitPoints -= 10;
        unit->getData()->stimTimer = 17;
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
    {
      if (!unit->getData()->exists) return;
      if (techType==TechTypes::Dark_Swarm)
      {
        unit->getData()->order = Orders::DarkSwarm.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Disruption_Web)
      {
        unit->getData()->order = Orders::CastDisruptionWeb.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::EMP_Shockwave)
      {
        unit->getData()->order = Orders::EmpShockwave.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Ensnare)
      {
        unit->getData()->order = Orders::Ensnare.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Maelstrom)
      {
        unit->getData()->order = Orders::CastMaelstrom.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Nuclear_Strike)
      {
        unit->getData()->order = Orders::NukePaint.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Plague)
      {
        unit->getData()->order = Orders::Plague.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Psionic_Storm)
      {
        unit->getData()->order = Orders::PsiStorm.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Recall)
      {
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Scanner_Sweep)
      {
        unit->getData()->order = Orders::Scanner.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Spider_Mines)
      {
        unit->getData()->order = Orders::PlaceMine.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
      else if (techType==TechTypes::Stasis_Field)
      {
        unit->getData()->order = Orders::StasisField.getID();
        unit->getData()->targetPositionX = position.x();
        unit->getData()->targetPositionY = position.y();
      }
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
    {
      if (!unit->getData()->exists) return;
      if (techType==TechTypes::Consume)
      {
        unit->getData()->order = Orders::Consume.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Defensive_Matrix)
      {
        unit->getData()->order = Orders::DefensiveMatrix.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Feedback)
      {
        unit->getData()->order = Orders::CastFeedback.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Hallucination)
      {
        unit->getData()->order = Orders::Hallucination1.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Healing)
      {
        unit->getData()->order = Orders::HealMove.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Infestation)
      {
        unit->getData()->order = Orders::InfestMine1.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Irradiate)
      {
        unit->getData()->order = Orders::Irradiate.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Lockdown)
      {
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Mind_Control)
      {
        unit->getData()->order = Orders::CastMindControl.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Optical_Flare)
      {
        unit->getData()->order = Orders::CastOpticalFlare.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Parasite)
      {
        unit->getData()->order = Orders::CastParasite.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Restoration)
      {
        unit->getData()->order = Orders::Restoration.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Spawn_Broodlings)
      {
        unit->getData()->order = Orders::SummonBroodlings.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Yamato_Gun)
      {
        unit->getData()->order = Orders::FireYamatoGun1.getID();
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Archon_Warp)
      {
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
      else if (techType==TechTypes::Dark_Archon_Meld)
      {
        unit->getData()->orderTarget = getUnitID(target);
        unit->getData()->targetPositionX = target->getPosition().x();
        unit->getData()->targetPositionY = target->getPosition().y();
      }
    }
  }
};
