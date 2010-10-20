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
    if ( !canIssueCommand( UnitCommand::attackMove(this, target) ) )
      return false;
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
    if ( !target || !((UnitImpl*)target)->attemptAccess() )
      return false;

    WeaponType weapon = this->getType().groundWeapon();
    bool targetInAir = (target->isLifted() || target->getType().isFlyer());
    if (targetInAir)
      weapon = this->getType().airWeapon();

    bool canAttack = (weapon != WeaponTypes::None);
    if ( (getType() == UnitTypes::Protoss_Reaver && getScarabCount() > 0 && !targetInAir) || 
         (getType() == UnitTypes::Protoss_Carrier && getInterceptorCount() > 0) )
      canAttack = true;

    if (!canAttack)
      return BroodwarImpl.setLastError(Errors::Unable_To_Hit);

    if (!this->getType().canMove())
    {
      if (this->getDistance(target) > weapon.maxRange() ||
          this->getDistance(target) < weapon.minRange())
        return BroodwarImpl.setLastError(Errors::Out_Of_Range);
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
    if ( !canIssueCommand( UnitCommand::build(this, target, type1)) )
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
  //--------------------------------------------- BUILD ADDON ------------------------------------------------
  bool UnitImpl::buildAddon(UnitType type1)
  {
    if ( !canIssueCommand( UnitCommand::buildAddon(this,type1) ) )
      return false;

    TilePosition target(getTilePosition().x() + 4, getTilePosition().y() + 1);
    target.makeValid();

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::MakeAddon(BW::TilePosition((u16)target.x(), (u16)target.y()), (u16)type1.getID()), sizeof(BW::Orders::MakeAddon));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::buildAddon(this,type1)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- TRAIN ------------------------------------------------------
  bool UnitImpl::train(UnitType type1)
  {
    if ( !canIssueCommand( UnitCommand::train(this,type1) ) )
      return false;

    if ( this->_getType.producesLarva() && type1.whatBuilds().first == UnitTypes::Zerg_Larva )
    {
      UnitImpl *larva = (UnitImpl*)(*connectedUnits.begin());
      return larva->train(type1);
    }

    BW::UnitType type((u16)type1.getID());
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::train(this,type1)));
    this->orderSelect();
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
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- MORPH ------------------------------------------------------
  bool UnitImpl::morph(UnitType type)
  {
    if ( !canIssueCommand( UnitCommand::morph(this,type)) )
      return false;

    if ( this->_getType.producesLarva() && type.whatBuilds().first == UnitTypes::Zerg_Larva )
    {
      UnitImpl *larva = (UnitImpl*)(*connectedUnits.begin());
      return larva->morph(type);
    }

    this->orderSelect();
    if(type.isBuilding())
      QueueGameCommand((PBYTE)&BW::Orders::BuildingMorph((u16)type.getID()), sizeof(BW::Orders::BuildingMorph));
    else
      QueueGameCommand((PBYTE)&BW::Orders::UnitMorph((u16)type.getID()), sizeof(BW::Orders::UnitMorph));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::morph(this,type)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- RESEARCH ---------------------------------------------------
  bool UnitImpl::research(TechType tech)
  {
    if ( !canIssueCommand( UnitCommand::research(this,tech)) )
      return false;

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Invent(BW::TechType((u8)tech.getID())), sizeof(BW::Orders::Invent));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::research(this,tech)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- UPGRADE ----------------------------------------------------
  bool UnitImpl::upgrade(UpgradeType upgrade)
  {
    if ( !canIssueCommand( UnitCommand::upgrade(this,upgrade)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Upgrade(BW::UpgradeType((u8)upgrade.getID())), sizeof(BW::Orders::Upgrade));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::upgrade(this,upgrade)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- SET RALLY POSITION -----------------------------------------
  bool UnitImpl::setRallyPosition(Position target)
  {
    if ( !canIssueCommand( UnitCommand::setRallyPosition(this,target)) )
      return false;

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::RallyPointTile), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::setRallyPosition(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- SET RALLY UNIT ---------------------------------------------
  bool UnitImpl::setRallyUnit(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::setRallyUnit(this,target)) )
      return false;

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::RallyPointUnit), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::setRallyUnit(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- MOVE -------------------------------------------------------
  bool UnitImpl::move(Position target)
  {
    if ( !canIssueCommand( UnitCommand::move(this,target)) )
      return false;

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Move), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::move(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- PATROL -----------------------------------------------------
  bool UnitImpl::patrol(Position target)
  {
    if ( !canIssueCommand( UnitCommand::patrol(this,target)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::Patrol), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::patrol(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- HOLD POSITION ----------------------------------------------
  bool UnitImpl::holdPosition()
  {
    if ( !canIssueCommand( UnitCommand::holdPosition(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::HoldPosition(0), sizeof(BW::Orders::HoldPosition));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::holdPosition(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- STOP -------------------------------------------------------
  bool UnitImpl::stop()
  {
    if ( !canIssueCommand( UnitCommand::stop(this)) )
      return false;
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
    if ( !canIssueCommand( UnitCommand::follow(this,target)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Follow), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::follow(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- GATHER -----------------------------------------------------
  bool UnitImpl::gather(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::gather(this,target)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Harvest1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::gather(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- RETURN CARGO -----------------------------------------------
  bool UnitImpl::returnCargo()
  {
    if ( !canIssueCommand( UnitCommand::returnCargo(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::ReturnCargo(0), sizeof(BW::Orders::ReturnCargo));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::returnCargo(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- REPAIR -----------------------------------------------------
  bool UnitImpl::repair(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::repair(this, target)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, BW::OrderID::Repair1), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::repair(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- BURROW -----------------------------------------------------
  bool UnitImpl::burrow()
  {
    if ( !canIssueCommand( UnitCommand::burrow(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Burrow(), sizeof(BW::Orders::Burrow));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::burrow(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- UNBURROW ---------------------------------------------------
  bool UnitImpl::unburrow()
  {
    if ( !canIssueCommand( UnitCommand::unburrow(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Unburrow(), sizeof(BW::Orders::Unburrow));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unburrow(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CLOAK ------------------------------------------------------
  bool UnitImpl::cloak()
  {
    if ( !canIssueCommand( UnitCommand::cloak(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Cloak(), sizeof(BW::Orders::Cloak));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cloak(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- DECLOAK ----------------------------------------------------
  bool UnitImpl::decloak()
  {
    if ( !canIssueCommand( UnitCommand::decloak(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Decloak(), sizeof(BW::Orders::Decloak));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::decloak(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- SIEGE ------------------------------------------------------
  bool UnitImpl::siege()
  {
    if ( !canIssueCommand( UnitCommand::siege(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Siege(), sizeof(BW::Orders::Siege));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::siege(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- UNSIEGE ----------------------------------------------------
  bool UnitImpl::unsiege()
  {
    if ( !canIssueCommand( UnitCommand::unsiege(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Unsiege(), sizeof(BW::Orders::Unsiege));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unsiege(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- LIFT -------------------------------------------------------
  bool UnitImpl::lift()
  {
    if ( !canIssueCommand( UnitCommand::lift(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Lift(), sizeof(BW::Orders::Lift));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::lift(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- LAND -------------------------------------------------------
  bool UnitImpl::land(TilePosition target)
  {
    if ( !canIssueCommand( UnitCommand::land(this,target)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Land(BW::TilePosition((u16)target.x(), (u16)target.y()), BW::UnitType((u16)this->self->type)), sizeof(BW::Orders::Land));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::land(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- LOAD -------------------------------------------------------
  bool UnitImpl::load(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::load(this,target)) )
      return false;

    // @Todo: Check if the unit is full

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
    return BroodwarImpl.setLastError(Errors::Incompatible_UnitType);
  }
  //--------------------------------------------- UNLOAD -----------------------------------------------------
  bool UnitImpl::unload(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::unload(this,target)) )
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
    if ( !canIssueCommand( UnitCommand::unloadAll(this)) )
      return false;

    if ( this->_getType.isFlyer() ) // non-bunker
      return this->unloadAll(this->getPosition());

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::UnloadAll(), sizeof(BW::Orders::UnloadAll));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unloadAll(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- UNLOAD ALL -------------------------------------------------
  bool UnitImpl::unloadAll(Position target)
  {
    if ( !canIssueCommand( UnitCommand::unloadAll(this,target)) )
      return false;

    if ( !this->_getType.isFlyer() ) // bunker
      return this->unloadAll();

    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack(BW::Position((u16)target.x(), (u16)target.y()), BW::OrderID::MoveUnload), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::unloadAll(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Position target)
  {
    if ( !canIssueCommand( UnitCommand::rightClick(this,target)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::RightClick(BW::Position((u16)target.x(), (u16)target.y())), sizeof(BW::Orders::RightClick));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::rightClick(this,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- RIGHT CLICK ------------------------------------------------
  bool UnitImpl::rightClick(Unit* target)
  {
    if ( !canIssueCommand( UnitCommand::rightClick(this,target)) )
      return false;

    if (!target->getPlayer()->isNeutral() && this->getPlayer()->isEnemy(target->getPlayer()))
    {
      WeaponType weapon = this->getType().groundWeapon();
      if (target->isLifted() || target->getType().isFlyer())
        weapon = this->getType().airWeapon();

      if (weapon == WeaponTypes::None)
        return BroodwarImpl.setLastError(Errors::Unable_To_Hit);

      if (!this->getType().canMove())
      {
        if (this->getDistance(target) > weapon.maxRange() ||
            this->getDistance(target) < weapon.minRange())
          return BroodwarImpl.setLastError(Errors::Out_Of_Range);
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
    if ( !canIssueCommand( UnitCommand::haltConstruction(this)) )
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
    if ( !canIssueCommand( UnitCommand::cancelConstruction(this)) )
      return false;
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::CancelConstruction(), sizeof(BW::Orders::CancelConstruction));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::cancelConstruction(this)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
  //--------------------------------------------- CANCEL ADDON -----------------------------------------------
  bool UnitImpl::cancelAddon()
  {
    if ( !canIssueCommand( UnitCommand::cancelAddon(this)) )
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
    if ( !canIssueCommand( UnitCommand::cancelTrain(this)) )
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
    if ( !canIssueCommand( UnitCommand::cancelTrain(this,slot)) )
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
    if ( !canIssueCommand( UnitCommand::cancelMorph(this)) )
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
    if ( !canIssueCommand( UnitCommand::cancelResearch(this)) )
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
    if ( !canIssueCommand( UnitCommand::cancelUpgrade(this)) )
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
    if ( !canIssueCommand( UnitCommand::useTech(this,tech)) )
      return false;

    this->orderSelect();
    switch (tech.getID())
    {
      case BW::TechID::Stimpacks:
        QueueGameCommand((PBYTE)&BW::Orders::UseStimPack(), sizeof(BW::Orders::UseStimPack));
        BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech)));
        this->lastOrderFrame = BroodwarImpl.frameCount;
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
        return BroodwarImpl.setLastError(Errors::Incompatible_TechType);
    }
    return true;
  }
  //--------------------------------------------- USE TECH ---------------------------------------------------
  bool UnitImpl::useTech(TechType tech, Position position)
  {
    if ( !canIssueCommand( UnitCommand::useTech(this,tech,position)) )
      return false;
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
        order = BW::OrderID::PlaceMine;
        break;
      case BW::TechID::StasisField:
        order = BW::OrderID::StasisField;
        break;
      default:
        return BroodwarImpl.setLastError(Errors::Incompatible_TechType);
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
    if ( !canIssueCommand( UnitCommand::useTech(this,tech,target)) )
      return false;
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
        return BroodwarImpl.setLastError(Errors::Incompatible_TechType);
    }
    this->orderSelect();
    QueueGameCommand((PBYTE)&BW::Orders::Attack((UnitImpl*)target, order), sizeof(BW::Orders::Attack));
    BroodwarImpl.addToCommandBuffer(new Command(UnitCommand::useTech(this,tech,target)));
    this->lastOrderFrame = BroodwarImpl.frameCount;
    return true;
  }
};
