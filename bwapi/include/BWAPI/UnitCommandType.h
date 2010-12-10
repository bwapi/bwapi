#pragma once
#include <string>
#include <set>

#define BWAPI_UNIT_COMMAND_TYPE_COUNT 46

namespace BWAPI
{
  class UnitCommandType
  {
    public:
      UnitCommandType();
      UnitCommandType(int id);
      UnitCommandType(const UnitCommandType& other);
      UnitCommandType& operator=(const UnitCommandType& other);
      bool operator==(const UnitCommandType& other) const;
      bool operator!=(const UnitCommandType& other) const;
      bool operator<(const UnitCommandType& other) const;
      bool operator>(const UnitCommandType& other) const;

      /** Returns a unique ID for this UnitCommandType. */
      int getID() const;

      /** Returns the string corresponding to the UnitCommandType object. For example,
       * UnitCommandTypes::Set_Rally_Position.getName() returns std::string("Set Rally Position")*/
      std::string getName() const;
    private:
      int id;
  };
  namespace UnitCommandTypes
  {
    /** Given a string, this function returns the command type it refers to. For example,
     * UnitCommandTypes::getUnitCommandType("Attack Position") returns UnitCommandTypes::Attack_Position. */
    UnitCommandType getUnitCommandType(std::string name);

    /** Returns the set of all the sizes, which are listed below: */
    std::set<UnitCommandType>& allUnitCommandTypes();
    void init();
    extern const UnitCommandType Attack_Move;
    extern const UnitCommandType Attack_Unit;
    extern const UnitCommandType Build;
    extern const UnitCommandType Build_Addon;
    extern const UnitCommandType Train;
    extern const UnitCommandType Morph;
    extern const UnitCommandType Research;
    extern const UnitCommandType Upgrade;
    extern const UnitCommandType Set_Rally_Position;
    extern const UnitCommandType Set_Rally_Unit;
    extern const UnitCommandType Move;
    extern const UnitCommandType Patrol;
    extern const UnitCommandType Hold_Position;
    extern const UnitCommandType Stop;
    extern const UnitCommandType Follow;
    extern const UnitCommandType Gather;
    extern const UnitCommandType Return_Cargo;
    extern const UnitCommandType Repair;
    extern const UnitCommandType Burrow;
    extern const UnitCommandType Unburrow;
    extern const UnitCommandType Cloak;
    extern const UnitCommandType Decloak;
    extern const UnitCommandType Siege;
    extern const UnitCommandType Unsiege;
    extern const UnitCommandType Lift;
    extern const UnitCommandType Land;
    extern const UnitCommandType Load;
    extern const UnitCommandType Unload;
    extern const UnitCommandType Unload_All;
    extern const UnitCommandType Unload_All_Position;
    extern const UnitCommandType Right_Click_Position;
    extern const UnitCommandType Right_Click_Unit;
    extern const UnitCommandType Halt_Construction;
    extern const UnitCommandType Cancel_Construction;
    extern const UnitCommandType Cancel_Addon;
    extern const UnitCommandType Cancel_Train;
    extern const UnitCommandType Cancel_Train_Slot;
    extern const UnitCommandType Cancel_Morph;
    extern const UnitCommandType Cancel_Research;
    extern const UnitCommandType Cancel_Upgrade;
    extern const UnitCommandType Use_Tech;
    extern const UnitCommandType Use_Tech_Position;
    extern const UnitCommandType Use_Tech_Unit;
    extern const UnitCommandType Place_COP;
    extern const UnitCommandType None;
    extern const UnitCommandType Unknown;
  }
}