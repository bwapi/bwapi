#pragma once

namespace BW
{
  /** Bit mask specifying players who parasited the unit in BW#Unit#parasiteFlags */
  namespace ParasiteFlags
  {
    enum Enum : u8
    {
      Player1Parasited  = 1 << 0,
      Player2Parasited  = 1 << 1,
      Player3Parasited  = 1 << 2,
      Player4Parasited  = 1 << 3,
      Player5Parasited  = 1 << 4,
      Player6Parasited  = 1 << 5,
      Player7Parasited  = 1 << 6,
      Player8Parasited  = 1 << 7,
    };
  };
};

