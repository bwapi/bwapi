namespace BWAPI { class UnitPrototype; }
namespace BWAI { class Unit; }
#include "../../BWAPI/Source/BW/Position.h"
namespace BWAI
{
  /** 
   * Represents task to make building given by the ai.
   * It will be used to handle all non-standard situations.
   * -# My building unit is in way (scv that build next building)
   * -# Opponent's probe/scv bugging around
   * -# My idle scv/military unit - will be ordered to move
   * The executor of this command will get the information if the command 
   * execution faced any problems so it can solve it.
   *
   * For example, he will do - if this unit couldn't build supply because of near probe
   * it will order other scv to build supply elswere.
   */
  class BuildingToMake
  {
    public :
      BuildingToMake(Unit* builder, BWAPI::UnitPrototype* buildingType, BW::Position position);
      /** Checks if the unit is trying to to execute the order. */
      void execute();
    private :
      Unit* builder;
      BWAPI::UnitPrototype* buildingType;
      BW::Position position;
      /** Will point to the building that is being constructed, and when it is done, this
       * class can be removed.
       */
      Unit *building;
  };
}
