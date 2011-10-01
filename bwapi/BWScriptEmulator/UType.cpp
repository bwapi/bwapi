#include "UType.h"
#include "ScriptEnum.h"


UType::UType() : UnitType()
{ }

UType::UType(int id) : UnitType(id)
{ }

UType::~UType()
{ }

const char *UType::name()
{
  return AISCRIPT::getUnitName(this->getID());
}

int UType::aiFlag()
{
  return AISCRIPT::getUnitInternal(this->getID());
}

int UType::aiIdle()
{
  return AISCRIPT::getUnitIdleOrder(this->getID());
}
