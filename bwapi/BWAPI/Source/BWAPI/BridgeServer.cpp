#include "BridgeServer.h"

#include <Bridge\Constants.h>
#include <Bridge\SharedStuff.h>
#include <Bridge\PipeMessage.h>

#include <Util\Version.h>
#include <Util\Strings.h>

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
    Bridge::StaticGameData* sharedStaticData = NULL;
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
        lastError = std::string(__FUNCTION__)+": connection not established";
        return false;
      }
      resetError();
      stateSharedMemoryInitialized = false;

      // create and publish static data
      if(!sharedStuff.staticData.create())
      {
        lastError = std::string(__FUNCTION__)+ ": staticData creating failed";
        return false;
      }
      sharedStaticData = &sharedStuff.staticData.get();

      // init dynamic objects
      if(!sharedStuff.userInput.init(1000, true))
      {
        lastError = std::string(__FUNCTION__)+ ": userInput creation failed";
        return false;
      }
      if(!sharedStuff.knownUnits.init(100, true))
      {
        lastError = std::string(__FUNCTION__)+ ": knownUnits creation failed";
        return false;
      }

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
        lastError = std::string(__FUNCTION__)+ ": connection not established";
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
          lastError = std::string(__FUNCTION__)+ ": staticData export failed";
          return false;
        }

        // pushlish the shared memory location
        if(!sharedStuff.pipe.sendRawStructure(startMatchEvent))
        {
          lastError = std::string(__FUNCTION__)+ ": error sending packet";
          return false;
        }
      }

      // wait untill event is done
      Util::Buffer buffer;
      if(!sharedStuff.pipe.receive(buffer))
      {
        lastError = std::string(__FUNCTION__)+ ": error receiving completion event";
        return false;
      }

      // audit completion
      Util::MemoryFrame packet = buffer.getMemory();
      int packetType = packet.getAs<int>();
      if(packetType != Bridge::PipeMessage::AgentMatchInitDone::_typeId)
      {
        lastError = std::string(__FUNCTION__)+ ": received unexpected packet type " + Util::Strings::intToString(packetType);
        return false;
      }

      Bridge::PipeMessage::AgentMatchInitDone initMatchDone;
      if(!packet.readTo(initMatchDone))
      {
        lastError = std::string(__FUNCTION__)+ ": received packet too small";
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
        lastError = std::string(__FUNCTION__)+ ": no connection established";
        return false;
      }

      // send next frame invocation packet
      Bridge::PipeMessage::ServerFrameNext nextFrame;
      nextFrame; // no data yet
      sharedStuff.pipe.sendRawStructure(nextFrame);

      // wait untill completion packet received
      for(;;)
      {
        // receive completion notification
        Util::Buffer buffer;
        if(!sharedStuff.pipe.receive(buffer))
        {
          lastError = std::string(__FUNCTION__)+ ": receive completion packet failed";
          return false;
        }

        // get packet type
        Util::MemoryFrame bufferFrame = buffer.getMemory();
        int packetType = bufferFrame.getAs<int>();

        // it way be an update export packet
        if(packetType == Bridge::PipeMessage::AgentUpdateCommands::_typeId)
        {
          Bridge::PipeMessage::AgentUpdateCommands packet;
          if(!bufferFrame.readTo(packet))
          {
            lastError = __FUNCTION__ ": too small AgentUpdateCommands packet.";
            return false;
          }
          if(!sharedStuff.commands.importNextUpdate(packet.exp))
          {
            lastError = __FUNCTION__ ": could not import commands update.";
            return false;
          }

          // wait for next packet
          continue;
        }

        // it way be an update export packet
        if(packetType == Bridge::PipeMessage::AgentUpdateSendText::_typeId)
        {
          Bridge::PipeMessage::AgentUpdateSendText packet;
          if(!bufferFrame.readTo(packet))
          {
            lastError = __FUNCTION__ ": too small AgentUpdateSendText packet.";
            return false;
          }
          if(!sharedStuff.sendText.importNextUpdate(packet.exp))
          {
            lastError = __FUNCTION__ ": could not import sendText update.";
            return false;
          }

          // wait for next packet
          continue;
        }

        // if it's not the completion packet
        if(packetType != Bridge::PipeMessage::AgentFrameNextDone::_typeId)
        {
          lastError = std::string(__FUNCTION__)+ ": unexpected packet type " + Util::Strings::intToString(packetType);
          return false;
        }

        // try parse as completion packet
        Bridge::PipeMessage::AgentFrameNextDone completion;
        if(!bufferFrame.readTo(completion))
        {
          lastError = std::string(__FUNCTION__)+ ": completion packet too small";
          return false;
        }

        completion; // yet no data here

        // sucessfully invoked
        break;
      }

      // clear frame-to-frame buffers
      sharedStuff.userInput.clear();

      return true;
    }
    //------------------------------ ON SEND TEXT ------------------------------------------------
    bool pushSendText(const char *text)
    {
      // check prerequisites
      if(!stateSharedMemoryInitialized)
      {
        lastError = std::string(__FUNCTION__)+ ": shared memory not initialized";
        return false;
      }

      Util::Buffer userInputEntry;
      int stringLength = ::strlen(text) + 1; // including terminal NULL
      userInputEntry.append(Util::MemoryFrame((char*)text, stringLength));
      sharedStuff.userInput.insert(userInputEntry.getMemory());

      return true;
    }
    //-------------------------- GET SEND TEXT ENTRIES ------------------------------------------
    std::deque<Bridge::SendTextEntry*> getSendTextEntries()
    {
      std::deque<Bridge::SendTextEntry*> retval;
      Bridge::SharedStuff::SendTextStack::Index i = sharedStuff.sendText.begin();
      while(i.isValid())
      {
        retval.push_back(sharedStuff.sendText.get(i).beginAs<Bridge::SendTextEntry>());
        i = sharedStuff.sendText.getNext(i);
      }
      return retval;
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
          lastError = std::string(__FUNCTION__)+ ": exporting userInput update failed";
          return false;
        }

        // send update export
        sharedStuff.pipe.sendRawStructure(packet);
      }

      // export knownUnits updates
      while(sharedStuff.knownUnits.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::ServerUpdateKnownUnits packet;
        if(!sharedStuff.knownUnits.exportNextUpdate(packet.exp, sharedStuff.remoteProcess))
        {
          lastError = std::string(__FUNCTION__)+ ": exporting knownUnits update failed";
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
