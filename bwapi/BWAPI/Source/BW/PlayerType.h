namespace BW
{
  /** Direct mapping of bw's player type */
  namespace PlayerType
  {
    enum Enum : u8
    {
      NotUsed          = 0,
      Computer         = 1,
      Human            = 2,
      Rescuable        = 3,
      Neutral          = 7,
      HumanDefeated    = 10, /**< Left */
      ComputerDefeated = 11  /**< Left */
    };
  }
}