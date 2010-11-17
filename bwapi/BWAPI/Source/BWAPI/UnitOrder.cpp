#include "UnitImpl.h"

#include <math.h>
#include <limits>
#include <sstream>

#include <Util/Logger.h>
#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include "BWAPI/GameImpl.h"
#include <BWAPI/WeaponType.h>
#include "Command.h"
#include "DLLMain.h"

#include <BW/UnitType.h>
#include <BW/Unit.h>
#include <BW/Offsets.h>

namespace BWAPI
{
  //-------------------------------------------- ORDER Issue Command -----------------------------------------
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    bool success = false;
    //call the appropriate command function based on the command type
    if      (command.type == UnitCommandTypes::Attack_Move)
      success = attackMove(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Attack_Unit)
      success = attackUnit(command.target);
    else if (command.type == UnitCommandTypes::Build)
      success = build(TilePosition(command.x,command.y),UnitType(command.extra));
    else if (command.type == UnitCommandTypes::Build_Addon)
      success = buildAddon(UnitType(command.extra));
    else if (command.type == UnitCommandTypes::Train)
      success = train(UnitType(command.extra));
    else if (command.type == UnitCommandTypes::Morph)
      success = morph(UnitType(command.extra));
    else if (command.type == UnitCommandTypes::Research)
      success = research(TechType(command.extra));
    else if (command.type == UnitCommandTypes::Upgrade)
      success = upgrade(UpgradeType(command.extra));
    else if (command.type == UnitCommandTypes::Set_Rally_Position)
      success = setRallyPoint(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
      success = setRallyPoint(command.target);
    else if (command.type == UnitCommandTypes::Move)
      success = move(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Patrol)
      success = patrol(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Hold_Position)
      success = holdPosition();
    else if (command.type == UnitCommandTypes::Stop)
      success = stop();
    else if (command.type == UnitCommandTypes::Follow)
      success = follow(command.target);
    else if (command.type == UnitCommandTypes::Gather)
      success = gather(command.target);
    else if (command.type == UnitCommandTypes::Return_Cargo)
      success = returnCargo();
    else if (command.type == UnitCommandTypes::Repair)
      success = repair(command.target);
    else if (command.type == UnitCommandTypes::Burrow)
      success = burrow();
    else if (command.type == UnitCommandTypes::Unburrow)
      success = unburrow();
    else if (command.type == UnitCommandTypes::Cloak)
      success = cloak();
    else if (command.type == UnitCommandTypes::Decloak)
      success = decloak();
    else if (command.type == UnitCommandTypes::Siege)
      success = siege();
    else if (command.type == UnitCommandTypes::Unsiege)
      success = unsiege();
    else if (command.type == UnitCommandTypes::Lift)
      success = lift();
    else if (command.type == UnitCommandTypes::Land)
      success = land(TilePosition(command.x,command.y));
    else if (command.type == UnitCommandTypes::Load)
      success = load(command.target);
    else if (command.type == UnitCommandTypes::Unload)
      success = unload(command.target);
    else if (command.type == UnitCommandTypes::Unload_All)
      success = unloadAll();
    else if (command.type == UnitCommandTypes::Unload_All_Position)
      success = unloadAll(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Right_Click_Position)
      success = rightClick(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
      success = rightClick(command.target);
    else if (command.type == UnitCommandTypes::Halt_Construction)
      success = haltConstruction();
    else if (command.type == UnitCommandTypes::Cancel_Construction)
      success = cancelConstruction();
    else if (command.type == UnitCommandTypes::Cancel_Addon)
      success = cancelAddon();
    else if (command.type == UnitCommandTypes::Cancel_Train)
      success = cancelTrain();
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
      success = cancelTrain(command.extra);
    else if (command.type == UnitCommandTypes::Cancel_Morph)
      success = cancelMorph();
    else if (command.type == UnitCommandTypes::Cancel_Research)
      success = cancelResearch();
    else if (command.type == UnitCommandTypes::Cancel_Upgrade)
      success = cancelUpgrade();
    else if (command.type == UnitCommandTypes::Use_Tech)
      success = useTech(TechType(command.extra));
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
      success = useTech(TechType(command.extra),Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
      success = useTech(TechType(command.extra),command.target);
    else
      BroodwarImpl.setLastError(Errors::Unknown);
    if ( success )
      this->lastOrderFrame = BroodwarImpl.frameCount;
    return success;
  }
  //--------------------------------------------- EXECUTE COMMAND --------------------------------------------
  void UnitImpl::executeCommand(UnitCommand command)
  {
    this->lastOrderFrame = BroodwarImpl.frameCount;
    bool addedCommandToBuffer = false;

    if      (command.type == UnitCommandTypes::Attack_Move)
    {
      Position target(command.x,command.y);
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::AttackMove), sizeof(BW::Orders::Attack));
     }
    else if (command.type == UnitCommandTypes::Attack_Unit)
    {
      Unit* target = command.target;
      switch ( getType().getID() )
      {
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CarrierAttack1), sizeof(BW::Orders::Attack));
        break;
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Warbringer:
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::ReaverAttack1), sizeof(BW::Orders::Attack));
        break;
      default:
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Attack1), sizeof(BW::Orders::Attack));
        break;
      }
    }
    else if (command.type == UnitCommandTypes::Build)
    {
      TilePosition target(command.x,command.y);
      UnitType type1(command.extra);
      BW::UnitType type((u16)command.extra);
      if ( this->getType() == BWAPI::UnitTypes::Zerg_Nydus_Canal && type == BW::UnitID::Zerg_NydusCanal )
        QueueGameCommand((PBYTE)&BW::Orders::MakeNydusExit(BW::TilePosition((u16)target.x(), (u16)target.y())), sizeof(BW::Orders::MakeNydusExit));
      else if (!type.isAddon())
        QueueGameCommand((PBYTE)&BW::Orders::MakeBuilding(BW::TilePosition((u16)target.x(), (u16)target.y()), type), sizeof(BW::Orders::MakeBuilding));
      else
        QueueGameCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)target.x(), (u16)target.y()), type), sizeof(BW::Orders::MakeAddon));
    }
    else if (command.type == UnitCommandTypes::Build_Addon)
    {
      TilePosition target(getTilePosition().x() + 4, getTilePosition().y() + 1);
      UnitType type1(command.extra);
      target.makeValid();
      QueueGameCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)target.x(), (u16)target.y()), (u16)type1.getID()), sizeof(BW::Orders::MakeAddon));
    }
    else if (command.type == UnitCommandTypes::Train)
    {
      UnitType type1(command.extra);
      BW::UnitType type((u16)type1.getID());
      switch ( this->_getType.getID() )
      {
      case BW::UnitID::Zerg_Larva:
      case BW::UnitID::Zerg_Mutalisk:
      case BW::UnitID::Zerg_Hydralisk:
        QueueGameCommand((PBYTE)&BW::Orders::UnitMorph(type), 3);
        break;
      case BW::UnitID::Zerg_Hatchery:
      case BW::UnitID::Zerg_Lair:
      case BW::UnitID::Zerg_Spire:
      case BW::UnitID::Zerg_CreepColony:
        QueueGameCommand((PBYTE)&BW::Orders::BuildingMorph(type), 3);
        break;
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Warbringer:
        QueueGameCommand((PBYTE)&BW::Orders::TrainFighter(), 1);
        break;
      default:
        QueueGameCommand((PBYTE)&BW::Orders::TrainUnit(type), 3);
        break;
      }
    }
    else if (command.type == UnitCommandTypes::Morph)
    {
      UnitType type(command.extra);
      if(type.isBuilding())
        QueueGameCommand((PBYTE)&BW::Orders::BuildingMorph((u16)type.getID()), sizeof(BW::Orders::BuildingMorph));
      else
        QueueGameCommand((PBYTE)&BW::Orders::UnitMorph((u16)type.getID()), sizeof(BW::Orders::UnitMorph));
    }
    else if (command.type == UnitCommandTypes::Research)
    {
      TechType tech(command.extra);
      QueueGameCommand((PBYTE)&BW::Orders::Invent(BW::TechType((u8)tech.getID())), sizeof(BW::Orders::Invent));
    }
    else if (command.type == UnitCommandTypes::Upgrade)
    {
      UpgradeType upgrade(command.extra);
      QueueGameCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType((u8)upgrade.getID())), sizeof(BW::Orders::Upgrade));
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Position)
    {
      Position target(command.x,command.y);
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::RallyPointTile), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
    {
      Unit* target = command.target;
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::RallyPointUnit), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Move)
    {
      Position target(command.x,command.y);
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Move), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Patrol)
    {
      Position target(command.x,command.y);
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Patrol), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Hold_Position)
    {
      //this->orderSelect();
      //QueueGameCommand((PBYTE)&BW::Orders::HoldPosition(0), sizeof(BW::Orders::HoldPosition));
      BroodwarImpl.cmdToHold.push_back(this);
    }
    else if (command.type == UnitCommandTypes::Stop)
    {
      switch ( _getType.getID() )
      {
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Warbringer:
        QueueGameCommand((PBYTE)&BW::Orders::ReaverStop(), sizeof(BW::Orders::ReaverStop));
        break;
      case BW::UnitID::Protoss_Carrier:
      case BW::UnitID::Protoss_Hero_Gantrithor:
        QueueGameCommand((PBYTE)&BW::Orders::CarrierStop(), sizeof(BW::Orders::CarrierStop));
        break;
      default:
        //QueueGameCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
        BroodwarImpl.cmdToStop.push_back(this);
        break;
      }
    }
    else if (command.type == UnitCommandTypes::Follow)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::Follow), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Gather)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::Harvest1), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Return_Cargo)
    {
      QueueGameCommand((PBYTE)&BW::Orders::ReturnCargo(0), sizeof(BW::Orders::ReturnCargo));
    }
    else if (command.type == UnitCommandTypes::Repair)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::Repair1), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Burrow)
    {
      //QueueGameCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
      BroodwarImpl.cmdToBurrow.push_back(this);
    }
    else if (command.type == UnitCommandTypes::Unburrow)
    {
      //QueueGameCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
      BroodwarImpl.cmdToUnburrow.push_back(this);
    }
    else if (command.type == UnitCommandTypes::Cloak)
    {   
      //QueueGameCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
      BroodwarImpl.cmdToCloak.push_back(this);
    }
    else if (command.type == UnitCommandTypes::Decloak)
    {
      //QueueGameCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
      BroodwarImpl.cmdToUncloak.push_back(this);
    }
    else if (command.type == UnitCommandTypes::Siege)
    {
      //QueueGameCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
      BroodwarImpl.cmdToSiege.push_back(this);
    }
    else if (command.type == UnitCommandTypes::Unsiege)
    {
      //QueueGameCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
      BroodwarImpl.cmdToUnsiege.push_back(this);
    }
    else if (command.type == UnitCommandTypes::Lift)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));
    }
    else if (command.type == UnitCommandTypes::Land)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Land(BW::TilePosition((u16)command.x, (u16)command.y), BW::UnitType((u16)this->self->type)), sizeof(BW::Orders::Land));
    }
    else if (command.type == UnitCommandTypes::Load)
    {
      BWAPI::UnitType thisType = this->getType();
      if ( thisType == UnitTypes::Terran_Bunker )
      {
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::PickupBunker), sizeof(BW::Orders::Attack));
      }
      else if ( thisType == UnitTypes::Terran_Dropship || 
                thisType == UnitTypes::Protoss_Shuttle || 
                thisType == UnitTypes::Zerg_Overlord   ||
                thisType == UnitTypes::Hero_Yggdrasill )
      {
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, BW::OrderID::PickupTransport), sizeof(BW::Orders::Attack));
      }
      else if ( command.target->getType() == UnitTypes::Terran_Bunker   ||
                command.target->getType() == UnitTypes::Terran_Dropship ||
                command.target->getType() == UnitTypes::Protoss_Shuttle ||
                command.target->getType() == UnitTypes::Zerg_Overlord   ||
                command.target->getType() == UnitTypes::Hero_Yggdrasill )
      {
        QueueGameCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)command.target), sizeof(BW::Orders::RightClick));
      }
    }
    else if (command.type == UnitCommandTypes::Unload)
    {
      QueueGameCommand((PBYTE)&BW::Orders::UnloadUnit((UnitImpl*)command.target), sizeof(BW::Orders::UnloadUnit));
    }
    else if (command.type == UnitCommandTypes::Unload_All)
    {
      if ( this->getType()==UnitTypes::Terran_Bunker)
        QueueGameCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
      else
        QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)this->getPosition().x(), (u16)this->getPosition().y()), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Unload_All_Position)
    {
      if ( this->getType()==UnitTypes::Terran_Bunker)
        QueueGameCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
      else
        QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)command.x, (u16)command.y), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Right_Click_Position)
    {
      QueueGameCommand((PBYTE)&BW::Orders::RightClick(BW::Position((u16)command.x, (u16)command.y)), sizeof(BW::Orders::RightClick));
    }
    else if (command.type == UnitCommandTypes::Right_Click_Unit)
    {
      QueueGameCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)command.target), sizeof(BW::Orders::RightClick));
    }
    else if (command.type == UnitCommandTypes::Halt_Construction)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
    }
    else if (command.type == UnitCommandTypes::Cancel_Construction)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
    }
    else if (command.type == UnitCommandTypes::Cancel_Addon)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelAddon(), sizeof(BW::Orders::CancelAddon));
    }
    else if (command.type == UnitCommandTypes::Cancel_Train)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelTrain((s8)command.extra), sizeof(BW::Orders::CancelTrain));
    }
    else if (command.type == UnitCommandTypes::Cancel_Train_Slot)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelTrain((s8)command.extra), sizeof(BW::Orders::CancelTrain));
    }
    else if (command.type == UnitCommandTypes::Cancel_Morph)
    {
      if (this->getType().isBuilding())
        QueueGameCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
      else
        QueueGameCommand((PBYTE)&BW::Orders::CancelUnitMorph(), sizeof(BW::Orders::CancelUnitMorph));
    }
    else if (command.type == UnitCommandTypes::Cancel_Research)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelResearch(), sizeof(BW::Orders::CancelResearch));
    }
    else if (command.type == UnitCommandTypes::Cancel_Upgrade)
    {
      QueueGameCommand((PBYTE)&BW::Orders::CancelUpgrade(), sizeof(BW::Orders::CancelUpgrade));
    }
    else if (command.type == UnitCommandTypes::Use_Tech)
    {
      TechType tech(command.extra);
      switch (tech.getID())
      {
        case BW::TechID::Stimpacks:
          QueueGameCommand((PBYTE)&BW::Orders::UseStimPack(), sizeof(BW::Orders::UseStimPack));
          BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech)));
          break;
        case BW::TechID::TankSiegeMode:
          if (this->isSieged())
          {
            BroodwarImpl.cmdToUnsiege.push_back(this);
            BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unsiege(this)));
          }
          else
          {
            BroodwarImpl.cmdToSiege.push_back(this);
            BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::siege(this)));
          }
          break;
        case BW::TechID::PersonnelCloaking:
        case BW::TechID::CloakingField:
          if(this->isCloaked())
          {
            BroodwarImpl.cmdToUncloak.push_back(this);
            BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::decloak(this)));
          }
          else
          {
            BroodwarImpl.cmdToCloak.push_back(this);
            BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cloak(this)));
          }
          break;
        case BW::TechID::Burrowing:
          if(this->isBurrowed())
          {
            BroodwarImpl.cmdToUnburrow.push_back(this);
            BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unburrow(this)));
          }
          else
          {

            BroodwarImpl.cmdToBurrow.push_back(this);
            BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::burrow(this)));
          }
          break;
      }
      addedCommandToBuffer = true;
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Position)
    {
      TechType tech(command.extra);
      Position position(command.x,command.y);
      u8 order = BW::OrderID::None;
      switch (tech.getID())
      {
        case BW::TechID::DarkSwarm:
          order = BW::OrderID::DarkSwarm;
          break;
        case BW::TechID::DisruptionWeb:
          order = BW::OrderID::CastDisruptionWeb;
          break;
        case BW::TechID::EMPShockwave:
          order = BW::OrderID::EmpShockwave;
          break;
        case BW::TechID::Ensnare:
          order = BW::OrderID::Ensnare;
          break;
        case BW::TechID::NuclearStrike:
          order = BW::OrderID::NukePaint;
          break;
        case BW::TechID::Plague:
          order = BW::OrderID::Plague;
          break;
        case BW::TechID::PsionicStorm:
          order = BW::OrderID::PsiStorm;
          break;
        case BW::TechID::Recall:
          order = BW::OrderID::Teleport;
          break;
        case BW::TechID::ScannerSweep:
          order = BW::OrderID::PlaceScanner;
          break;
        case BW::TechID::SpiderMines:
          order = BW::OrderID::PlaceMine;
          break;
        case BW::TechID::StasisField:
          order = BW::OrderID::StasisField;
          break;
      }
      QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), order), sizeof(BW::Orders::Attack));
    }
    else if (command.type == UnitCommandTypes::Use_Tech_Unit)
    {
      TechType tech(command.extra);
      if (tech==TechTypes::Archon_Warp)
        QueueGameCommand((PBYTE)&BW::Orders::MergeArchon(), sizeof(BW::Orders::MergeArchon));
      else if (tech==TechTypes::Dark_Archon_Meld)
        QueueGameCommand((PBYTE)&BW::Orders::MergeDarkArchon(), sizeof(BW::Orders::MergeDarkArchon));
      else
      {
        u8 order;
        switch (tech.getID())
        {
          case BW::TechID::Consume:
            order = BW::OrderID::Consume;
            break;
          case BW::TechID::DefensiveMatrix:
            order = BW::OrderID::DefensiveMatrix;
            break;
          case BW::TechID::Feedback:
            order = BW::OrderID::CastFeedback;
            break;
          case BW::TechID::Hallucination:
            order = BW::OrderID::Hallucination1;
            break;
          case BW::TechID::Healing:
            order = BW::OrderID::MedicHeal1;
            break;
          case BW::TechID::Infestation:
            order = BW::OrderID::InfestMine2;
            break;
          case BW::TechID::Irradiate:
            order = BW::OrderID::Irradiate;
            break;
          case BW::TechID::Lockdown:
            order = BW::OrderID::MagnaPulse;
            break;
          case BW::TechID::Maelstorm:
            order = BW::OrderID::CastMaelstrom;
            break;
          case BW::TechID::MindControl:
            order = BW::OrderID::CastMindControl;
            break;
          case BW::TechID::OpticalFlare:
            order = BW::OrderID::CastOpticalFlare;
            break;
          case BW::TechID::Parasite:
            order = BW::OrderID::CastParasite;
            break;
          case BW::TechID::Restoration:
            order = BW::OrderID::Restoration;
            break;
          case BW::TechID::SpawnBroodlings:
            order = BW::OrderID::SummonBroodlings;
            break;
          case BW::TechID::YamatoGun:
            order = BW::OrderID::FireYamatoGun1;
            break;
          default:
            order = BW::OrderID::None;
        }
        QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)command.target, order), sizeof(BW::Orders::Attack));
      }
    }
    else
    {
    }
    if (addedCommandToBuffer == false)
    {
      BroodwarImpl.addToCommandBuffer(new Command(command));
    }
  }
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool UnitImpl::attackMove(Position target)
  {
    if ( !canIssueCommand( UnitCommand::attackMove(this, target) ) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::attackMove(this, target) );
    return true;
  }
  //--------------------------------------------- ATTACK UNIT ------------------------------------------------
  bool UnitImpl::attackUnit(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::attackUnit(this, target) ) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::attackUnit(this, target) );
    return true;
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool UnitImpl::build(TilePosition target, UnitType type1)
  {
    if ( !canIssueCommand( UnitCommand::build(this, target, type1)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::build(this, target, type1) );
    return true;
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool UnitImpl::buildAddon(UnitType type1)
  {
    if ( !canIssueCommand( UnitCommand::buildAddon(this,type1) ) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::buildAddon(this,type1) );
    return true;
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool UnitImpl::train(UnitType type)
  {
    UnitImpl* thisUnit = this;
    if (getType().producesLarva() && type.whatBuilds().first == UnitTypes::Zerg_Larva )
    {
      if (getLarva().empty())
      {
        Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        return false;
      }
      UnitImpl *larva = (UnitImpl*)(*connectedUnits.begin());
      thisUnit = larva;
    }

    if ( !canIssueCommand( UnitCommand::train(thisUnit,type) ) )
      return false;

    thisUnit->orderSelect();
    executeCommand( UnitCommand::train(thisUnit,type) );
    return true;
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    UnitImpl* thisUnit = this;
    if (getType().producesLarva() && type.whatBuilds().first == UnitTypes::Zerg_Larva )
    {
      if (getLarva().empty())
      {
        Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
        return false;
      }
      UnitImpl *larva = (UnitImpl*)(*connectedUnits.begin());
      thisUnit = larva;
    }

    if ( !canIssueCommand( UnitCommand::morph(thisUnit,type)) )
      return false;

    thisUnit->orderSelect();
    executeCommand( UnitCommand::morph(thisUnit,type) );
    return true;
  }
  //--------------------------------------------- RESEARCH ---------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    if ( !canIssueCommand( UnitCommand::research(this,tech)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::research(this,tech) );
    return true;
  }
  //--------------------------------------------- UPGRADE ----------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    if ( !canIssueCommand( UnitCommand::upgrade(this,upgrade)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::upgrade(this,upgrade) );
    return true;
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool UnitImpl::setRallyPoint(Position target)
  {
    if ( !canIssueCommand( UnitCommand::setRallyPosition(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::setRallyPosition(this,target) );
    return true;
  }
  //--------------------------------------------- SET RALLY UNIT ---------------------------------------------
  bool UnitImpl::setRallyPoint(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::setRallyUnit(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::setRallyUnit(this,target) );
    return true;
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool UnitImpl::move(Position target)
  {
    if ( !canIssueCommand( UnitCommand::move(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::move(this,target) );
    return true;
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool UnitImpl::patrol(Position target)
  {
    if ( !canIssueCommand( UnitCommand::patrol(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::patrol(this,target) );
    return true;
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool UnitImpl::holdPosition()
  {
    if ( !canIssueCommand( UnitCommand::holdPosition(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::holdPosition(this) );
    return true;
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool UnitImpl::stop()
  {
    if ( !canIssueCommand( UnitCommand::stop(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::stop(this) );
    return true;
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool UnitImpl::follow(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::follow(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::follow(this,target) );
    return true;
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool UnitImpl::gather(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::gather(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::gather(this,target) );
    return true;
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool UnitImpl::returnCargo()
  {
    if ( !canIssueCommand( UnitCommand::returnCargo(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::returnCargo(this) );
    return true;
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::repair(this, target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::repair(this,target) );
    return true;
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool UnitImpl::burrow()
  {
    if ( !canIssueCommand( UnitCommand::burrow(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::burrow(this) );
    return true;
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool UnitImpl::unburrow()
  {
    if ( !canIssueCommand( UnitCommand::unburrow(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::unburrow(this) );
    return true;
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool UnitImpl::cloak()
  {
    if ( !canIssueCommand( UnitCommand::cloak(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::cloak(this) );
    return true;
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool UnitImpl::decloak()
  {
    if ( !canIssueCommand( UnitCommand::decloak(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::decloak(this) );
    return true;
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool UnitImpl::siege()
  {
    if ( !canIssueCommand( UnitCommand::siege(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::siege(this) );
    return true;
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool UnitImpl::unsiege()
  {
    if ( !canIssueCommand( UnitCommand::unsiege(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::unsiege(this) );
    return true;
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool UnitImpl::lift()
  {
    if ( !canIssueCommand( UnitCommand::lift(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::lift(this) );
    return true;
  }
  //--------------------------------------------- LAND -------------------------------------------------------
  bool UnitImpl::land(TilePosition target)
  {
    if ( !canIssueCommand( UnitCommand::land(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::land(this,target) );
    return true;
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::load(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::load(this,target) );
    return true;
  }
  //--------------------------------------------- UNLOAD -----------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::unload(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::unload(this,target) );
    return true;
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll()
  {
    if ( !canIssueCommand( UnitCommand::unloadAll(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::unloadAll(this) );
    return true;
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(Position target)
  {
    if ( !canIssueCommand( UnitCommand::unloadAll(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::unloadAll(this,target) );
    return true;
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Position target)
  {
    if ( !canIssueCommand( UnitCommand::rightClick(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::rightClick(this,target) );
    return true;
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::rightClick(this,target)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::rightClick(this,target) );
    return true;
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    if ( !canIssueCommand( UnitCommand::haltConstruction(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::haltConstruction(this) );
    return true;
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    if ( !canIssueCommand( UnitCommand::cancelConstruction(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::cancelConstruction(this) );
    return true;
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    if ( !canIssueCommand( UnitCommand::cancelAddon(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::cancelAddon(this) );
    return true;
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    if ( !canIssueCommand( UnitCommand::cancelTrain(this,slot) ) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::cancelTrain(this, slot) );
    return true;
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    if ( !canIssueCommand( UnitCommand::cancelMorph(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::cancelMorph(this) );
    return true;
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    if ( !canIssueCommand( UnitCommand::cancelResearch(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::cancelResearch(this) );
    return true;
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    if ( !canIssueCommand( UnitCommand::cancelUpgrade(this)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::cancelUpgrade(this) );
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    if ( !canIssueCommand( UnitCommand::useTech(this,tech)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::useTech(this,tech) );
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position position)
  {
    if ( !canIssueCommand( UnitCommand::useTech(this,tech,position)) )
      return false;

    this->orderSelect();
    executeCommand( UnitCommand::useTech(this,tech,position) );
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::useTech(this,tech,target)) )
      return false;

    if (tech==TechTypes::Archon_Warp || tech==TechTypes::Dark_Archon_Meld)
    {
      BW::Orders::Select sel = BW::Orders::Select(2, this, (UnitImpl*)target);
      QueueGameCommand((PBYTE)&sel, sel.size);
    }
    else
      this->orderSelect();

    executeCommand( UnitCommand::useTech(this,tech,target) );
    return true;
  }
};
