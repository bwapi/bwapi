
#include "Unit.h"
#include "BridgeClient.h"

#include <string>

#include "Util\Version.h"

namespace BWAgent
{
  bool Unit::holdPosition()
  {
    if(!BridgeClient::sharedStaticData)
      return false;
    BWAPI::UnitCommand* c = &(BridgeClient::sharedStaticData->commandQueue[BridgeClient::sharedStaticData->lastFreeCommandSlot]);
    c->unitID = 0;//fix fix fix
    c->commandID = BWAPI::CommandID::HoldPosition;
    BridgeClient::sharedStaticData->lastFreeCommandSlot++;
    return true;
  }
}
