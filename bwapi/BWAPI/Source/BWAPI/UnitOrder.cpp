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
      success = setRallyPosition(Position(command.x,command.y));
    else if (command.type == UnitCommandTypes::Set_Rally_Unit)
      success = setRallyUnit(command.target);
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
  //--------------------------------------------- ATTACK MOVE ------------------------------------------------
  bool UnitImpl::attackMove(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::AttackMove), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::attackMove(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- ATTACK UNIT ------------------------------------------------
  bool UnitImpl::attackUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    WeaponType weapon = this->getType().groundWeapon();
    bool targetInAir = (target->isLifted() || target->getType().isFlyer());
    if (targetInAir)
      weapon=this->getType().airWeapon();

    bool canAttack = (weapon!=WeaponTypes::None);
    if (getType() == UnitTypes::Protoss_Reaver && getScarabCount()>0 && !targetInAir)
      canAttack = true;
    if (getType() == UnitTypes::Protoss_Carrier && getInterceptorCount() > 0)
      canAttack = true;
    if (!canAttack)
    {
      BroodwarImpl.setLastError(Errors::Unable_To_Hit);
      return false;
    }
    if (!this->getType().canMove())
    {
      if (this->getDistance(target)>weapon.maxRange() ||
          this->getDistance(target)<weapon.minRange())
      {
        BroodwarImpl.setLastError(Errors::Out_Of_Range);
        return false;
      }
    }
    this->orderSelect();
    switch ( getType().getID() )
    {
    case BW::UnitID::Protoss_Carrier:
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::CarrierAttack1), sizeof(BW::Orders::Attack));
      break;
    case BW::UnitID::Protoss_Reaver:
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::ReaverAttack1), sizeof(BW::Orders::Attack));
      break;
    default:
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Attack1), sizeof(BW::Orders::Attack));
      break;
    }
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::attackUnit(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- BUILD ------------------------------------------------------
  bool UnitImpl::build(TilePosition target, UnitType type1)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->canMake(this,type1))
      return false;

    if (!type1.isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->isConstructing() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    if (!type1.isAddon() && !Broodwar->canBuildHere(this,target,type1))
      return false;

    BW::UnitType type((u16)type1.getID());
    this->orderSelect();
    if ( this->getType() == BWAPI::UnitTypes::Zerg_Nydus_Canal && type == BW::UnitID::Zerg_NydusCanal )
      QueueGameCommand((PBYTE)&BW::Orders::MakeNydusExit(BW::TilePosition((u16)target.x(), (u16)target.y())), sizeof(BW::Orders::MakeNydusExit));
    else if (!type.isAddon())
      QueueGameCommand((PBYTE)&BW::Orders::MakeBuilding(BW::TilePosition((u16)target.x(), (u16)target.y()), type), sizeof(BW::Orders::MakeBuilding));
    else
      QueueGameCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)target.x(), (u16)target.y()), type), sizeof(BW::Orders::MakeAddon));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::build(this,target,type1)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //---------------------------------------------- MAKE UNIT -------------------------------------------------
  bool UnitImpl::make(UnitType type1)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !Broodwar->canMake(this,type1) )
      return false;

    if ( this->isConstructing() || !this->isCompleted() )
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    UnitType thisType = this->getType();
    BW::UnitType maketype((u16)type1.getID());

    if ( !type1.isBuilding() ) // UNITS
    {
      switch ( this->_getType.getID() )
      {
      case BW::UnitID::Zerg_Larva:      // MAKE ZERG UNIT NORMAL
      case BW::UnitID::Zerg_Mutalisk:
      case BW::UnitID::Zerg_Hydralisk:
        this->orderSelect();
        QueueGameCommand((PBYTE)&BW::Orders::UnitMorph(maketype), 3);
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::morph(this,type1)));
        this->lastOrderFrame = BroodwarImpl.frameCount;
        return true;
      case BW::UnitID::Zerg_Hatchery:     // MAKE ZERG UNIT FROM HATCHERY
      case BW::UnitID::Zerg_Lair:
      case BW::UnitID::Zerg_Hive:
        if ( type1.whatBuilds().first == UnitTypes::Zerg_Larva && connectedUnits.size() > 0 )
        {
          UnitImpl *larva = (UnitImpl*)(*connectedUnits.begin());
          larva->orderSelect();
          QueueGameCommand((PBYTE)&BW::Orders::UnitMorph(maketype), 3);
          BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::morph(this,type1)));
          this->lastOrderFrame = BroodwarImpl.frameCount;
          return true;
        }
        break;
      case BW::UnitID::Protoss_Carrier:         // MAKE INTERCEPTOR/SCARAB
      case BW::UnitID::Protoss_Hero_Gantrithor:
      case BW::UnitID::Protoss_Reaver:
      case BW::UnitID::Protoss_Hero_Warbringer:
        this->orderSelect();
        QueueGameCommand((PBYTE)&BW::Orders::TrainFighter(), 1);
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::train(this,type1)));
        this->lastOrderFrame = BroodwarImpl.frameCount;
        return true;
      default:                                  // TRAIN UNITS
        if ( this->isLifted() )
        {
          BroodwarImpl.setLastError(Errors::Unit_Busy);
          return false;
        }
        this->orderSelect();
        QueueGameCommand((PBYTE)&BW::Orders::TrainUnit(maketype), 3);
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::train(this,type1)));
        this->lastOrderFrame = BroodwarImpl.frameCount;
        return true;
      } // type switch      
    } // units
    else // BUILDINGS
    {
      if ( type1.isAddon() && type1.whatBuilds().first == thisType )            // MAKE ADDON
      {
        TilePosition target(getTilePosition().x() + 4, getTilePosition().y() + 1);
        target.makeValid();
        this->orderSelect();
        QueueGameCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)target.x(), (u16)target.y()), maketype), sizeof(BW::Orders::MakeAddon));
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::buildAddon(this,type1)));
        this->lastOrderFrame = BroodwarImpl.frameCount;
        return true;
      }
      else if ( type1.getRace() == BWAPI::Races::Zerg )    // BUILDING MORPH
      {
        this->orderSelect();
        QueueGameCommand((PBYTE)&BW::Orders::BuildingMorph(maketype), 3);
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::morph(this,type1)));
        this->lastOrderFrame = BroodwarImpl.frameCount;
        return true;
      }
      else
      {
        BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
        return false;
      }
    } // buildings
    BroodwarImpl.setLastError(Errors::Unknown);
    return false;
  }
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool UnitImpl::buildAddon(UnitType type1)
  {
    return make(type1);
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool UnitImpl::train(UnitType type1)
  {
    return make(type1);
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    return make(type);
  }
  //--------------------------------------------- RESEARCH ---------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->canResearch(this,tech))
      return false;
    if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    u8 techenum = (u8)tech.getID();
    QueueGameCommand((PBYTE)&BW::Orders::Invent(BW::TechType(techenum)), sizeof(BW::Orders::Invent));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::research(this,tech)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- UPGRADE ----------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->canUpgrade(this,upgrade))
      return false;
    if (this->isLifted() || !this->isIdle() || !this->isCompleted())
    {
      BroodwarImpl.setLastError(Errors::Unit_Busy);
      return false;
    }

    this->orderSelect();
    u8 upgradeenum = (u8)upgrade.getID();
    QueueGameCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType(upgradeenum)), sizeof(BW::Orders::Upgrade));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::upgrade(this,upgrade)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool UnitImpl::setRallyPosition(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::RallyPointTile), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::setRallyPosition(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- SET RALLY UNIT ---------------------------------------------
  bool UnitImpl::setRallyUnit(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (!this->getType().canProduce())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::RallyPointUnit), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::setRallyUnit(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool UnitImpl::move(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (this->getType().isBuilding() && !isLifted())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Move), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::move(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool UnitImpl::patrol(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (this->getType().isBuilding() && !isLifted())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Patrol), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::patrol(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool UnitImpl::holdPosition()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::HoldPosition(0), sizeof(BW::Orders::HoldPosition));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::holdPosition(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool UnitImpl::stop()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    this->orderSelect();
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
      QueueGameCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
      break;
    }
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::stop(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- FOLLOW -----------------------------------------------------
  bool UnitImpl::follow(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Follow), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::follow(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool UnitImpl::gather(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (!this->getType().isWorker() || (!target->getType().isRefinery() && target->getType() != UnitTypes::Resource_Mineral_Field))
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Harvest1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::gather(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool UnitImpl::returnCargo()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().isWorker())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!this->isCarryingGas() && !this->isCarryingMinerals())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Ammo);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::ReturnCargo(0), sizeof(BW::Orders::ReturnCargo));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::returnCargo(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (this->getType() != UnitTypes::Terran_SCV || target->getType().isOrganic())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Repair1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::repair(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool UnitImpl::burrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().isBurrowable())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->getType() != UnitTypes::Zerg_Lurker && !Broodwar->self()->hasResearched(TechTypes::Burrowing))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }

    if(!this->isBurrowed())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::burrow(this)));
    }
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool UnitImpl::unburrow()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    if (!this->getType().isBurrowable())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }

    if(this->isBurrowed())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unburrow(this)));
    }
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool UnitImpl::cloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    BWAPI::TechType tech;
    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_Wraith:
    case BW::UnitID::Terran_Hero_TomKazansky:
      tech = TechTypes::Cloaking_Field;
      break;
    case BW::UnitID::Terran_Ghost:
    case BW::UnitID::Terran_Hero_AlexeiStukov:
    case BW::UnitID::Terran_Hero_SamirDuran:
    case BW::UnitID::Terran_Hero_SarahKerrigan:
    case BW::UnitID::Zerg_Hero_InfestedDuran:
    case BW::UnitID::Zerg_Hero_InfestedKerrigan:
      tech = TechTypes::Personnel_Cloaking;
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }

    if ( !Broodwar->self()->hasResearched(tech) )
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if ( this->getEnergy() < tech.energyUsed() )
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    if(!this->isCloaked())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cloak(this)));
    }
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool UnitImpl::decloak()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_Wraith:
    case BW::UnitID::Terran_Hero_TomKazansky:
    case BW::UnitID::Terran_Ghost:
    case BW::UnitID::Terran_Hero_AlexeiStukov:
    case BW::UnitID::Terran_Hero_SamirDuran:
    case BW::UnitID::Terran_Hero_SarahKerrigan:
    case BW::UnitID::Zerg_Hero_InfestedDuran:
    case BW::UnitID::Zerg_Hero_InfestedKerrigan:
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if ( this->isCloaked() )
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::decloak(this)));
    }
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool UnitImpl::siege()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_SiegeTankTankMode:
    case BW::UnitID::Terran_Hero_EdmundDukeT:
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (!Broodwar->self()->hasResearched(TechTypes::Tank_Siege_Mode))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if ( !this->isSieged() )
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::siege(this)));
    }
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool UnitImpl::unsiege()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_SiegeTankSiegeMode:
    case BW::UnitID::Terran_Hero_EdmundDukeS:
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if (this->isSieged())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unsiege(this)));
    }
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool UnitImpl::lift()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(!this->isLifted())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::lift(this)));
    }
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- LAND -------------------------------------------------------
  bool UnitImpl::land(TilePosition target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!this->getType().isFlyingBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    if(this->isLifted())
    {
      this->orderSelect();
      QueueGameCommand((PBYTE)&BW::Orders::Land(BW::TilePosition((u16)target.x(), (u16)target.y()), BW::UnitType((u16)this->self->type)), sizeof(BW::Orders::Land));
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::land(this,target)));
    }
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    this->orderSelect();
    bool loaded = false;
    if (this->getType() == UnitTypes::Terran_Bunker)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::PickupBunker), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (this->getType() == UnitTypes::Terran_Dropship || this->getType() == UnitTypes::Protoss_Shuttle || this->getType() == UnitTypes::Zerg_Overlord)
    {
      QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::PickupTransport), sizeof(BW::Orders::Attack));
      loaded = true;
    }
    else if (target->getType() == UnitTypes::Terran_Bunker || target->getType() == UnitTypes::Terran_Dropship || target->getType() == UnitTypes::Protoss_Shuttle || target->getType() == UnitTypes::Zerg_Overlord)
    {
      this->rightClick(target);
      loaded = true;
    }
    if (loaded)
    {
      BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::load(this,target)));
      this->lastOrderFrame = BroodwarImpl.frameCount;
      return true;
    }
    //if neither this unit nor the target unit is a bunker, dropship, shuttle, or overlord, return false.
    BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
    return false;
  }
  //--------------------------------------------- UNLOAD -----------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::UnloadUnit((UnitImpl*)target), sizeof(BW::Orders::UnloadUnit));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unload(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    switch ( this->getType().getID() )
    {
    case BW::UnitID::Terran_Dropship:
    case BW::UnitID::Protoss_Shuttle:
    case BW::UnitID::Zerg_Overlord:
      return this->unloadAll(this->getPosition());
    case BW::UnitID::Terran_Bunker:
      break;
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unloadAll(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    switch( this->getType().getID() )
    {
    case BW::UnitID::Terran_Dropship:
    case BW::UnitID::Protoss_Shuttle:
    case BW::UnitID::Zerg_Overlord:
      break;
    case BW::UnitID::Terran_Bunker:
      return this->unloadAll();
    default:
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unloadAll(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Position target)
  {
    target.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::RightClick(BW::Position((u16)target.x(), (u16)target.y())), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::rightClick(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !target )
      return false;

    if (!((UnitImpl*)target)->attemptAccess())
      return false;

    if (!target->getPlayer()->isNeutral() && this->getPlayer()->isEnemy(target->getPlayer()))
    {
      WeaponType weapon = this->getType().groundWeapon();
      if (target->isLifted() || target->getType().isFlyer())
        weapon = this->getType().airWeapon();

      if (weapon == WeaponTypes::None)
      {
        BroodwarImpl.setLastError(Errors::Unable_To_Hit);
        return false;
      }
      if (!this->getType().canMove())
      {
        if (this->getDistance(target)>weapon.maxRange() ||
            this->getDistance(target)<weapon.minRange())
        {
          BroodwarImpl.setLastError(Errors::Out_Of_Range);
          return false;
        }
      }
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::RightClick((UnitImpl*)target), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::rightClick(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- HALT CONSTRUCTION ------------------------------------------
  bool UnitImpl::haltConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (this->getOrder() != Orders::ConstructingBuilding)
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Stop(0), sizeof(BW::Orders::Stop));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::haltConstruction(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CANCEL CONSTRUCTION ----------------------------------------
  bool UnitImpl::cancelConstruction()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();

    if (this->isCompleted())
      return false;

    if (!this->getType().isBuilding())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelConstruction(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if ( !getAddon() || getAddon()->isCompleted() )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelAddon(), sizeof(BW::Orders::CancelAddon));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelAddon(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool UnitImpl::cancelTrain()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!isTraining())
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelTrainLast(), sizeof(BW::Orders::CancelTrainLast));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelTrain(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CANCEL TRAIN -----------------------------------------------
  bool UnitImpl::cancelTrain(int slot)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!isTraining() || (int)(this->getTrainingQueue().size()) <= slot)
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelTrain((u8)slot), sizeof(BW::Orders::CancelTrain));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelTrain(this,slot)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CANCEL MORPH -----------------------------------------------
  bool UnitImpl::cancelMorph()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!isMorphing())
      return false;
    this->orderSelect();
    if (this->getType().isBuilding())
      QueueGameCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
    else
      QueueGameCommand((PBYTE)&BW::Orders::CancelUnitMorph(), sizeof(BW::Orders::CancelUnitMorph));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelMorph(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CANCEL RESEARCH --------------------------------------------
  bool UnitImpl::cancelResearch()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (self->order != BW::OrderID::ResearchTech)
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelResearch(), sizeof(BW::Orders::CancelResearch));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelResearch(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CANCEL UPGRADE ---------------------------------------------
  bool UnitImpl::cancelUpgrade()
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (self->order != BW::OrderID::Upgrade)
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelUpgrade(), sizeof(BW::Orders::CancelUpgrade));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelUpgrade(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->self()->hasResearched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy() < tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    if (tech.whatUses().find(this->getType()) == tech.whatUses().end())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::Stimpacks:
        QueueGameCommand((PBYTE)&BW::Orders::UseStimPack(), sizeof(BW::Orders::UseStimPack));
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech)));
        break;
      case BW::TechID::TankSiegeMode:
        if (this->isSieged())
          this->unsiege();
        else
          this->siege();
        break;
      case BW::TechID::PersonnelCloaking:
      case BW::TechID::CloakingField:
        if(this->isCloaked())
          this->decloak();
        else
          this->cloak();
        break;
      case BW::TechID::Burrowing:
        if(this->isBurrowed())
          this->unburrow();
        else
          this->burrow();
        break;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position position)
  {
    position.makeValid();
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->self()->hasResearched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy() < tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    if (tech.whatUses().find(this->getType()) == tech.whatUses().end())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
    u8 order;
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
        if ( this->getSpiderMineCount() <= 0 )
        {
          BroodwarImpl.setLastError(Errors::Insufficient_Ammo);
          return false;
        }
        order = BW::OrderID::PlaceMine;
        break;
      case BW::TechID::StasisField:
        order = BW::OrderID::StasisField;
        break;
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)position.x(), (u16)position.y()), order), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech,position)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Unit* target)
  {
    BroodwarImpl.setLastError(Errors::None);
    checkAccessBool();
    checkOwnership();
    if (!Broodwar->self()->hasResearched(tech))
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Tech);
      return false;
    }
    if (this->getEnergy() < tech.energyUsed())
    {
      BroodwarImpl.setLastError(Errors::Insufficient_Energy);
      return false;
    }
    if (tech.whatUses().find(this->getType()) == tech.whatUses().end())
    {
      BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
      return false;
    }
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
      case BW::TechID::ArchonWarp:
        {
          BW::Orders::Select sel = BW::Orders::Select(2, this, (UnitImpl*)target);
          QueueGameCommand((PBYTE)&sel, sel.size);
          QueueGameCommand((PBYTE)&BW::Orders::MergeArchon(), sizeof(BW::Orders::MergeArchon));
          BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech,target)));
          this->lastOrderFrame = BroodwarImpl.frameCount;
          return true;
        }
      case BW::TechID::DarkArchonMeld:
        {
          BW::Orders::Select sel = BW::Orders::Select(2, this, (UnitImpl*)target);
          QueueGameCommand((PBYTE)&sel, sel.size);
          QueueGameCommand((PBYTE)&BW::Orders::MergeDarkArchon(), sizeof(BW::Orders::MergeDarkArchon));
          BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech,target)));
          this->lastOrderFrame = BroodwarImpl.frameCount;
          return true;
        }
      default:
        BroodwarImpl.setLastError(Errors::Incompatible_TechType);
        return false;
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, order), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
};
