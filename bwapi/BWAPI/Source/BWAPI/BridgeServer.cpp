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
    bool stateSharedMemoryInitialized = false;
    bool stateConnectionEstablished = false;
    bool stateIsInitialized = false;
    Bridge::SharedStuff sharedStuff;

    //error handling
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
        stateIsInitialized = false;
        return false;
      }
      stateIsInitialized = true;
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
        if(!sharedStuff.pipe.sendRawStructure(handshake2))
        {
          lastError = "Could not send pipe";
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
    //-------------------------- DISCONNECT -----------------------------------------------------
    void disconnect()
    {
      if(stateSharedMemoryInitialized)
      {
        releaseSharedMemory();
      }
      sharedStuff.pipe.disconnect();
      stateConnectionEstablished = false;
    }
    //-------------------------- CREATE SHARED MEMORY -------------------------------------------
    bool createSharedMemory()
    {
      // check prerequisites
      if(!stateConnectionEstablished)
      {
        lastError = __FUNCTION__ ": connection not established";
        return false;
      }
      resetError();
      stateSharedMemoryInitialized = false;

      // create and publish static data
      if(!sharedStuff.staticData.create())
      {
        lastError = __FUNCTION__ ": staticData creating failed";
        return false;
      }
      sharedStaticData = &sharedStuff.staticData.get();

      // init dynamic objects
      sharedStuff.userInput.init(1000, true);

      stateSharedMemoryInitialized = true;
      return true;
    }

    //-------------------------- STOP MATCH -----------------------------------------------------
    bool releaseSharedMemory()
    {
      stateSharedMemoryInitialized = false;
      return true;
    }
    //-------------------------- INVOKE ON START MATCH ------------------------------------------
    bool invokeOnStartMatch(bool fromBeginning)
    {
      // check prerequisites
      if(!stateConnectionEstablished)
      {
        lastError = __FUNCTION__ ": connection not established";
        return false;
      }
      resetError();

      // send onStartMatch event
      {
        // create packet
        Bridge::PipeMessage::ServerMatchInit startMatchEvent;

        startMatchEvent.fromBeginning = fromBeginning;

        startMatchEvent.staticGameDataExport = sharedStuff.staticData.exportToProcess(sharedStuff.remoteProcess, true);
        if(!startMatchEvent.staticGameDataExport.isValid())
        {
          lastError = __FUNCTION__ ": staticData export failed";
          return false;
        }

        // pushlish the shared memory location
        if(!sharedStuff.pipe.sendRawStructure(startMatchEvent))
        {
          lastError = __FUNCTION__ ": error sending packet";
          return false;
        }
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
      int packetType = packet.getAs<int>();
      if(packetType != Bridge::PipeMessage::AgentMatchInitDone::_typeId)
      {
        lastError = __FUNCTION__ ": received unexpected packet type " + packetType;
        return false;
      }

      Bridge::PipeMessage::AgentMatchInitDone initMatchDone;
      if(!packet.readTo(initMatchDone))
      {
        lastError = __FUNCTION__ ": received packet too small";
        return false;
      }

      initMatchDone;  // yet no data to read

      stateSharedMemoryInitialized = true;
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
      Bridge::PipeMessage::ServerFrameNext nextFrame;
      nextFrame; // no data yet
      sharedStuff.pipe.sendRawStructure(nextFrame);

      // receive completion notification
      Util::Buffer buffer;
      if(!sharedStuff.pipe.receive(buffer))
      {
        lastError = __FUNCTION__ ": receive completion packet failed";
        return false;
      }

      // parse packet
      Util::MemoryFrame packet = buffer.getMemory();
      int packetType = packet.getAs<int>();


      if(packetType != Bridge::PipeMessage::AgentFrameNextDone::_typeId)
      {
        lastError = __FUNCTION__ ": unexpected packet type " + packetType;
        return false;
      }

      Bridge::PipeMessage::AgentFrameNextDone completion;
      if(!packet.readTo(completion))
      {
        lastError = __FUNCTION__ ": completion packet too small";
        return false;
      }

      completion; // yet no data here

      return true;
    }
    //------------------------------ ON SEND TEXT ------------------------------------------------
    bool pushSendText()
    {
      // check prerequisites
      if(!stateSharedMemoryInitialized)
      {
        lastError = __FUNCTION__ ": shared memory not initialized";
        return false;
      }

//      sharedStuff.

      return true;
    }
    //-------------------------- UPDATE REMOTE SHARED MEMORY ------------------------------------
    bool updateRemoteSharedMemory()
    {
      // export userInput updates
      while(sharedStuff.userInput.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::ServerUpdateUserInput packet;
        if(!sharedStuff.userInput.exportNextUpdate(packet.exp, sharedStuff.remoteProcess))
        {
          lastError = __FUNCTION__ ": update exporting userInput failed";
          return false;
        }

        // send update export
        sharedStuff.pipe.sendRawStructure(packet);
      }
      return true;
    }
    //-------------------------- IS AGENT CONNECTED ---------------------------------------------
    bool isAgentConnected()
    {
      return stateConnectionEstablished;
    }
    //-------------------------- IS BRIDGE INITIALIZED ------------------------------------------
    bool isBridgeInitialized()
    {
      return stateIsInitialized;
    }
    //-------------------------- IS SHARED MEMORY INITIALIZED -----------------------------------
    bool isSharedMemoryInitialized()
    {
      return stateSharedMemoryInitialized;
    }
    //-------------------------- ----------------------------------------------------------------
  }
}
