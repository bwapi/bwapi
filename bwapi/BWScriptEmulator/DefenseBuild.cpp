#include "DefenseBuild.h"
#include "Controller.h"

using namespace AISCRIPT;

DefenseBuild defenseuse_gg_impl(AISCRIPT::Enum::DEFENSEBUILD_GG);
DefenseBuild defenseuse_ag_impl(AISCRIPT::Enum::DEFENSEBUILD_AG);
DefenseBuild defenseuse_ga_impl(AISCRIPT::Enum::DEFENSEBUILD_GA);
DefenseBuild defenseuse_aa_impl(AISCRIPT::Enum::DEFENSEBUILD_AA);

bool DefenseBuild::execute(aithread &thread) const
{
  // Parameters
  BYTE bCount;
  WORD wUnitType;
  thread.readTuple( std::tie(bCount, wUnitType) );

  MainController.DefenseBuild(this->getOpcode() - AISCRIPT::Enum::DEFENSEBUILD_GG, bCount, wUnitType);

  // Debug and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%u %s", bCount, AISCRIPT::getUnitName(wUnitType) );
  return true;
}
