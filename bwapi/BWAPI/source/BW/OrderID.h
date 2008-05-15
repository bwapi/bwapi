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
      GameNotInitialized      =   0, /** Everything has order 0 when the*/
      Finishing               =   2, /**< Unit is exiting production building and finishing command (move), maybe more. */
      Idle                    =   3, /**< Unit has nothing to do. */
      Moving                  =   6, /**< Unit is moving. */
      Attacking               =  10, /**< Unit is attacking target. */
      AttackMoving            =  14, /**< Unit is attack moving. */
      NotMovable              =  23, /**< Unit can't move (building/mineral/under construction). */
      JustToMutate            =  26, /**< The zerg drone is just about to mutate to building */
      GoingToWarp             =  31, /**< Probe is going to warp building */
      Constructing            =  33, /**< Scv is constructing building */
      GoingToBuild            =  30, /**< Unit is approaching place where it will make building. */
      Repair                  =  34, /**< Approaching unit to repair + repairing. */
      BuildingWarping         =  41, /**< Protoss buliding is warping (constructing). */
      EggMutating             =  42, /**< Zerg egg is mutating. */
      BuildingMutating        =  45, /**< Zerg building is mutating (only Terran tested). */
      UnderConstruction       =  44, /**< Unit is under construction (only Terran tested). */
      AssimilatorWarping      =  48, /**< Protoss assimilator is warping. */
      Following               =  49, /**< Unit is following other unit (can be unit of the same player, or enemy - if the performing unit can't attack). */
      GoingToMutate           =  70, /**< Going to mutate to building */
      Building_Landing        =  71, /**< Terran building is landing (or ordered to land somewhere */
      Lifting                 =  74, /**< Terran building on the ground is lifting, Drone is lifting from cancelled building */
      ArmoryUpgrading         =  76, /**< Terran armory (maybe other bulding also) is upgrading*/
      NotControllable         =  77, /**< Larva has it, the only movable not controllable unit I know so far */
      ApproachingRafinery     =  81, /**< The gathering unit is on it's way to rafinery */
      EnteringRafinery        =  82, /**< @todo Investigate The unit is just entering Rafinery/Extractor/Assimilator (while have it special state?)*/
      InRafinery              =  83, /**< The unit is inside of Rafinery/Extractor/Asimilator */
      ReturningGas            =  84, /**< Unit gathering gas resource is returning with it to Hatchery/CommandCenter/Nexus */
      ApproachingMinerals     =  85, /**< Unit is approaching minerals and will gather them (so only unit that can gather can do this)*/
      StartingMining          =  86, /**< Unit is starting it's mining process (not that even when the mineral is occupied and the worker will go elswhere this state will be present) */
      Mining                  =  87, /**< Unit is mining minerals */
      ReturningMinerals       =  90, /**< Unit is with minerals is returning it back to Command Center/Hatchery/nexus */
      OverlordIdle            =  93, /**< Overlord is idle - and is doing it's strange rotation stuff */
      Burrowing               = 116, /**< Zerg unit performing the burrow command */
      Burrowed                = 117, /**< Zerg burrowed unit */
      Unburrowing             = 118, /**< Zerg burrowed performing the unburrow command */
      Stop                    = 150, /**< The unit has give order to stop*/
      GettingMinedMinerals    = 151, /**< @todo Investigate That action is there for a moment between Mining and Returning Minerals states */
      ComputerCommand         = 156, /**< True meaning of this is unknown, but computer ordering units uses this command all the time. */
      ComputerOverlordCommand = 159, /**< True meaning of this is unknown, but computer ordering units uses this command all the time. */
      CritterWandering        = 166, /**< @todo Investigate That action is there for a moment between Mining and Returning Minerals states */
      MedicHeal               = 176  /**< Healing medic. */
    };

    static const std::string orderName(OrderID::Enum order)
    {
      switch (order)
      {
        case GameNotInitialized : return "Game not initialized";
        case Finishing : return "Finishing";
        case Idle : return "Idle";
        case Moving : return "Moving";
        case Attacking : return "Attacking";
        case AttackMoving : return "Attack Moving";
        case NotMovable : return "Not Movable";
        case Constructing : return "Constructing";
        case GoingToBuild : return "Going To Build";
        case Repair : return "Repair";
        case EggMutating  : return "Egg Mutating";
        case BuildingMutating : return "Building mutating";
        case UnderConstruction : return "Under Construction";
        case Following : return "Following";
        case Building_Landing : return "Building landing";
        case Lifting : return "Lifting";
        case NotControllable : return "Not Controllable";
        case ApproachingRafinery : return "Approaching Rafinery";
        case EnteringRafinery : return "Entering Rafinery";
        case InRafinery : return "In Rafinery";
        case ReturningGas : return "Returning Gas";
        case ApproachingMinerals : return "Approaching Minerals";
        case StartingMining : return "Starting Mining";
        case Mining : return "Mining";
        case ReturningMinerals : return "Returning Minerals";
        case OverlordIdle : return "Overlord Idle";
        case Burrowing : return "Burrowing";
        case Burrowed : return "Burrowed";
        case Unburrowing  : return "Unburrowing";
        case GettingMinedMinerals : return "Getting Mined Minerals";
        case ComputerCommand : return "Computer command";
        case CritterWandering : return "Critter wandering";
                                       
        default :
        {
          char message[50];
          sprintf(message, "Unknown command %d", order);
          return message;
        }
      }
    }
  };
};
