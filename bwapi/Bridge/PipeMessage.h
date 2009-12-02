#pragma once
#include <Bridge\SharedStuff.h>

namespace Bridge
{
  namespace PipeMessage
  {
#define UNIQUE_MESSAGE_ID static const int ID = (__COUNTER__+1);
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
    struct HubHandshake
    {
      bool accepted;
      int hubVersion;
      int hubProcessHandle;
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

      Packet() : packetType(T::ID){};
    };
    //------------------------------------------------
    // Sent at the beginning of a Match to pass controll
    // During this controll time, Agents has access to
    // all neutral units
    //------------------------------------------------
    struct HubOnStart
    {
      Bridge::SharedStuff::SharedGameDataStructure::Export staticGameDataExport;
      UNIQUE_MESSAGE_ID;
    };
    struct AgentOnStartDone
    {
      UNIQUE_MESSAGE_ID;
    };
    //------------------------------------------------
    // Sent each frame, to pass controll between
    // the two processes.
    // Hub sends first.
    //------------------------------------------------
    struct HubNextFrame
    {
      UNIQUE_MESSAGE_ID;
    };
    struct AgentNextFrameDone
    {
      UNIQUE_MESSAGE_ID;
    };
    //------------------------------------------------
    // Notifies of the current game being over
    // Requests all Shared objects to be released
    //------------------------------------------------
    struct HubEndMatch
    {
      UNIQUE_MESSAGE_ID;
    };
    //------------------------------------------------
    // Exports the Unit array
    //------------------------------------------------
    struct HubExportUnitUpdate
    {
      UNIQUE_MESSAGE_ID;
      // dynamic set export data
    };

#pragma pack(pop)
#define PIPEMESSAGE_TYPE_COUNT UNIQUE_MESSAGE_ID;
#undef UNIQUE_MESSAGE_ID
  }
}
