#pragma once
/**
 * UnitCommand contains a whole single command
 *
 */

namespace BWAPI
{
  namespace UnitCommandTypeIDs
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
  typedef UnitCommandTypeIDs::Enum UnitCommandTypeID;
}