#include "Get_OldPeons.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Get_OldPeons);

bool Get_OldPeons::execute(aithread &thread) const
{
  // Parameters
  BYTE bNumPeons;
  thread.readTuple( std::tie( bNumPeons ) );

  // this->pTown->GetOldPeons(bCount);

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u", bNumPeons);
  return true;
}
