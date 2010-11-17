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
    command.unit = this;
    if (command.type == UnitCommandTypes::Train ||
        command.type == UnitCommandTypes::Morph)
    {
      if (getType().producesLarva() && UnitType(command.extra).whatBuilds().first == UnitTypes::Zerg_Larva )
      {
        if (getLarva().empty())
        {
          Broodwar->setLastError(Errors::Unit_Does_Not_Exist);
          return false;
        }
        command.unit = (UnitImpl*)(*getLarva().begin());
      }
    }

    if ( !canIssueCommand( command ) )
      return false;

    if (command.type == UnitCommandTypes::Use_Tech_Unit &&
       (command.extra==TechTypes::Archon_Warp.getID() || command.extra==TechTypes::Dark_Archon_Meld.getID()))
    {
      //select both units for archon warp or dark archon meld
      BW::Orders::Select sel = BW::Orders::Select(2, command.unit, (UnitImpl*)command.target);
      QueueGameCommand((PBYTE)&sel, sel.size);
    }
    else
      command.unit->orderSelect();

    executeCommand( command );
    return true;
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
    return issueCommand( UnitCommand::attackMove(this, target) );
  }
  //--------------------------------------------- ATTACK UNIT ------------------------------------------------
  bool UnitImpl::attackUnit(Unit* target)
  {
    return issueCommand( UnitCommand::attackUnit(this, target) );
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool UnitImpl::build(TilePosition target, UnitType type)
  {
    return issueCommand( UnitCommand::build(this, target, type) );
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool UnitImpl::buildAddon(UnitType type)
  {
    return issueCommand( UnitCommand::buildAddon(this,type) );
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool UnitImpl::train(UnitType type)
  {
    return issueCommand( UnitCommand::train(this,type) );
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    return issueCommand( UnitCommand::morph(this,type) );
  }
  //--------------------------------------------- RESEARCH ---------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    return issueCommand( UnitCommand::research(this,tech) );
  }
  //--------------------------------------------- UPGRADE ----------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    return issueCommand( UnitCommand::upgrade(this,upgrade) );
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool UnitImpl::setRallyPoint(Position target)
  {
    return issueCommand( UnitCommand::setRallyPosition(this,target) );
  }
  //--------------------------------------------- SET RALLY UNIT ---------------------------------------------
  bool UnitImpl::setRallyPoint(Unit* target)
  {
    return issueCommand( UnitCommand::setRallyUnit(this,target) );
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool UnitImpl::move(Position target)
  {
    return issueCommand( UnitCommand::move(this,target) );
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool UnitImpl::patrol(Position target)
  {
    return issueCommand( UnitCommand::patrol(this,target) );
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool UnitImpl::holdPosition()
  {
    return issueCommand( UnitCommand::holdPosition(this) );
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool UnitImpl::stop()
  {
    return issueCommand( UnitCommand::stop(this) );
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool UnitImpl::follow(Unit* target)
  {
    return issueCommand( UnitCommand::follow(this,target) );
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool UnitImpl::gather(Unit* target)
  {
    return issueCommand( UnitCommand::gather(this,target) );
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool UnitImpl::returnCargo()
  {
    return issueCommand( UnitCommand::returnCargo(this) );
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    return issueCommand( UnitCommand::repair(this,target) );
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool UnitImpl::burrow()
  {
    return issueCommand( UnitCommand::burrow(this) );
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool UnitImpl::unburrow()
  {
    return issueCommand( UnitCommand::unburrow(this) );
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool UnitImpl::cloak()
  {
    return issueCommand( UnitCommand::cloak(this) );
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool UnitImpl::decloak()
  {
    return issueCommand( UnitCommand::decloak(this) );
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool UnitImpl::siege()
  {
    return issueCommand( UnitCommand::siege(this) );
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool UnitImpl::unsiege()
  {
    return issueCommand( UnitCommand::unsiege(this) );
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool UnitImpl::lift()
  {
    return issueCommand( UnitCommand::lift(this) );
  }
  //--------------------------------------------- LAND -------------------------------------------------------
  bool UnitImpl::land(TilePosition target)
  {
    return issueCommand( UnitCommand::land(this,target) );
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    return issueCommand( UnitCommand::load(this,target) );
  }
  //--------------------------------------------- UNLOAD -----------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    return issueCommand( UnitCommand::unload(this,target) );
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll()
  {
    return issueCommand( UnitCommand::unloadAll(this) );
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(Position target)
  {
    return issueCommand( UnitCommand::unloadAll(this,target) );
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Position target)
  {
    return issueCommand( UnitCommand::rightClick(this,target) );
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Unit* target)
  {
    return issueCommand( UnitCommand::rightClick(this,target) );
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    return issueCommand( UnitCommand::haltConstruction(this) );
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    return issueCommand( UnitCommand::cancelConstruction(this) );
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    return issueCommand( UnitCommand::cancelAddon(this) );
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    return issueCommand( UnitCommand::cancelTrain(this, slot) );
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    return issueCommand( UnitCommand::cancelMorph(this) );
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    return issueCommand( UnitCommand::cancelResearch(this) );
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    return issueCommand( UnitCommand::cancelUpgrade(this) );
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    return issueCommand( UnitCommand::useTech(this,tech) );
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position position)
  {
    return issueCommand( UnitCommand::useTech(this,tech,position) );
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    return issueCommand( UnitCommand::useTech(this,tech,target) );
  }
};
