#include "DefenseUse.h"
#include "Controller.h"

using namespace AISCRIPT;

DefenseUse defenseuse_gg_impl(AISCRIPT::Enum::DEFENSEUSE_GG);
DefenseUse defenseuse_ag_impl(AISCRIPT::Enum::DEFENSEUSE_AG);
DefenseUse defenseuse_ga_impl(AISCRIPT::Enum::DEFENSEUSE_GA);
DefenseUse defenseuse_aa_impl(AISCRIPT::Enum::DEFENSEUSE_AA);

bool DefenseUse::execute(aithread &thread) const
{
  // Parameters
  BYTE bCount;
  WORD wUnitType;
  thread.readTuple( std::tie(bCount, wUnitType) );

  MainController.DefenseUse(this->getOpcode() - AISCRIPT::Enum::DEFENSEUSE_GG, bCount, wUnitType);

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u %s", bCount, AISCRIPT::getUnitName(wUnitType) );
  return true;
}
