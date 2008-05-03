#pragma once

namespace BW
{
  namespace OrderID
  {
    enum Enum : u8
    {
      Idle                 = 3,
      Moving               = 6,
      Attacking            = 10,
      AttackMoving         = 14,
      GoingToBuild         = 31,
      Following            = 49,
      ApproachingRafinery  = 81,
      EnteringRafinery     = 82,
      InRafinery           = 83,
      ReturningGas         = 84,
      ApproachingMinerals  = 85,
      Mining               = 87,
      ReturningMinerals    = 90,
      GettingMinedMinerals = 151
    };
  };
};
