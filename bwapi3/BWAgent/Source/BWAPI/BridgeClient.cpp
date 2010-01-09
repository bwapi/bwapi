#include "BridgeClient.h"

#include <Bridge\PipeMessage.h>
#include <Bridge\Constants.h>
#include <Bridge\DrawShape.h>
#include <Bridge\EventEntry.h>
#include <Bridge\CommandEntry.h>
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

  //public:
    //----------------------------------------- PUBLIC DATA -----------------------------------------------------
    // public access to shared memory
    BWAPI::StaticGameData* gameData;
    BWAPI::DynamicGameData dynamicData;

    // additional data for RPC states
    bool isMatchStartFromBeginning = false;
    //----------------------------------------- CONNECT ---------------------------------------------------------
    bool connect()
    {
      if(connectionEstablished)
        throw GeneralException(__FUNCTION__ ": Already connected");

      // try to connect
      if(!sharedStuff.pipe.connect(Bridge::globalPipeName))
      {
        return false;
      }

      // receive handshake
      {
        Util::Buffer data;
        sharedStuff.pipe.receive(data);

        Bridge::PipeMessage::ServerHandshake handshake;
        data.getMemory().readTo(handshake);

        // check if this agent is same version
        if(handshake.serverVersion != SVN_REV)
        {
          sharedStuff.pipe.disconnect();
          throw GeneralException(__FUNCTION__ ": BWAPI.dll("
            + Util::Strings::intToString(handshake.serverVersion)
            + ") and BWAgent.dll(" SVN_REV_STR ") version mismatch");
        }
      }

      // handshake successfull

      try
      {
        // release everything, just to be sure
        sharedStuff.staticData.release();
        sharedStuff.commands.release();
        sharedStuff.events.release();

        // init agent-side dynamic memory
        sharedStuff.commands.init(4000, true);
        sharedStuff.drawShapes.init(4000, true);

        // send handshake packet (this handshake is part of the protocoll)
        {
          Bridge::PipeMessage::AgentHandshake handshake;

          // transmit process id
          handshake.agentProcessId = ::GetCurrentProcessId();
          sharedStuff.pipe.sendRawStructure(handshake);
        }

        // receive the initialisation packet
        {
          Util::Buffer data;
          sharedStuff.pipe.receive(data);

          Bridge::PipeMessage::ServerSharedMemoryInit init;
          data.getMemory().readTo(init);

          // import static data
          sharedStuff.staticData.import(init.staticGameDataExport);
          gameData = &sharedStuff.staticData.get();

          // store the process handle for shared memory handle duplication
          sharedStuff.remoteProcess.importHandle(init.serverProcessHandle);
        }

        // send the initialisation packet
        {
          Bridge::PipeMessage::AgentSharedMemoryInit init;
          sharedStuff.pipe.sendRawStructure(init);
        }
      }
      catch(GeneralException &e)
      {
        sharedStuff.pipe.disconnect();
        e.append("while attaching");
        throw;
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
        sharedStuff.commands.release();
        sharedStuff.drawShapes.release();
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
    void updateRemoteSharedMemory()
    {
      // TODO: DRY this code
      // export commands updates
      while(sharedStuff.commands.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::AgentUpdateCommands packet;
        try
        {
          sharedStuff.commands.exportNextUpdate(packet.exp, sharedStuff.remoteProcess);
        }
        catch(GeneralException &e)
        {
          e.append(__FUNCTION__ " exporting commands update");
          throw;
        }

        // send update export
        sharedStuff.pipe.sendRawStructure(packet);
      }

      // export drawShapes updates
      while(sharedStuff.drawShapes.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::AgentUpdateDrawShapes packet;
        try
        {
          sharedStuff.drawShapes.exportNextUpdate(packet.exp, sharedStuff.remoteProcess);
        }
        catch(GeneralException &e)
        {
          e.append(__FUNCTION__ " exporting drawShapes update");
          throw;
        }

        // send update export
        sharedStuff.pipe.sendRawStructure(packet);
      }
    }
    //----------------------------------------- EVENT ENTRY HANDLERS --------------------------------------------
    int sortEventKnownUnitAdd(Bridge::EventEntry::KnownUnitAdd& packet)
    {
      dynamicData.unitAddEvents.push_back(&packet.data);
      return 0;
    }
    int sortEventKnownUnitRemove(Bridge::EventEntry::KnownUnitRemove& packet)
    {
      dynamicData.unitRemoveEvents.push_back(&packet.data);
      return 0;
    }
    //----------------------------------------- PIPE PACKET HANDLERS --------------------------------------------
    bool handleUpdateEvents(Bridge::PipeMessage::ServerUpdateEvents& packet)
    {
      sharedStuff.events.importNextUpdate(packet.exp);
      return true;
    }
    bool handleMatchInit(Bridge::PipeMessage::ServerMatchInit& packet)
    {
      // save options
      isMatchStartFromBeginning = packet.fromBeginning;

      rpcState = OnInitMatch;

      // do not wait for next packet
      return false;
    }
    bool handleFrameNext(Bridge::PipeMessage::ServerFrameNext& packet)
    {
      // clear all outgoing frame-by-frame buffers
      sharedStuff.drawShapes.clear();
      sharedStuff.commands.clear();

      // sort incoming frame by frame buffers
      {
        // callback based sorting, int return value ignored
        static Util::TypedPacketSwitch<int> packetSwitch;
        if(!packetSwitch.getHandlerCount())
        {
          // init packet switch
          packetSwitch.addHandler(sortEventKnownUnitAdd);
          packetSwitch.addHandler(sortEventKnownUnitRemove);
        }

        // prepare arrays
        dynamicData.unitAddEvents.clear();
        dynamicData.unitRemoveEvents.clear();

        // fill arrays with references to event data
        for(Bridge::SharedStuff::EventStack::Index index = sharedStuff.events.begin();
            index.isValid();
            index = sharedStuff.events.getNext(index))
        {
          // put event into the right array (sort)
          packetSwitch.handlePacket(sharedStuff.events.get(index));
        }
      }

      rpcState = OnFrame;

      // do not wait for next packet
      return false;
    }
    //----------------------------------------- WAIT FOR EVENT --------------------------------------------------
    void waitForEvent()
    {
      // packet handlers have bool (wait for next packet) as return type
      static Util::TypedPacketSwitch<bool> packetSwitch;
      if(!packetSwitch.getHandlerCount())
      {
        // init packet switch
        packetSwitch.addHandler(handleUpdateEvents);
        packetSwitch.addHandler(handleMatchInit);
        packetSwitch.addHandler(handleFrameNext);
      }

      // send readyness packet
      switch(rpcState)
      {
      case Intermediate:
        {
        }break;
      case OnFrame:
        {
          // update remote dynamic memory
          updateRemoteSharedMemory();

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

      // repare data
      Util::MemoryFrame textMemory = Util::MemoryFrame((char*)string, strlen(string)+1);
      Bridge::CommandEntry::SendText head;
      head.printOnly = !send;

      // add a new empty packet
      Bridge::SharedStuff::CommandStack::Index index = sharedStuff.commands.insertBytes(sizeof(head) + textMemory.size());
      if(!index.isValid())
        return false;
      Util::MemoryFrame targetMemory = sharedStuff.commands.get(index);

      // write packet with data
      targetMemory.writeAs(head);
      targetMemory.write(textMemory);
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
    bool pushDrawText(Position pos, const char* text)
    {
      Bridge::DrawShape::Text packet;
      packet.pos = pos;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet), Util::MemoryFrame((void*)text, strlen(text)+1));
    }
    bool pushDrawRectangle(Position pos, Position size, int color, bool solid)
    {
      Bridge::DrawShape::Rectangle packet;
      packet.pos = pos;
      packet.size = size;
      packet.color = color;
      packet.isSolid = solid;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet));
    }
    bool pushDrawCircle(Position center, int r, int color, bool solid)
    {
      Bridge::DrawShape::Circle packet;
      packet.center = center;
      packet.radius = r;
      packet.color = color;
      packet.isSolid = solid;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet));
    }
    bool pushDrawLine(Position from, Position to, int color)
    {
      Bridge::DrawShape::Line packet;
      packet.from = from;
      packet.to = to;
      packet.color = color;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet));
    }
    bool pushDrawDot(Position pos, int color)
    {
      Bridge::DrawShape::Dot packet;
      packet.pos = pos;
      packet.color = color;
      return pushDrawShapePacket(Util::MemoryFrame::from(packet));
    }
    //----------------------------------------- -----------------------------------------------------------------
  }
}
