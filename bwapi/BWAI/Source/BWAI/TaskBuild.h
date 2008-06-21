#pragma once

#include <BWAPI/ReservedResources.h>

#include "Task.h"
#include "BuildingPosition.h"

namespace BWAI { class BuildingPositionSet; }

namespace BWAI
{
  /**
   * Represents task to make building given by the ai. It will be used to handle
   * all non-standard situations.
   * -# My building unit is in way (scv that build next building)
   * -# Opponent's probe/scv bugging around
   * -# My idle scv/military unit - will be ordered to move
   * The executor of this command will get the information if the command  execution faced any problems so it can solve it.
   *
   * For example, he will do - if this unit couldn't build supply because of
   * near probe it will order other scv to build supply elswere.
   */
  class TaskBuild : public Task
  {
    public :
      TaskBuild(BW::UnitType buildingType, BuildingPosition* position, Unit* builder, BuildingPositionSet* alternatives);
      virtual ~TaskBuild();
      bool execute();
      BW::UnitType getBuildingType();
      TaskType::Enum getType();
      Unit* getBuilding();
      BWAPI::ReservedResources getReserved();
    private :  
      BW::UnitType buildingType;
      BuildingPosition* position;
      BuildingPositionSet* alternatives;
      /**
       * Will point to the building that is being constructed, and when it is
       * done, this class can be removed.
       */
      Unit *building;
      bool canIBuild(BW::TilePosition here);
  };
}