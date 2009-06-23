#pragma once
namespace BWAPI
{
  /** Type of #BWAPI#Command descendant. */
  namespace CommandTypes
  {
    enum Enum
    {
      Train          = 0,  /** BWAPI#CommandTrain */
      RightClick     = 1,  /** BWAPI#CommandRightClick */
      CancelTrain    = 2,  /** BWAPI#CommandCancelTrain */
      Build          = 3,  /** BWAPI#CommandBuild */
      Invent         = 4,  /** BWAPI#CommandInvent */
      Upgrade        = 5,  /** BWAPI#CommandUpgrade */
      AttackLocation = 6,  /** BWAPI#CommandAttackLocation */
      AttackUnit     = 7,  /** BWAPI#CommandAttackUnit */
      Patrol         = 8,  /** BWAPI#CommandPatrol */
      Stop           = 9,  /** BWAPI#CommandStop */
      HoldPosition   = 10, /** BWAPI#CommandHoldPosition */
    };
  }
}