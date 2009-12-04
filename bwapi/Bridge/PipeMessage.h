#pragma once
#include <Bridge\SharedStuff.h>

#include <Util\Pipe.h>
#include <Util\TypeHead.h>

namespace Bridge
{
  namespace PipeMessage
  {
#pragma pack(push, 1)
    //------------------------------------------------
    // Automatic ID distribution
    // as line number of structure declaration
    //------------------------------------------------
#define UNIQUE_ID __LINE__
    //------------------------------------------------
    // Sent once at the beginning as identification
    // Do not change! To keep backwards compability
    // Does not even include TypeHead
    //------------------------------------------------
    struct AgentHandshake
    {
      int agentVersion;
      Util::RemoteProcessId agentProcessId;
    };
    struct ServerHandshake
    {
      bool accepted;
      int serverVersion;
      int serverProcessHandle;
    };
    struct AgentHandshakeAcknoledge
    {
      bool accepted;
    };
    //------------------------------------------------
    // Sent at the beginning of a Match to pass control.
    // During this controll time, Agents has access to
    // all neutral units
    //
    // Also exports static shared memory
    //------------------------------------------------
    struct ServerMatchInit : Util::TypeHead<UNIQUE_ID>
    {
      bool fromBeginning;

      Bridge::SharedStuff::StaticGameDataStructure::Export staticGameDataExport;
    };
    struct AgentMatchInitDone : Util::TypeHead<UNIQUE_ID>
    {
    };
    //------------------------------------------------
    // Update packets. One for each update export page,
    //------------------------------------------------
    struct ServerUpdateUserInput : Util::TypeHead<UNIQUE_ID>
    {
      Bridge::SharedStuff::UserInputStack::Export exp;
    };
    struct AgentUpdateCommands : Util::TypeHead<UNIQUE_ID>
    {
      Bridge::SharedStuff::CommandStack::Export exp;
    };
    //------------------------------------------------
    // Sent each frame, to pass control between
    // the two processes.
    // Server sends first.
    //------------------------------------------------
    struct ServerFrameNext : Util::TypeHead<UNIQUE_ID>
    {
    };
    struct AgentFrameNextDone : Util::TypeHead<UNIQUE_ID>
    {
    };
    //------------------------------------------------
    // Notifies of the current game being over
    // Requests all Shared objects to be released
    //------------------------------------------------
    struct ServerMatchEnd : Util::TypeHead<UNIQUE_ID>
    {
    };
    //------------------------------------------------
    //
    //------------------------------------------------
#pragma pack(pop)
#undef UNIQUE_ID
  }
}
