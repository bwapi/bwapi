#pragma once
#include <Bridge\SharedStuff.h>

namespace Bridge
{
  namespace PipeMessage
  {
#define UNIQUE_MESSAGE_ID static const int Id = (__COUNTER__+1);
#pragma pack(push, 1)
    //------------------------------------------------
    // Automatic ID distribution, starting with 1
    //------------------------------------------------
    //------------------------------------------------
    // Sent once at the beginning as identification
    // Do not change! To keep backwards compability
    // Does not even include BasePacket
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
    // Base packet
    //------------------------------------------------
    template<typename T> struct Packet
    {
      int packetType;
      T data;

      Packet() : packetType(T::Id){};
    };
    //------------------------------------------------
    // Sent at the beginning of a Match to pass control.
    // During this controll time, Agents has access to
    // all neutral units
    // Export one-time exports here
    //------------------------------------------------
    struct ServerInitMatch
    {
      Bridge::SharedStuff::SharedGameDataStructure::Export staticGameDataExport;
      UNIQUE_MESSAGE_ID;
    };
    struct AgentInitMatchDone
    {
      UNIQUE_MESSAGE_ID;
    };
    //------------------------------------------------
    // Sent each frame, to pass control between
    // the two processes.
    // Server sends first.
    //------------------------------------------------
    struct ServerNextFrame
    {
      UNIQUE_MESSAGE_ID;
    };
    struct AgentNextFrameDone
    {
      UNIQUE_MESSAGE_ID;
    };
    struct ServerOnSendText
    {
      std::string text;
      UNIQUE_MESSAGE_ID;
    };
    struct AgentOnSendTextDone
    {
      bool retval;
      UNIQUE_MESSAGE_ID;
    };
    //------------------------------------------------
    // Notifies of the current game being over
    // Requests all Shared objects to be released
    //------------------------------------------------
    struct ServerEndMatch
    {
      UNIQUE_MESSAGE_ID;
    };
    //------------------------------------------------
    // Exports the Unit array
    //------------------------------------------------
    struct ServerExportUnitUpdate
    {
      UNIQUE_MESSAGE_ID;
      // dynamic set export data
    };

#pragma pack(pop)
#define PIPEMESSAGE_TYPE_COUNT UNIQUE_MESSAGE_ID;
#undef UNIQUE_MESSAGE_ID
  }
}
