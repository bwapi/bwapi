#pragma once
namespace BW
{
  /** Direct mapping of bw's player type */
  namespace PlayerType
  {
  /*  Notes: I think this is ID & 0x07
             Defeated or inactive is ID & 0x08
  */
  enum Enum
    {
      None                  = 0,
      Computer              = 1,
      Player                = 2,
      RescuePassive         = 3,
      Unknown4              = 4,
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
