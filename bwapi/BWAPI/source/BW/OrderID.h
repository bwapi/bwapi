#pragma once

#include "../Types.h"
#include <string>

namespace BW
{
  /** Specifies type of action performed by unit. */
  namespace OrderID
  {
    enum Enum : u8
    {
      ExitingBuilding      =  2, /**< Unit is exiting production building (probably). */
      Idle                 =  3, /**< Unit has nothing to do. */
      Moving               =  6, /**< Unit is moving. */
      Attacking            = 10, /**< Unit is attacking target. */
      AttackMoving         = 14, /**< Unit is attack moving. */
      UnderConstruction    = 23, /**< Unit is under construction. */
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

    static const std::string orderName(OrderID::Enum order)
    {
      switch (order)
      {
        case ExitingBuilding : return "Exiting Building";
        case Idle : return "Idle";
        case Moving : return "Moving";
        case Attacking : return "Attacking";
        case AttackMoving : return "Attack Moving";
        case UnderConstruction : return "Under construction";
        case GoingToBuild : return "Going To Build";
        case Following : return "Following";
        case ApproachingRafinery : return "Approaching Rafinery";
        case EnteringRafinery : return "Entering Rafinery";
        case InRafinery : return "In Rafinery";
        case ReturningGas : return "Returning Gas";
        case ApproachingMinerals : return "Approaching Minerals";
        case Mining : return "Mining";
        case ReturningMinerals : return "Returning Minerals";
        case GettingMinedMinerals : return "Getting Mined Minerals";
        default :
        {
          char message[30];
          sprintf(message, "Unknown command %d", order);
          return message;
        }
      }
    }
  };
};
