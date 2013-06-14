#include "Attack_Add.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

Attack_Add attack_add_impl(Enum::ATTACK_ADD);
Attack_Add easy_attack_impl(Enum::EASY_ATTACK);

bool Attack_Add::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bCount;
  WORD wUnitType;
  thread.readTuple( std::tie(bCount, wUnitType) );

  // Debug
  thread.saveDebug(Text::Green, this->getOpcode(), "%u %s", bCount, AISCRIPT::getUnitName(wUnitType));

  // Don't add to attack group if this is easy_attack and diff value is not 0
  if ( this->getOpcode() == Enum::EASY_ATTACK && MainController.bIfDif > 0 )
    return true;

  // Add to attack group
  MainController.AttackAdd(bCount, wUnitType);

  return true;
}
