#pragma once

namespace BWAPI
{
  namespace UnitCommandType
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
      ReturnCargo,
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
}