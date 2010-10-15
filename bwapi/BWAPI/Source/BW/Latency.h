#pragma once
#include <Util/Types.h>
namespace BW
{
  /** Number of frames until actions take effect @ref localData.  */
  namespace Latency
  {
  enum Enum
    {
      SinglePlayer    = 2,
      LanLow          = 5,
      LanMedium       = 7,
      LanHigh         = 9,
      BattlenetLow    = 14,
      BattlenetMedium = 19,
      BattlenetHigh   = 24
    };
  }
};
