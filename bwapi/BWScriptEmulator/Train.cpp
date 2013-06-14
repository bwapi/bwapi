#include "Train.h"
#include "Controller.h"

using namespace AISCRIPT;

Train do_morph_impl(AISCRIPT::Enum::DO_MORPH);
Train wait_force_impl(AISCRIPT::Enum::WAIT_FORCE);
Train train_impl(AISCRIPT::Enum::TRAIN);

bool Train::execute(aithread &thread) const
{
  // Parameters
  BYTE bCount;
  WORD wUnitType;
  thread.readTuple( std::tie(bCount, wUnitType) );

  // Debug
  thread.saveDebug(Text::Green, this->getOpcode(), "%u %s", bCount, AISCRIPT::getUnitName(wUnitType) );

  // Perform actions
  if ( GetStandardUnitCount(wUnitType, this->getOpcode() != AISCRIPT::Enum::TRAIN) < bCount )
  {
    MainController.wWaitForType = (int)wUnitType + 1;
    if ( this->getOpcode() != AISCRIPT::Enum::DO_MORPH )
      return thread.retry();
  }
  return thread.noretry();
}
