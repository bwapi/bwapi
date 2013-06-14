#include "Fake_Nuke.h"
#include <tuple>
#include "Controller.h"

using namespace AISCRIPT;

IMPLEMENT(Fake_Nuke);

bool Fake_Nuke::execute(aithread &thread) const
{
  MainController.dwLastNukeTime = Broodwar->elapsedTime() + 1;

  // Save debug info and return
  thread.saveDebug(Text::Green, this->getOpcode());
  return true;
}
