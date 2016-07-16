#include "UnitInfo.h"
#include "ScriptEnum.h"

UnitWrap::UnitWrap() : pUnit(nullptr)
{}
UnitWrap::UnitWrap(BWAPI::Unit unit) : pUnit(unit)
{}

UnitWrap &UnitWrap::operator =(BWAPI::Unit unit)
{
  this->pUnit = unit;
  return *this;
}
BWAPI::Unit UnitWrap::operator->() const
{
  return this->pUnit;
}
UnitWrap::operator BWAPI::Unit () const
{
  return this->pUnit;
}

void UnitWrap::AssignComputerIdleOrder()
{
  SetVirtualUnitOrder(AISCRIPT::getUnitIdleOrder(pUnit->getType()));
}

bool UnitWrap::CanUseTech(TechType tech) const
{
  return pUnit->getPlayer()->hasResearched(TechTypes::Lockdown) && pUnit->getEnergy() >= tech.energyCost();
}
bool UnitWrap::UseTech(BWAPI::TechType tech, Unit pTarget, int targBit)
{
  if ( !pTarget )
    return false;

  if ( pUnit->getOrder() == tech.getOrder() && pUnit->getTarget() == pTarget ) // Already targetting it
    return false;
  
  int targFlags = UnitWrap(pTarget).GetTargetFlags();
  if ( targFlags & targBit )
    return false;

  if ( pUnit->useTech(tech, pTarget) ) // Use the tech!
  {
    UnitWrap(pTarget).SetTargetFlags(targFlags | targBit);
    SetVirtualUnitOrder( (Orders::Enum::Enum)tech.getOrder().getID() );
    return true;
  }
  return false;   // If the tech failed
}
bool UnitWrap::UseTechOnClosest(BWAPI::TechType tech, int range, const BWAPI::UnitFilter &pred, int targBit)
{
  Unit pTarget = pUnit->getClosestUnit(pred, range);
  return UseTech(tech, pTarget, targBit);
}

// ----------------------------------------------- GETTERS -----------------------------------------
int UnitWrap::GetOrderTimer() const
{
  return pUnit->getClientInfo<int>(UnitInfo::OrderTimer);
}

int UnitWrap::GetOrderState() const
{
  return pUnit->getClientInfo<int>(UnitInfo::OrderState);
}

Orders::Enum::Enum UnitWrap::GetUnitOrder() const
{ 
  return pUnit->getClientInfo<Orders::Enum::Enum>(UnitInfo::Order); 
}

Orders::Enum::Enum UnitWrap::GetQueuedOrder() const
{ 
  return pUnit->getClientInfo<Orders::Enum::Enum>(UnitInfo::QueuedOrder);
}

Unit UnitWrap::GetAttackTarget() const
{ 
  return pUnit->getClientInfo<Unit >(UnitInfo::AttackTarget); 
}

ControlTypes::Enum UnitWrap::GetControlType() const
{
  return pUnit->getClientInfo<ControlTypes::Enum>(UnitInfo::AI_Controller_Type);
}
CaptainTypes::Enum UnitWrap::GetCaptainType() const
{
  return pUnit->getClientInfo<CaptainTypes::Enum>(UnitInfo::AI_Captain_Type);
}
int UnitWrap::GetWorkerTask() const
{
  return pUnit->getClientInfo<int>(UnitInfo::Worker_Task);
}

BWAPI::Position UnitWrap::GetGuardReturnPosition() const
{
  return BWAPI::Position( pUnit->getClientInfo<int>(UnitInfo::Guard_Return_X),
                          pUnit->getClientInfo<int>(UnitInfo::Guard_Return_Y) );
}

BWAPI::Position UnitWrap::GetOrderTargetPosition() const
{
  return BWAPI::Position( pUnit->getClientInfo<int>(UnitInfo::OrderTarget_X),
                          pUnit->getClientInfo<int>(UnitInfo::OrderTarget_Y) );
}
int UnitWrap::GetTargetFlags() const
{
  return pUnit->getClientInfo<int>(UnitInfo::TargetFlags);
}
// ----------------------------------------------- SETTERS -----------------------------------------
void UnitWrap::SetOrderState(int state)
{ 
  pUnit->setClientInfo(state, UnitInfo::OrderState);  
}

void UnitWrap::SetOrderTimer(int time)
{ 
  pUnit->setClientInfo(time, UnitInfo::OrderTimer);  
}

void UnitWrap::SetQueuedOrder(Orders::Enum::Enum order)
{ 
  pUnit->setClientInfo(order, UnitInfo::QueuedOrder); 
}

void UnitWrap::SetVirtualUnitOrder(Orders::Enum::Enum order, BWAPI::Position pos)
{
  pUnit->setClientInfo( order, UnitInfo::Order);
  pUnit->setClientInfo( 0, UnitInfo::OrderState);
  pUnit->setClientInfo(pos.x, UnitInfo::OrderTarget_X);
  pUnit->setClientInfo(pos.y, UnitInfo::OrderTarget_Y);
}

void UnitWrap::SetAttackTarget(BWAPI::Unit pTarget)
{ 
  pUnit->setClientInfo(pTarget, UnitInfo::AttackTarget); 
}

void UnitWrap::SetControlType(int type)
{
  pUnit->setClientInfo(type, UnitInfo::AI_Controller_Type);
}
void UnitWrap::SetCaptainType(int type)
{
  pUnit->setClientInfo(type, UnitInfo::AI_Captain_Type);
}
void UnitWrap::SetWorkerTask(int type)
{
  pUnit->setClientInfo(type, UnitInfo::Worker_Task);
}

void UnitWrap::SetGuardReturnPosition(Position pos)
{
  pUnit->setClientInfo(pos.x, UnitInfo::Guard_Return_X);
  pUnit->setClientInfo(pos.y, UnitInfo::Guard_Return_Y);
}
void UnitWrap::SetTargetFlags(int flags)
{
  pUnit->setClientInfo(flags, UnitInfo::TargetFlags);
}
