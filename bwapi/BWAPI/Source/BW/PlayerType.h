#pragma once
namespace BW
{
  /** Direct mapping of bw's player type */
  namespace PlayerType
  {
  enum Enum
    {
      None                  = 0,
      Computer              = 1,
      Player                = 2,
      RescuePassive         = 3,
      RescueActive          = 4,  // no longer used
      EitherPreferComputer  = 5,
      EitherPreferHuman     = 6,
      Neutral               = 7,
      Closed                = 8,
      Observer              = 9,  // no longer used
      PlayerLeft            = 10, /**< Left */
      ComputerLeft          = 11  /**< Left */
    };
  }
};
