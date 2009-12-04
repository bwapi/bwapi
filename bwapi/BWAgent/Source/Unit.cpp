
#include "Unit.h"
#include "BridgeClient.h"

#include <string>

#include "Util\Version.h"

namespace BWAgent
{
  Unit::Unit()
  {
  }
  int Unit::getID()
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return -1;
    return data->getID;
  }
  int Unit::getType()
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return -1;
    return data->getType;
  }


  int Unit::getHitPoints()
  {
    if(!BridgeClient::sharedStaticData || !this->data)
      return -1;
    return data->getHitPoints;
  }

  bool Unit::holdPosition()
  {
    if(!BridgeClient::sharedCommandData || !this->data)
      return false;
    BWAPI::UnitCommand* c = &(BridgeClient::sharedCommandData->commandQueue[BridgeClient::sharedCommandData->lastFreeCommandSlot]);
    c->unitID = data->getID;
    c->commandID = BWAPI::CommandID::HoldPosition;
    BridgeClient::sharedCommandData->lastFreeCommandSlot++;
    return true;
  }
  void Unit::_update(BWAPI::ClearanceLevels level,BWAPI::State* data)
  {
    this->level=level;
    this->data=data;
  }
}
