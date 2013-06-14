#include "UnitInfo.h"

#define REFINERY_GOOD 3
#define REFINERY_DEPLETED 2
#define REFINERY_NEED 1
#define REFINERY_BAD 0

bool UnitWrap::AttendRepair() // @TODO
{
  return false;
}

bool UnitWrap::AttendTown() // @TODO
{
  return false;
}

void UnitWrap::RunWorkerController()
{
  // Check timer
  int timer = pUnit->getClientInfo<int>(UnitInfo::AI_Controller_Timer);
  if ( timer )
  {
    pUnit->setClientInfo(--timer, UnitInfo::AI_Controller_Timer);
    return;
  }

  // Check for things that need to be repaired
  if ( pUnit->getType() == UnitTypes::Terran_SCV && AttendRepair() )
    return;

  // Check if the town is requesting any construction work
  if ( AttendTown() )
    return;
   
  // @TODO
  // if ( !town_has_resource_depot )
  //  return;


  /* switch for something (task value?)
    Unit gas = nullptr;
    Unit mineral = nullptr;
    switch ( blah )
    {
    case 1:
      mineral = pUnit->getTargetResource();
      if ( !mineral || !mineral->getType().isMineralField() )
      {
        mineral = town->resource;
        if ( !mineral || <resourceBelongsToAI> )
        {
          mineral = getNextNearestResource(townCountSomething <= 12, pUnit);
          if ( !mineral )
            mineral = town->resource;
        }
          
        gas = GetTownRefinery(town, REFINERY_GOOD, 1);
        if ( !gas )
          gas = GetTownRefinery(town, REFINERY_DEPLETED, 1);
      }
      AI_Harvest(pUnit, mineral, gas);
      return;
    case 2:
      gas = GetTownRefinery(town, REFINERY_GOOD, 1);
      break;
    case 3:
      gas = GetTownRefinery(town, REFINERY_GOOD, 2);
      break;
    default:
      gas = GetTownRefinery(town, REFINERY_GOOD, 3);
      break;
    }

    if ( !gas )
    {
      if ( !(gas = GetTownRefinery(town, REFINERY_GOOD, 1)) )
      {
        if ( !(gas = GetTownRefinery(town, REFINERY_GOOD, 2)) )
        {
          if ( !(gas = GetTownRefinery(town, REFINERY_GOOD, 3)) )
          {
            if ( !(gas = GetTownRefinery(town, REFINERY_DEPLETED, 1)) )
            {
              if ( !(gas = GetTownRefinery(town, REFINERY_DEPLETED, 2)) )
                gas = GetTownRefinery(town, REFINERY_DEPLETED, 3);
            }
          }
        }
      }
    }
    AI_Harvest(pUnit, gas, town->resource);
  */

}

