#include "Send_Suicide.h"
#include <tuple>
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Send_Suicide);

bool Send_Suicide::execute(aithread &thread) const
{
  // Retrieve parameters
  BYTE bSuicideType;
  thread.readTuple( std::tie(bSuicideType) );

  // Execute
  memset(MainController.attackGroups, 0, sizeof(MainController.attackGroups));
  MainController.wAttackGroup = 0;
  // MainController.dwAttackTime = 0;
  // AttackClear();
  // SendSuicide(bSuicideType);
  MainController.dwAttackTime = Broodwar->elapsedTime() - 175;

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u", bSuicideType);
  return true;
}
