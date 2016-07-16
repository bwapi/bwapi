#include "Sharedvision.h"
#include "Controller.h"

using namespace AISCRIPT;

Sharedvision sharedvision_on_impl(AISCRIPT::Enum::SHAREDVISION_ON);
Sharedvision sharedvision_off_impl(AISCRIPT::Enum::SHAREDVISION_OFF);

bool Sharedvision::execute(aithread &thread) const
{
  // Parameters
  BYTE bPlayer;
  thread.readTuple( std::tie(bPlayer) );

  // Vision/Unvision the target player (note: this is the OPPOSITE behaviour of AIScript)
  Broodwar->setVision( Broodwar->getPlayer(bPlayer), this->getOpcode() == Enum::SHAREDVISION_ON);

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u", bPlayer );
  return true;
}
