#include "BridgeClient.h"
#include "Mappings.h"

#include <Bridge\SharedStuff.h>
#include <Bridge\PipeMessage.h>
#include <Bridge\Constants.h>
#include <Bridge\Doodle.h>
#include <Util\Version.h>
#include <Util\RemoteProcess.h>
#include <Util\Pipe.h>
#include <Util\SharedMemory.h>
#include <Util\SharedSet.h>

namespace BWAgent
{
  // singleton class
  namespace BridgeClient
  {
  //private:
    Bridge::SharedStuff sharedStuff;

    // Bridge Client state
    bool connectionEstablished = false;
    bool sharedMemoryInitialized = false;
    RpcState rpcState = Intermediate;

    // error handling
    std::string lastError;
    void resetError()
    {
      lastError = "no error";
    }

  //public:
    //----------------------------------------- PUBLIC DATA -----------------------------------------------------
    // public access to shared memory
    Bridge::StaticGameData* sharedStaticData;
    //----------------------------------------- CONNECT ---------------------------------------------------------
    bool connect()
    {
      resetError();
      if(connectionEstablished)
      {
        lastError = __FUNCTION__ ": Already connected";
        return true;
      }

      // try to connect
      if(!sharedStuff.pipe.connect(Bridge::globalPipeName))
      {
        lastError = __FUNCTION__ ": Could not establish pipe connection. Check whether:\n - Broodwar is lauched with BWAPI\n - Another bot is already connected";
        return false;
      }

      // send handshake packet
      {
        Bridge::PipeMessage::AgentHandshake handshake;
        handshake.agentVersion = SVN_REV;
        handshake.agentProcessId = ::GetCurrentProcessId();
        if(!sharedStuff.pipe.sendRawStructure(handshake))
        {
          lastError = __FUNCTION__ ": Could not send per pipe";
          return false;
        }
      }

      // receive handshake
      {
        Util::Buffer data;
        if(!sharedStuff.pipe.receive(data))
        {
          lastError = __FUNCTION__ ": Could not read pipe";
          return false;
        }
        Bridge::PipeMessage::ServerHandshake handshake;
        if(!data.getMemory().readTo(handshake))
        {
          lastError = __FUNCTION__ ": Handshake failure";
          return false;
        }
        if(!handshake.accepted)
        {
          if(handshake.serverVersion != SVN_REV)
          {
            lastError = __FUNCTION__ ": BWAPI.dll(";
            lastError += handshake.serverVersion;
            lastError += ") and BWAgent.dll(" SVN_REV_STR ") version mismatch";
            return false;
          }
          lastError = __FUNCTION__ ": BWAPI rejected the connection";
          return false;
        }
        // try access the process for handle duplication
        if(!sharedStuff.remoteProcess.importHandle(handshake.serverProcessHandle))
        {
          lastError = __FUNCTION__ ": imported faulty process handle";
          return false;
        }
      }

      // acknoledge handshake
      {
        Bridge::PipeMessage::AgentHandshakeAcknoledge ack;
        ack.accepted = true;
        if(!sharedStuff.pipe.sendRawStructure(ack))
        {
          lastError = __FUNCTION__ ": Sending AgentHandshakeAcknoledge failed";
          return false;
        }
      }

      // connected
      connectionEstablished = true;

      return true;
    }
    //----------------------------------------- DISCONNECT ------------------------------------------------------
    void disconnect()
    {
      if(sharedMemoryInitialized)
      {
        sharedStuff.sendText.release();
      }
      sharedStuff.pipe.disconnect();
      connectionEstablished = true;
    }
    //----------------------------------------- GET CURRENT STATE -----------------------------------------------
    RpcState getCurrentRpc()
    {
      return rpcState;
    }

