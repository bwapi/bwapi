#include "BridgeClient.h"

#include <Bridge\PipeMessage.h>
#include <Bridge\Constants.h>
#include <Bridge\DrawShape.h>
#include <Util\Version.h>
#include <Util\Strings.h>
#include <Util\RemoteProcess.h>
#include <Util\Pipe.h>
#include <Util\SharedMemory.h>
#include <Util\SharedSet.h>
#include <Util\TypedPacketSwitch.h>

namespace BWAPI
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
    BWAPI::StaticGameData* sharedStaticData;

    // additional data for RPC states
    bool isMatchStartFromBeginning = false;
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
        sharedStuff.pipe.sendRawStructure(handshake);
      }

      // receive handshake
      {
        Util::Buffer data;
        sharedStuff.pipe.receive(data);

        Bridge::PipeMessage::ServerHandshake handshake;
        data.getMemory().readTo(handshake);
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
        sharedStuff.pipe.sendRawStructure(ack);
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
      // TODO: DRY this code
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
        sharedStuff.pipe.sendRawStructure(packet);
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
        sharedStuff.pipe.sendRawStructure(packet);
      }

      // export drawShapes updates
      while(sharedStuff.drawShapes.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::AgentUpdateDrawShapes packet;
        if(!sharedStuff.drawShapes.exportNextUpdate(packet.exp, sharedStuff.remoteProcess))
        {
          lastError = __FUNCTION__ ": exporting drawShapes update failed";
          return false;
        }

        // send update export
        sharedStuff.pipe.sendRawStructure(packet);
      }

      return true;
    }
    //----------------------------------------- UPDATE PACKET HANDLERS ------------------------------------------
    bool handleUpdateUserInput(Bridge::PipeMessage::ServerUpdateUserInput& packet)
    {
      sharedStuff.userInput.importNextUpdate(packet.exp);
      return true;
    }
    bool handleUpdateKnownUnits(Bridge::PipeMessage::ServerUpdateKnownUnits& packet)
    {
      sharedStuff.knownUnits.importNextUpdate(packet.exp);
      return true;
    }
    bool handleUpdateKnownUnitAddEvents(Bridge::PipeMessage::ServerUpdateKnownUnitAddEvents& packet)
    {
      sharedStuff.knownUnitAddEvents.importNextUpdate(packet.exp);
      return true;
    }
    bool handleUpdateKnownUnitRemoveEvents(Bridge::PipeMessage::ServerUpdateKnownUnitRemoveEvents& packet)
    {
      sharedStuff.knownUnitRemoveEvents.importNextUpdate(packet.exp);
      return true;
    }
    //----------------------------------------- MATCH INIT PACKET HANDLER ---------------------------------------
    bool handleMatchInit(Bridge::PipeMessage::ServerMatchInit& packet)
    {
      // save options
      isMatchStartFromBeginning = packet.fromBeginning;

      // release everything, just to be sure
      sharedStuff.staticData.release();
      sharedStuff.commands.release();
      sharedStuff.userInput.release();
      sharedStuff.knownUnits.release();
      sharedStuff.knownUnitAddEvents.release();
      sharedStuff.knownUnitRemoveEvents.release();

      // first import static data. It's all combined into staticData
      sharedStuff.staticData.import(packet.staticGameDataExport);
      sharedStaticData = &sharedStuff.staticData.get();

      // init agent-side dynamic memory
      sharedStuff.commands.init(1000, true);
      sharedStuff.sendText.init(2000, true);
      sharedStuff.drawShapes.init(3000, true);

      rpcState = OnInitMatch;

      // do not wait for next packet
      return false;
    }
    //----------------------------------------- NEXT FRAME PACKET HANDLER ---------------------------------------
    bool handleFrameNext(Bridge::PipeMessage::ServerFrameNext& packet)
    {
      // clear all frame-by-frame buffers
      sharedStuff.sendText.clear();
      sharedStuff.drawShapes.clear();
      sharedStuff.commands.clear();

      rpcState = OnFrame;

      // do not wait for next packet
      return false;
    }
    //----------------------------------------- WAIT FOR EVENT --------------------------------------------------
    bool waitForEvent()
    {
      // packet handlers have bool (wait for next packet) as return type
      static Util::TypedPacketSwitch<bool> packetSwitch;
      if(!packetSwitch.getHandlerCount())
      {
        // init packet switch
        packetSwitch.addHandler(handleUpdateUserInput);
        packetSwitch.addHandler(handleUpdateKnownUnits);
        packetSwitch.addHandler(handleUpdateKnownUnitAddEvents);
        packetSwitch.addHandler(handleUpdateKnownUnitRemoveEvents);
        packetSwitch.addHandler(handleMatchInit);
        packetSwitch.addHandler(handleFrameNext);
      }

      // error handling
      resetError();

      // send readyness packet
      switch(rpcState)
      {
      case Intermediate:
        {
        }break;
      case OnFrame:
        {
          // update remote dynamic memory
          if(!updateRemoteSharedMemory())
            return false;

          // return RPC
          Bridge::PipeMessage::AgentFrameNextDone done;
          sharedStuff.pipe.sendRawStructure(done);
        }break;
      case OnInitMatch:
        {
          // return RPC
          Bridge::PipeMessage::AgentMatchInitDone done;
          sharedStuff.pipe.sendRawStructure(done);
          sharedMemoryInitialized = true;
        }break;
      }

      // wait and receive something
      bool waitForNextPacket = true;
      while(waitForNextPacket)
      {
        Util::Buffer buffer;
        sharedStuff.pipe.receive(buffer);

        // handle received packet. throws unhandled packets automatically
        waitForNextPacket = packetSwitch.handlePacket(buffer.getMemory());
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
    bool pushSendText(bool send, const char *string)
    {
      // check prerequisites
      if(!connectionEstablished || !sharedMemoryInitialized)
        return false;

      // push next packet
      Util::MemoryFrame textmem = Util::MemoryFrame((char*)string, strlen(string)+1);
      Bridge::SharedStuff::SendTextStack::Index index = sharedStuff.sendText.insertBytes(sizeof(bool) + textmem.size());
      if(!index.isValid())
        return false;
      Util::MemoryFrame targetmem = sharedStuff.sendText.get(index);
      targetmem.writeAs<bool>(send);
      targetmem.write(textmem);
      return true;
    }
    //----------------------------------------- -----------------------------------------------------------------
    bool pushDrawShapePacket(Util::MemoryFrame packet, Util::MemoryFrame text = Util::MemoryFrame())
    {
      // check prerequisites
      if(!connectionEstablished || !sharedMemoryInitialized)
        return false;

      // push next packet
      Bridge::SharedStuff::DrawShapeStack::Index index = sharedStuff.drawShapes.insertBytes(packet.size() + text.size());
      if(!index.isValid())
        return false;
      Util::MemoryFrame targetmem = sharedStuff.drawShapes.get(index);
      targetmem.write(packet);
      targetmem.write(text);

      return true;
    }
    // TODO: change to pre-allocation
    bool pushDrawText(int x, int y, const char* text)
    {
      Bridge::DrawShape::Text packet;
      packet.pos.x = x;
      packet.pos.y = y;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet), Util::MemoryFrame((void*)text, strlen(text)+1));
    }
    bool pushDrawRectangle(int x, int y, int w, int h, int color, bool solid)
    {
      Bridge::DrawShape::Rectangle packet;
      packet.pos.x = x;
      packet.pos.y = y;
      packet.size.x = x;
      packet.size.y = y;
      packet.color = color;
      packet.isSolid = solid;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet));
    }
    bool pushDrawCircle(int x, int y, int r, int color, bool solid)
    {
      Bridge::DrawShape::Circle packet;
      packet.center.x = x;
      packet.center.y = y;
      packet.radius = r;
      packet.color = color;
      packet.isSolid = solid;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet));
    }
    bool pushDrawLine(int x, int y, int x2, int y2, int color)
    {
      Bridge::DrawShape::Line packet;
      packet.from.x = x;
      packet.from.y = y;
      packet.to.x = x2;
      packet.to.y = y2;
      packet.color = color;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet));
    }
    bool pushDrawDot(int x, int y, int color)
    {
      Bridge::DrawShape::Dot packet;
      packet.pos.x = x;
      packet.pos.y = y;
      packet.color = color;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet));
    }
    //----------------------------------------- GET LAST ERROR --------------------------------------------------
    std::string getLastError()
    {
      return lastError;
    }
    //----------------------------------------- -----------------------------------------------------------------
  }
}
