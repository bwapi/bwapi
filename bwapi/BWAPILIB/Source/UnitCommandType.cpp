#include <string>
#include <map>
#include <set>
#include <BWAPI/UnitCommandType.h>
#include <Util/Foreach.h>

#include "Common.h"

#include "../../Debug.h"

namespace BWAPI
{
  bool initializingUnitCommandType = true;
  std::string unitCommandTypeName[BWAPI_UNIT_COMMAND_TYPE_COUNT];
  std::map<std::string, UnitCommandType> unitCommandTypeMap;
  std::set< UnitCommandType > unitCommandTypeSet;
  namespace UnitCommandTypes
  {
    const UnitCommandType Attack_Move(0);
    const UnitCommandType Attack_Unit(1);
    const UnitCommandType Build(2);
    const UnitCommandType Build_Addon(3);
    const UnitCommandType Train(4);
    const UnitCommandType Morph(5);
    const UnitCommandType Research(6);
    const UnitCommandType Upgrade(7);
    const UnitCommandType Set_Rally_Position(8);
    const UnitCommandType Set_Rally_Unit(9);
    const UnitCommandType Move(10);
    const UnitCommandType Patrol(11);
    const UnitCommandType Hold_Position(12);
    const UnitCommandType Stop(13);
    const UnitCommandType Follow(14);
    const UnitCommandType Gather(15);
    const UnitCommandType Return_Cargo(16);
    const UnitCommandType Repair(17);
    const UnitCommandType Burrow(18);
    const UnitCommandType Unburrow(19);
    const UnitCommandType Cloak(20);
    const UnitCommandType Decloak(21);
    const UnitCommandType Siege(22);
    const UnitCommandType Unsiege(23);
    const UnitCommandType Lift(24);
    const UnitCommandType Land(25);
    const UnitCommandType Load(26);
    const UnitCommandType Unload(27);
    const UnitCommandType Unload_All(28);
    const UnitCommandType Unload_All_Position(29);
    const UnitCommandType Right_Click_Position(30);
    const UnitCommandType Right_Click_Unit(31);
    const UnitCommandType Halt_Construction(32);
    const UnitCommandType Cancel_Construction(33);
    const UnitCommandType Cancel_Addon(34);
    const UnitCommandType Cancel_Train(35);
    const UnitCommandType Cancel_Train_Slot(36);
    const UnitCommandType Cancel_Morph(37);
    const UnitCommandType Cancel_Research(38);
    const UnitCommandType Cancel_Upgrade(39);
    const UnitCommandType Use_Tech(40);
    const UnitCommandType Use_Tech_Position(41);
    const UnitCommandType Use_Tech_Unit(42);
    const UnitCommandType Place_COP(43);
    const UnitCommandType None(44);
    const UnitCommandType Unknown(45);

#define SET_COMMAND_TYPE(x) unitCommandTypeName[x] = underscoresToSpaces(#x); unitCommandTypeSet.insert(x);
    void init()
    {
      SET_COMMAND_TYPE(Attack_Move);
      SET_COMMAND_TYPE(Attack_Unit);
      SET_COMMAND_TYPE(Build);
      SET_COMMAND_TYPE(Build_Addon);
      SET_COMMAND_TYPE(Train);
      SET_COMMAND_TYPE(Morph);
      SET_COMMAND_TYPE(Research);
      SET_COMMAND_TYPE(Upgrade);
      SET_COMMAND_TYPE(Set_Rally_Position);
      SET_COMMAND_TYPE(Set_Rally_Unit);
      SET_COMMAND_TYPE(Move);
      SET_COMMAND_TYPE(Patrol);
      SET_COMMAND_TYPE(Hold_Position);
      SET_COMMAND_TYPE(Stop);
      SET_COMMAND_TYPE(Follow);
      SET_COMMAND_TYPE(Gather);
      SET_COMMAND_TYPE(Return_Cargo);
      SET_COMMAND_TYPE(Repair);
      SET_COMMAND_TYPE(Burrow);
      SET_COMMAND_TYPE(Unburrow);
      SET_COMMAND_TYPE(Cloak);
      SET_COMMAND_TYPE(Decloak);
      SET_COMMAND_TYPE(Siege);
      SET_COMMAND_TYPE(Unsiege);
      SET_COMMAND_TYPE(Lift);
      SET_COMMAND_TYPE(Land);
      SET_COMMAND_TYPE(Load);
      SET_COMMAND_TYPE(Unload);
      SET_COMMAND_TYPE(Unload_All);
      SET_COMMAND_TYPE(Unload_All_Position);
      SET_COMMAND_TYPE(Right_Click_Position);
      SET_COMMAND_TYPE(Right_Click_Unit);
      SET_COMMAND_TYPE(Halt_Construction);
      SET_COMMAND_TYPE(Cancel_Construction);
      SET_COMMAND_TYPE(Cancel_Addon);
      SET_COMMAND_TYPE(Cancel_Train);
      SET_COMMAND_TYPE(Cancel_Train_Slot);
      SET_COMMAND_TYPE(Cancel_Morph);
      SET_COMMAND_TYPE(Cancel_Research);
      SET_COMMAND_TYPE(Cancel_Upgrade);
      SET_COMMAND_TYPE(Use_Tech);
      SET_COMMAND_TYPE(Use_Tech_Position);
      SET_COMMAND_TYPE(Use_Tech_Unit);
      SET_COMMAND_TYPE(Place_COP);
      SET_COMMAND_TYPE(None);
      SET_COMMAND_TYPE(Unknown);

      foreach(UnitCommandType i, unitCommandTypeSet)
      {
        std::string name(i.getName());
        fixName(&name);
        unitCommandTypeMap.insert(std::make_pair(name, i));
      }
      initializingUnitCommandType = false;
    }
  }
  UnitCommandType::UnitCommandType()
  {
    this->id = UnitCommandTypes::None;
  }
  UnitCommandType::UnitCommandType(int id)
  {
    this->id = id;
    if (!initializingUnitCommandType && (id < 0 || id >= BWAPI_UNIT_COMMAND_TYPE_COUNT))
      this->id = UnitCommandTypes::Unknown;
  }
  UnitCommandType::UnitCommandType(const UnitCommandType& other)
  {
    this->id = other;
  }
  UnitCommandType& UnitCommandType::operator=(const UnitCommandType& other)
  {
    this->id = other;
    return *this;
  }
  UnitCommandType::operator int() const
  {
    return id;
  }
  int UnitCommandType::getID() const
  {
    return this->id;
  }
  std::string UnitCommandType::getName() const
  {
    return unitCommandTypeName[this->id];
  }
  const char *UnitCommandType::c_str() const
  {
    return unitCommandTypeName[this->id].c_str();
  }
  UnitCommandType UnitCommandTypes::getUnitCommandType(std::string name)
  {
    fixName(&name);
    std::map<std::string, UnitCommandType>::iterator i = unitCommandTypeMap.find(name);
    if (i == unitCommandTypeMap.end())
      return UnitCommandTypes::Unknown;
    return (*i).second;
  }
  std::set<UnitCommandType>& UnitCommandTypes::allUnitCommandTypes()
  {
    return unitCommandTypeSet;
  }
}

