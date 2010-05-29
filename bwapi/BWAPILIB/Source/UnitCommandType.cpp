#include <string>
#include <map>
#include <set>
#include <BWAPI/UnitCommandType.h>

namespace BWAPI
{
  bool initializingUnitCommandType = true;
  std::string unitCommandTypeName[43];
  std::map<std::string, UnitCommandType> unitCommandTypeMap;
  std::set< UnitCommandType > unitCommandTypeSet;
  namespace UnitCommandTypes
  {
    const UnitCommandType Attack_Position(0);
    const UnitCommandType Attack_Unit(1);
    const UnitCommandType Right_Click_Position(2);
    const UnitCommandType Right_Click_Unit(3);
    const UnitCommandType Train(4);
    const UnitCommandType Build(5);
    const UnitCommandType Build_Addon(6);
    const UnitCommandType Research(7);
    const UnitCommandType Upgrade(8);
    const UnitCommandType Stop(9);
    const UnitCommandType Hold_Position(10);
    const UnitCommandType Patrol(11);
    const UnitCommandType Follow(12);
    const UnitCommandType Set_Rally_Position(13);
    const UnitCommandType Set_Rally_Unit(14);
    const UnitCommandType Repair(15);
    const UnitCommandType Return_Cargo(16);
    const UnitCommandType Morph(17);
    const UnitCommandType Burrow(18);
    const UnitCommandType Unburrow(19);
    const UnitCommandType Siege(20);
    const UnitCommandType Unsiege(21);
    const UnitCommandType Cloak(22);
    const UnitCommandType Decloak(23);
    const UnitCommandType Lift(24);
    const UnitCommandType Land(25);
    const UnitCommandType Load(26);
    const UnitCommandType Unload(27);
    const UnitCommandType Unload_All(28);
    const UnitCommandType Unload_All_Position(29);
    const UnitCommandType Cancel_Construction(30);
    const UnitCommandType Halt_Construction(31);
    const UnitCommandType Cancel_Morph(32);
    const UnitCommandType Cancel_Train(33);
    const UnitCommandType Cancel_Train_Slot(34);
    const UnitCommandType Cancel_Addon(35);
    const UnitCommandType Cancel_Research(36);
    const UnitCommandType Cancel_Upgrade(37);
    const UnitCommandType Use_Tech(38);
    const UnitCommandType Use_Tech_Position(39);
    const UnitCommandType Use_Tech_Unit(40);
    const UnitCommandType None(41);
    const UnitCommandType Unknown(42);
    void init()
    {
      unitCommandTypeName[Attack_Position.getID()] = "Attack Position";
      unitCommandTypeName[Attack_Unit.getID()] = "Attack Unit";
      unitCommandTypeName[Right_Click_Position.getID()] = "Right Click Position";
      unitCommandTypeName[Right_Click_Unit.getID()] = "Right Click Unit";
      unitCommandTypeName[Train.getID()] = "Train";
      unitCommandTypeName[Build.getID()] = "Build";
      unitCommandTypeName[Build_Addon.getID()] = "Build Addon";
      unitCommandTypeName[Research.getID()] = "Research";
      unitCommandTypeName[Upgrade.getID()] = "Upgrade";
      unitCommandTypeName[Stop.getID()] = "Stop";
      unitCommandTypeName[Hold_Position.getID()] = "Hold Position";
      unitCommandTypeName[Patrol.getID()] = "Patrol";
      unitCommandTypeName[Follow.getID()] = "Follow";
      unitCommandTypeName[Set_Rally_Position.getID()] = "Set Rally Position";
      unitCommandTypeName[Set_Rally_Unit.getID()] = "Set Rally Unit";
      unitCommandTypeName[Repair.getID()] = "Repair";
      unitCommandTypeName[Return_Cargo.getID()] = "Return Cargo";
      unitCommandTypeName[Morph.getID()] = "Morph";
      unitCommandTypeName[Burrow.getID()] = "Burrow";
      unitCommandTypeName[Unburrow.getID()] = "Unburrow";
      unitCommandTypeName[Siege.getID()] = "Siege";
      unitCommandTypeName[Unsiege.getID()] = "Unsiege";
      unitCommandTypeName[Cloak.getID()] = "Cloak";
      unitCommandTypeName[Decloak.getID()] = "Decloak";
      unitCommandTypeName[Lift.getID()] = "Lift";
      unitCommandTypeName[Land.getID()] = "Land";
      unitCommandTypeName[Load.getID()] = "Load";
      unitCommandTypeName[Unload.getID()] = "Unload";
      unitCommandTypeName[Unload_All.getID()] = "Unload All";
      unitCommandTypeName[Unload_All_Position.getID()] = "Unload All Position";
      unitCommandTypeName[Cancel_Construction.getID()] = "Cancel Construction";
      unitCommandTypeName[Halt_Construction.getID()] = "Halt Construction";
      unitCommandTypeName[Cancel_Morph.getID()] = "Cancel Morph";
      unitCommandTypeName[Cancel_Train.getID()] = "Cancel Train";
      unitCommandTypeName[Cancel_Train_Slot.getID()] = "Cancel Train Slot";
      unitCommandTypeName[Cancel_Addon.getID()] = "Cancel Addon";
      unitCommandTypeName[Cancel_Research.getID()] = "Cancel Research";
      unitCommandTypeName[Cancel_Upgrade.getID()] = "Cancel Upgrade";
      unitCommandTypeName[Use_Tech.getID()] = "Use Tech";
      unitCommandTypeName[Use_Tech_Position.getID()] = "Use Tech Position";
      unitCommandTypeName[Use_Tech_Unit.getID()] = "Use Tech Unit";
      unitCommandTypeName[None.getID()] = "None";
      unitCommandTypeName[Unknown.getID()] = "Unknown";

      unitCommandTypeSet.insert(Attack_Position);
      unitCommandTypeSet.insert(Attack_Unit);
      unitCommandTypeSet.insert(Right_Click_Position);
      unitCommandTypeSet.insert(Right_Click_Unit);
      unitCommandTypeSet.insert(Train);
      unitCommandTypeSet.insert(Build);
      unitCommandTypeSet.insert(Build_Addon);
      unitCommandTypeSet.insert(Research);
      unitCommandTypeSet.insert(Upgrade);
      unitCommandTypeSet.insert(Stop);
      unitCommandTypeSet.insert(Hold_Position);
      unitCommandTypeSet.insert(Patrol);
      unitCommandTypeSet.insert(Follow);
      unitCommandTypeSet.insert(Set_Rally_Position);
      unitCommandTypeSet.insert(Set_Rally_Unit);
      unitCommandTypeSet.insert(Repair);
      unitCommandTypeSet.insert(Return_Cargo);
      unitCommandTypeSet.insert(Morph);
      unitCommandTypeSet.insert(Burrow);
      unitCommandTypeSet.insert(Unburrow);
      unitCommandTypeSet.insert(Siege);
      unitCommandTypeSet.insert(Unsiege);
      unitCommandTypeSet.insert(Cloak);
      unitCommandTypeSet.insert(Decloak);
      unitCommandTypeSet.insert(Lift);
      unitCommandTypeSet.insert(Land);
      unitCommandTypeSet.insert(Load);
      unitCommandTypeSet.insert(Unload);
      unitCommandTypeSet.insert(Unload_All);
      unitCommandTypeSet.insert(Unload_All_Position);
      unitCommandTypeSet.insert(Cancel_Construction);
      unitCommandTypeSet.insert(Halt_Construction);
      unitCommandTypeSet.insert(Cancel_Morph);
      unitCommandTypeSet.insert(Cancel_Train);
      unitCommandTypeSet.insert(Cancel_Train_Slot);
      unitCommandTypeSet.insert(Cancel_Addon);
      unitCommandTypeSet.insert(Cancel_Research);
      unitCommandTypeSet.insert(Cancel_Upgrade);
      unitCommandTypeSet.insert(Use_Tech);
      unitCommandTypeSet.insert(Use_Tech_Position);
      unitCommandTypeSet.insert(Use_Tech_Unit);
      unitCommandTypeSet.insert(None);
      unitCommandTypeSet.insert(Unknown);

      for(std::set<UnitCommandType>::iterator i = unitCommandTypeSet.begin(); i != unitCommandTypeSet.end(); i++)
      {
        std::string name=(*i).getName();
        for(int j=0;j<(int)name.length();j++)
        {
          if (name[j]==' ') name[j]='_';
          if (name[j]>='a' && name[j]<='z') name[j]+='A'-'a';
        }
        unitCommandTypeMap.insert(std::make_pair(name, *i));
      }
      initializingUnitCommandType = false;
    }
  }
  UnitCommandType::UnitCommandType()
  {
    this->id = UnitCommandTypes::None.id;
  }
  UnitCommandType::UnitCommandType(int id)
  {
    this->id = id;
    if (!initializingUnitCommandType)
    {
      if (id < 0 || id >= 43)
      {
        this->id = UnitCommandTypes::Unknown.id;
      }
    }
  }
  UnitCommandType::UnitCommandType(const UnitCommandType& other)
  {
    this->id = other.id;
  }
  UnitCommandType& UnitCommandType::operator=(const UnitCommandType& other)
  {
    this->id = other.id;
    return *this;
  }
  bool UnitCommandType::operator==(const UnitCommandType& other) const
  {
    return this->id == other.id;
  }
  bool UnitCommandType::operator!=(const UnitCommandType& other) const
  {
    return this->id != other.id;
  }
  bool UnitCommandType::operator<(const UnitCommandType& other) const
  {
    return this->id < other.id;
  }
  int UnitCommandType::getID() const
  {
    return this->id;
  }
  std::string UnitCommandType::getName() const
  {
    return unitCommandTypeName[this->id];
  }

  UnitCommandType UnitCommandTypes::getUnitCommandType(std::string& name)
  {
    for(int j=0;j<(int)name.length();j++)
    {
      if (name[j]==' ') name[j]='_';
      if (name[j]>='a' && name[j]<='z') name[j]+='A'-'a';
    }
    std::map<std::string, UnitCommandType>::iterator i = unitCommandTypeMap.find(name);
    if (i == unitCommandTypeMap.end()) return UnitCommandTypes::Unknown;
    return (*i).second;
  }
  std::set<UnitCommandType>& UnitCommandTypes::allUnitCommandTypes()
  {
    return unitCommandTypeSet;
  }
}

