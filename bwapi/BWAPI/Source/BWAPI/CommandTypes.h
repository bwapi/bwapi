#pragma once
namespace BWAPI
{
  /** Type of #BWAPI#Command descendant. */
  namespace CommandTypes
  {
    enum Enum
    {
      Train,
      RightClick,
      CancelTrain,
      Build,
      Invent,
      Upgrade,
      AttackMove,
      AttackUnit,
      Patrol,
      Stop,
      HoldPosition,
      Repair,
      MorphUnit,
      MorphBuilding,
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
      CancelResearch,
      CancelUpgrade,
      CancelConstruction,
      CancelAddon,
      CancelMorph,
    };
  }
}