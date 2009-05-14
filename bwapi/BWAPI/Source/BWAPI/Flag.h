#pragma once
namespace BWAPI
{
  /** Type of #BWAPI#Command descendant. */
  namespace Flag
  {
    enum Enum
    {
      CompleteMapInformation = 0,
      UserInput              = 1,
    };
  }
  static const int FLAG_COUNT  =  2;
}