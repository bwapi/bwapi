#include "BridgeServer.h"

#include <Bridge\Constants.h>
#include <Bridge\SharedStuff.h>
#include <Bridge\PipeMessage.h>

#include <Util\Version.h>

namespace BWAPI
{
  // see it as a singleton class.
  namespace BridgeServer
  {
  //private:
    //-------------------------- PRIVATE VARIABLES ----------------------------------------------
    bool stateConnectionEstablished;
    Bridge::SharedStuff sharedStuff;

    //-------------------------- ----------------------------------------------------------------
    std::string lastError;
    void resetError()
    {
      lastError = "no bridge error";
    }
    const std::string &getLastError()
    {
      return lastError;
    }
    //-------------------------- ----------------------------------------------------------------
  //public:
    //-------------------------- PUBLIC DATA ----------------------------------------------------
    Bridge::StaticGameDataStructure* sharedStaticData = NULL;
    //-------------------------- INIT -----------------------------------------------------------
    bool initConnectionServer()
    {
      resetError();
      if(!sharedStuff.pipe.create(Bridge::globalPipeName))
      {
        lastError = "Could not create pipe server";
        return false;
      }
      return true;
    }
    //-------------------------- CHECK INCOMING CONNECTIONS -------------------------------------
    bool acceptIncomingConnections()
    {
      // check for incoming pipe connections
      if(!stateConnectionEstablished && sharedStuff.pipe.pollIncomingConnection())
      {
        // receive handshake
        Bridge::PipeMessage::AgentHandshake handshake;
        {
          Util::Buffer data;
          if(!sharedStuff.pipe.receive(data))
          {
            lastError = "Could not receive pipe";
            return false;
          }
          if(!data.getMemory().readTo(handshake))
          {
            lastError = "Packet corrupt";
            return false;
          }
        }

        // audit agent
        bool accept = true;
        if(handshake.agentVersion != SVN_REV)
        {
          accept = false;
          lastError = "Wrong agent version, rejected";
        }
        if(!sharedStuff.remoteProcess.acquire(handshake.agentProcessId, true))
        {
          accept = false;
          lastError = "Could not open agent's process, rejected";
        }

        // send back response
        Bridge::PipeMessage::ServerHandshake handshake2;
        handshake2.accepted = accept;
        handshake2.serverProcessHandle = sharedStuff.remoteProcess.exportOwnHandle();
        handshake2.serverVersion = SVN_REV;
        if(!sharedStuff.pipe.sendStructure(handshake2))
        {
          lastError = "Could not open agent's process, rejected";
          return false;
        }

        // agent screw'd it, beat it
        if(!accept)
        {
          return false;
        }

        // wait for response acknoledgement
        Bridge::PipeMessage::AgentHandshakeAcknoledge ack;
        {
          Util::Buffer data;
          if(!sharedStuff.pipe.receive(data))
          {
            lastError = "Could not receive pipe";
            return false;
          }
          if(!data.getMemory().readTo(ack))
          {
            lastError = "Packet corrupt";
            return false;
          }
        }
        if(!ack.accepted)
        {
          lastError = "Agent has rejected response (could not open process handle?)";
          return false;
        }

        stateConnectionEstablished = true;
        return true;
      }
      return true;
    }
    //-------------------------- INIT MATCH -----------------------------------------------------
    bool initMatch()
    {
      // check prerequisites
      if(!stateConnectionEstablished)
      {
        lastError = __FUNCTION__ ": connection not established";
        return false;
      }

      // create match initialization packet
      Bridge::PipeMessage::Packet<Bridge::PipeMessage::ServerInitMatch> initMatchEvent;
      if(initMatchEvent.packetType != Bridge::PipeMessage::ServerInitMatch::Id)
      {
        lastError = __FUNCTION__ ": unique id macro does not work";
        return false;
      }

      // create and export static data
      if(!sharedStuff.staticData.create())
      {
        lastError = __FUNCTION__ ": staticData creating failed";
        return false;
      }
      initMatchEvent.data.staticGameDataExport = sharedStuff.staticData.exportToProcess(
        sharedStuff.remoteProcess, true);
      if(!initMatchEvent.data.staticGameDataExport.isValid())
      {
        lastError = __FUNCTION__ ": staticData export failed";
        return false;
      }

      sharedStaticData = &sharedStuff.staticData.get();

      if(!sharedStuff.pipe.sendStructure(initMatchEvent))
      {
        lastError = __FUNCTION__ ": error sending packet";
        return false;
      }

      // wait untill event is done
      Util::Buffer buffer;
      if(!sharedStuff.pipe.receive(buffer))
      {
        lastError = __FUNCTION__ ": error receiving completion event";
        return false;
      }

      // audit completion
      Util::MemoryFrame packet = buffer.getMemory();
      int packetType;
      packet.readTo(packetType);
      if(packetType != Bridge::PipeMessage::AgentInitMatchDone::Id)
      {
        lastError = __FUNCTION__ ": received unexpected packet type " + packetType;
        return false;
      }

      Bridge::PipeMessage::AgentInitMatchDone initMatchDone;
      if(!packet.readTo(initMatchDone))
      {
        lastError = __FUNCTION__ ": received packet too small";
        return false;
      }

      initMatchDone;  // yet no data to read

      return true;
    }
    //-------------------------- INVOKE ON FRAME ------------------------------------------------
    bool invokeOnFrame()
    {
      // check prerequisites
      if(!stateConnectionEstablished)
      {
        lastError = __FUNCTION__ ": no connection established";
        return false;
      }

      // send next frame invocation packet
      Bridge::PipeMessage::Packet<Bridge::PipeMessage::ServerNextFrame> nextFrame;
      nextFrame;  // no data yet
      sharedStuff.pipe.sendStructure(nextFrame);

      // receive completion notification
      Util::Buffer buffer;
      if(!sharedStuff.pipe.receive(buffer))
      {
        lastError = __FUNCTION__ ": receive completion packet failed";
        return false;
      }

      // parse packet
      Util::MemoryFrame packet = buffer.getMemory();
      int packetType;
      if(!packet.readTo(packetType))
      {
        lastError = __FUNCTION__ ": received packet too small";
        return false;
      }

      if(packetType != Bridge::PipeMessage::AgentNextFrameDone::Id)
      {
        lastError = __FUNCTION__ ": unexpected packet type " + packetType;
        return false;
      }

      Bridge::PipeMessage::AgentNextFrameDone completion;
      if(!packet.readTo(completion))
      {
        lastError = __FUNCTION__ ": completion packet too small";
        return false;
      }

      completion; // yet no data here

      return true;
    }
    //------------------------------ ON SEND TEXT ------------------------------------------------
    bool invokeOnSendText(std::string text)
    {
      // check prerequisites
      if(!stateConnectionEstablished)
      {
        lastError = __FUNCTION__ ": no connection established";
        return false;
      }

      // send next frame invocation packet
      Bridge::PipeMessage::Packet<Bridge::PipeMessage::ServerOnSendText> onSendTextPacket;
      onSendTextPacket;  // no data yet
      sharedStuff.pipe.sendStructure(onSendTextPacket);

      // receive completion notification
      Util::Buffer buffer;
      if(!sharedStuff.pipe.receive(buffer))
      {
        lastError = __FUNCTION__ ": receive completion packet failed";
        return false;
      }

      // parse packet
      Util::MemoryFrame packet = buffer.getMemory();
      int packetType;
      if(!packet.readTo(packetType))
      {
        lastError = __FUNCTION__ ": received packet too small";
        return false;
      }

      if(packetType != Bridge::PipeMessage::AgentOnSendTextDone::Id)
      {
        lastError = __FUNCTION__ ": unexpected packet type " + packetType;
        return false;
      }

      Bridge::PipeMessage::AgentOnSendTextDone completion;
      if(!packet.readTo(completion))
      {
        lastError = __FUNCTION__ ": completion packet too small";
        return false;
      }

      completion; // yet no data here

      return true;
    }
    //-------------------------- IS AGENT CONNECTED ---------------------------------------------
    bool isAgentConnected()
    {
      return stateConnectionEstablished;
    }
    //-------------------------- ----------------------------------------------------------------
  }
}
