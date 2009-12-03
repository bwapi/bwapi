
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
    if(!BridgeClient::sharedStaticData || data==NULL)
      return -1;
    return data->getID;
  }

  int Unit::getHitPoints()
  {
    if(!BridgeClient::sharedStaticData || data==NULL)
      return -1;
    return data->getHitPoints;
  }

  bool Unit::holdPosition()
  {
    if(!BridgeClient::sharedStaticData)
      return false;
    BWAPI::UnitCommand* c = &(BridgeClient::sharedStaticData->commandQueue[BridgeClient::sharedStaticData->lastFreeCommandSlot]);
    c->unitID = data->getID;
    c->commandID = BWAPI::CommandID::HoldPosition;
    BridgeClient::sharedStaticData->lastFreeCommandSlot++;
    return true;
  }
  void Unit::_update(BWAPI::ClearanceLevels level,BWAPI::State* data)
  {
    this->level=level;
    this->data=data;
  }
}
