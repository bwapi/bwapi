#pragma once
namespace BWAPI
{
  namespace Flag
  {
    enum Enum
    {
      /** Enable to get information about all units on the map, not just the visible units. */
      CompleteMapInformation = 0,

      /** Enable to get information from the user (what units are selected, chat messages the user enters,
       * etc) */
      UserInput              = 1,
    };
  }
  static const int FLAG_COUNT  =  2;
}
