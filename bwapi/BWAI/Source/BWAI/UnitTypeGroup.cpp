#include "UnitTypeGroup.h"
namespace BWAI
{
  /* Given a group and a unittype, this function will determine if the unit type is in the group */
  bool isInUnitTypeGroup(UnitTypeGroup::Enum grp, BW::UnitType typ)
  {
    switch (grp)
    {
      case UnitTypeGroup::All:
        return true;
      case UnitTypeGroup::Buildings:
        return typ.isBuilding();
      case UnitTypeGroup::Factories:
        return typ.canProduce();
      case UnitTypeGroup::Infantry:
        return typ.canAttack() && typ.isOrganic() && !typ.isWorker();
      case UnitTypeGroup::Mech:
        return typ.isMechanical() && !typ.isWorker();
      case UnitTypeGroup::Men:
        return !typ.isBuilding();
      case UnitTypeGroup::Zerg_Main:
        return (typ==BW::UnitID::Zerg_Hatchery || typ==BW::UnitID::Zerg_Lair || typ==BW::UnitID::Zerg_Hive);
      case UnitTypeGroup::Zerg_MainLair:
        return (typ==BW::UnitID::Zerg_Lair || typ==BW::UnitID::Zerg_Hive);
      default:
        return false;
    }
  }
}