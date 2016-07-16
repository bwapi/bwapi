#include "Start_Town.h"
#include "Controller.h"

using namespace AISCRIPT;

Start_Town start_town_impl(AISCRIPT::Enum::START_TOWN);
Start_Town start_area_town_impl(AISCRIPT::Enum::START_AREATOWN);

bool Start_Town::execute(aithread &thread) const
{
  /* town *pTown = (bOpcode == AISCRIPT::Enum::START_TOWN) ? AI_StartTown(x,y,flags) : AI_StartAreaTown(x,y,flags);
  MainController.pTown = pTown;
  if ( !pTown )
  {
    this->killThread();
    return;
  }*/
  
  MainController.wFlags |= CONTROLLER_TOWN_STARTED;
  MainController.bIfDif = 1;
  
  // AI_StartShareTown();

  // Debug and return
  thread.saveDebug(Text::Red, this->getOpcode());
  return true;
}
