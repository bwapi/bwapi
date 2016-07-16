#include "Set_Attacks.h"
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Set_Attacks);

bool Set_Attacks::execute(aithread &thread) const
{
  // Parameters
  BYTE bAttacks;
  thread.readTuple( std::tie(bAttacks) );

  // Perform actions
  MainController.bAttacks = bAttacks;

  // Debug
  thread.saveDebug(Text::Green, this->getOpcode(), "%u", bAttacks );
  return true;
}
