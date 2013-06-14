#include "Set_RandomSeed.h"
#include <tuple>

using namespace AISCRIPT;

IMPLEMENT(Set_RandomSeed);

bool Set_RandomSeed::execute(aithread &thread) const
{
  // Parameter
  DWORD dwRandomSeed;
  thread.readTuple( std::tie(dwRandomSeed) );

  // Set random seed (the noob way)
  srand(dwRandomSeed);

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode(), "%X", dwRandomSeed);
  return true;
}
