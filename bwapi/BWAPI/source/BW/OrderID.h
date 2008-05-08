#pragma once

#include "../Types.h"

namespace BW
{
  /** Specifies type of action performed by unit. */
  namespace OrderID
  {
    enum Enum : u8
    {
      Idle                 =  3, /**< Unit has nothing to do. */
      Moving               =  6, /**< Unit is moving. */
      Attacking            = 10, /**< Unit is attacking target. */
      AttackMoving         = 14, /**< Unit is attack moving. */
      GoingToBuild         = 31, /**< Unit is approaching place where it will make building. */
      Following            = 49, /**< Unit is following other unit (can be unit of the same player, or enemy - if the performing unit can't attack). */
      ApproachingRafinery  = 81, /**< The gathering unit is on it's way to rafinery */
      EnteringRafinery     = 82, /**< @todo Investigate The unit is just entering Rafinery/Extractor/Assimilator (while have it special state?)*/
      InRafinery           = 83, /**< The unit is inside of Rafinery/Extractor/Asimilator */
      ReturningGas         = 84, /**< Unit gathering gas resource is returning with it to Hatchery/CommandCenter/Nexus */
      ApproachingMinerals  = 85, /**< Unit is approaching minerals and will gather them (so only unit that can gather can do this)*/
      Mining               = 87, /**< Unit is mining minerals */
      ReturningMinerals    = 90, /**< Unit is with minerals is returning it back to Command Center/Hatchery/nexus */
      GettingMinedMinerals = 151 /**< @todo Investigate That action is there for a moment between Mining and Returning Minerals states */
    };
  };
};
