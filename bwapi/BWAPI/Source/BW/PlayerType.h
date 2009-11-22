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
      NotUsed          = 0,
      Computer         = 1,
      Human            = 2,
      Rescuable        = 3,
      Unknown0         = 4,
      ComputerSlot     = 5,
      OpenSlot         = 6,
      Neutral          = 7,
      ClosedSlot       = 8,
      Unknown1         = 9,
      HumanDefeated    = 10, /**< Left */
      ComputerDefeated = 11  /**< Left */
    };
  }
}
