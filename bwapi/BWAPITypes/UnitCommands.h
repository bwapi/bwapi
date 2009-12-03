#pragma once
namespace BWAPI
{
  namespace CommandID
  {
    enum Enum
    {
      None,
      AttackPosition,
      AttackUnit,
      RightClickPosition,
      RightClickUnit,
      Train,
      Build,
      BuildAddon,
      Research,
      Upgrade,
      Stop,
      HoldPosition,
      Patrol,
      Follow,
      SetRallyPosition,
      SetRallyUnit,
      Repair,
      Morph,
      Burrow,
      Unburrow,
      Siege,
      Unsiege,
      Cloak,
      Decloak,
      Lift,
      Land,
      Load,
      Unload,
      UnloadAll,
      UnloadAllPosition,
      CancelConstruction,
      HaltConstruction,
      CancelMorph,
      CancelTrain,
      CancelTrainSlot,
      CancelAddon,
      CancelResearch,
      CancelUpgrade,
      UseTech,
      UseTechPosition,
      UseTechUnit
    };
  }
  struct UnitCommand
  {
    CommandID::Enum commandID;
    int unitID;
    int x; //x position or tile position, depending on order type
    int y; //y position or tile position, depending on order type
    int targetID; //target unit ID, if applicable
    int specialID; //tech type, upgrade type, or unit type, depending on command type
  };
}