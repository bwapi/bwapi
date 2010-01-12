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
    // Do not change! To keep backwards compatibility
    // Does not even include TypeHead
    //------------------------------------------------
    struct ServerHandshake
    {
      // protocoll version is defined by the server version
      // agent, if unconfortable, simply detaches
      int serverVersion;
    };
    //------------------------------------------------
    // the initalisation
    //------------------------------------------------
    struct AgentHandshake : Util::TypeHead<UNIQUE_ID>
    {
      Util::RemoteProcessId agentProcessId;
    };
    struct ServerSharedMemoryInit : Util::TypeHead<UNIQUE_ID>
    {
      Bridge::SharedStuff::StaticGameDataStructure::Export staticGameDataExport;
      int serverProcessHandle;
    };
    struct AgentSharedMemoryInit : Util::TypeHead<UNIQUE_ID>
    {
    };
    //------------------------------------------------
    // Update packets. One for each update export page,
    //------------------------------------------------
    struct ServerUpdateEvents : Util::TypeHead<UNIQUE_ID>
    {
      Bridge::SharedStuff::EventStack::Export exp;
    };
    struct AgentUpdateCommands : Util::TypeHead<UNIQUE_ID>
    {
      Bridge::SharedStuff::CommandStack::Export exp;
    };
    struct AgentUpdateDrawShapes : Util::TypeHead<UNIQUE_ID>
    {
      Bridge::SharedStuff::DrawShapeStack::Export exp;
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
