#pragma once
/**
 * These entries are packets in the per-frame stack SharedStuff::CommandStack
 * Messages that are sent from client to server.
 */

#include <BWAPITypes\UnitCommand.h>

// zero sized arrays are FINE! It's the C++ standart who's being crazy here
#pragma warning(disable:4200)

namespace Bridge
{
  namespace CommandEntry
  {
#pragma pack(push, 1)
#define UNIQUE_ID __LINE__
    struct SendText : Util::TypeHead<UNIQUE_ID>
    {
      bool printOnly;
      char text[0];
    };

    struct UnitOrder : Util::TypeHead<UNIQUE_ID>
    {
      BWAPI::UnitCommand unitCommand;
    };

    struct StartGame : Util::TypeHead<UNIQUE_ID>
    {
    };

    struct PauseGame : Util::TypeHead<UNIQUE_ID>
    {
    };

    struct LeaveGame : Util::TypeHead<UNIQUE_ID>
    {
    };

    struct ResumeGame : Util::TypeHead<UNIQUE_ID>
    {
    };

    struct RestartGame : Util::TypeHead<UNIQUE_ID>
    {
    };

    struct SetLocalSpeed : Util::TypeHead<UNIQUE_ID>
    {
      u32 speed;
    };
#undef UNIQUE_ID
#pragma pack(pop)
  }
}