    //----------------------------------------- UPDATE REMOTE SHARED MEMORY -------------------------------------
    bool updateRemoteSharedMemory()
    {
      resetError();

      // export commands updates
      while(sharedStuff.commands.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::AgentUpdateCommands packet;
        if(!sharedStuff.commands.exportNextUpdate(packet.exp, sharedStuff.remoteProcess))
        {
          lastError = __FUNCTION__ ": exporting commands update failed";
          return false;
        }

        // send update export
        if(!sharedStuff.pipe.sendRawStructure(packet))
        {
          lastError = __FUNCTION__ ": sending commands update export packet failed";
          return false;
        }
      }

      // export sendText updates
      while(sharedStuff.sendText.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::AgentUpdateSendText packet;
        if(!sharedStuff.sendText.exportNextUpdate(packet.exp, sharedStuff.remoteProcess))
        {
          lastError = __FUNCTION__ ": exporting sendText update failed";
          return false;
        }

        // send update export
        if(!sharedStuff.pipe.sendRawStructure(packet))
        {
          lastError = __FUNCTION__ ": sending sendText update export packet failed";
          return false;
        }
      }

      return true;
    }
    //----------------------------------------- WAIT FOR EVENT --------------------------------------------------
    bool waitForEvent()
    {
      resetError();

      // send readyness packet
      bool success = false;
      switch(rpcState)
      {
      case Intermediate:
        {
          success = true;
        }break;
      case OnFrame:
        {
          // update remote dynamic memory
          if(!updateRemoteSharedMemory())
            return false;

          // return RPC
          Bridge::PipeMessage::AgentFrameNextDone done;
          success = sharedStuff.pipe.sendRawStructure(done);
        }break;
      case OnInitMatch:
        {
          // return RPC
          Bridge::PipeMessage::AgentMatchInitDone done;
          success = sharedStuff.pipe.sendRawStructure(done);
          sharedMemoryInitialized = true;
        }break;
      }
      if(!success)
      {
        lastError = __FUNCTION__ ": error pipe send.";
        return false;
      }

      // wait and receive something
      while(true)
      {
        Util::Buffer buffer;
        if(!sharedStuff.pipe.receive(buffer))
        {
          lastError = __FUNCTION__ ": error pipe receive.";
          int lastWinError = (int)::GetLastError();
          return false;
        }

        // examine received packet
        Util::MemoryFrame bufferFrame = buffer.getMemory();
        int packetType = bufferFrame.getAs<int>();

        // update userInput
        if(packetType == Bridge::PipeMessage::ServerUpdateUserInput::_typeId)
        {
          Bridge::PipeMessage::ServerUpdateUserInput packet;
          if(!bufferFrame.readTo(packet))
          {
            lastError = __FUNCTION__ ": too small ServerUpdateUserInput packet.";
            return false;
          }
          if(!sharedStuff.userInput.importNextUpdate(packet.exp))
          {
            lastError = __FUNCTION__ ": could not import userInput update.";
            return false;
          }

          // wait for next packet
          continue;
        }

        // update knownUnits
        if(packetType == Bridge::PipeMessage::ServerUpdateKnownUnits::_typeId)
        {
          Bridge::PipeMessage::ServerUpdateKnownUnits packet;
          if(!bufferFrame.readTo(packet))
          {
            lastError = __FUNCTION__ ": too small ServerUpdateKnownUnits packet.";
            return false;
          }
          if(!sharedStuff.knownUnits.importNextUpdate(packet.exp))
          {
            lastError = __FUNCTION__ ": could not import knownUnits update.";
            return false;
          }

          // wait for next packet
          continue;
        }

        // explicit events
        if(packetType == Bridge::PipeMessage::ServerMatchInit::_typeId)
        {
          // onInitMatch state
          Bridge::PipeMessage::ServerMatchInit packet;
          if(!bufferFrame.readTo(packet))
          {
            lastError = __FUNCTION__ ": too small ServerMatchInit packet.";
            return false;
          }

          // release everything, just to be sure
          sharedStuff.staticData.release();
          sharedStuff.commands.release();
          sharedStuff.userInput.release();
          sharedStuff.knownUnits.release();

          // init agent-side dynamic memory
          if(!sharedStuff.commands.init(1000, true))
          {
            lastError = __FUNCTION__ ": commands failed initializing.";
            return false;
          }
          if(!sharedStuff.sendText.init(2000, true))
          {
            lastError = __FUNCTION__ ": sendText failed initializing.";
            return false;
          }

          // import static data. It's all combined into staticData
          if (!sharedStuff.staticData.import(packet.staticGameDataExport))
          {
            lastError = __FUNCTION__ ": staticGameData failed importing.";
            return false;
          }
          sharedStaticData = &sharedStuff.staticData.get();
          

          rpcState = OnInitMatch;
          // return
          break;
        }

        if(packetType == Bridge::PipeMessage::ServerFrameNext::_typeId)
        {
          // onFrame state

          // clear all frame-by-frame buffers
          sharedStuff.sendText.clear();

          rpcState = OnFrame;
          // return
          break;
        }

        // None catched
        lastError = __FUNCTION__ ": unknown packet type ";
        lastError += packetType;
        return false;
      }
      return true;
    }
    //----------------------------------------- GET USER INPUT STRINGS ------------------------------------------
    std::deque<std::string> getUserInputStrings()
    {
      std::deque<std::string> retval;
      Bridge::SharedStuff::UserInputStack::Index i = sharedStuff.userInput.begin();
      while(i.isValid())
      {
        retval.push_back(std::string(sharedStuff.userInput.get(i).beginAs<char>()));
        i = sharedStuff.userInput.getNext(i);
      }
      return retval;
    }
    //----------------------------------------- IS CONNECTED ----------------------------------------------------
    bool isConnected()
    {
      return connectionEstablished;
    }
    //----------------------------------------- IS SHARED MEMORY INITIALIZED ------------------------------------
    bool isSharedMemoryInitialized()
    {
      return sharedMemoryInitialized;
    }
    //----------------------------------------- PUSH SEND TEXT --------------------------------------------------
    bool pushSendText(bool send, char *string)
    {
      if(!connectionEstablished || !sharedMemoryInitialized)
        return false;
      Util::MemoryFrame textmem = Util::MemoryFrame(string, strlen(string)+1);
      Bridge::SharedStuff::SendTextStack::Index index = sharedStuff.sendText.insertBytes(sizeof(bool) + textmem.size());
      if(!index.isValid())
        return false;
      Util::MemoryFrame targetmem = sharedStuff.sendText.get(index);
      targetmem.writeAs<bool>(send);
      targetmem.write(textmem);
      return true;
    }
    //----------------------------------------- GET LAST ERROR --------------------------------------------------
    std::string getLastError()
    {
      return lastError;
    }
    //----------------------------------------- -----------------------------------------------------------------
  }
}
