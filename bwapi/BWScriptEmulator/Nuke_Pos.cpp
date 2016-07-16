#include "Nuke_Pos.h"
#include <tuple>

#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Nuke_Pos);

bool Nuke_Pos::execute(aithread &thread) const
{
  // Params
  WORD x, y;
  thread.readTuple( std::tie(x,y) );

  // @TODO

  // Save debug info and return
  thread.saveDebug(Text::Red, this->getOpcode(), "%u %u", x, y);
  return true;
}
