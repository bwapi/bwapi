#include "BridgeServer.h"

#include <Bridge\Constants.h>
#include <Bridge\PipeMessage.h>
#include <Bridge\EventEntry.h>

#include <Util\Version.h>
#include <Util\Strings.h>
#include <Util\TypedPacketSwitch.h>

namespace BWAPI
{
  // see it as a singleton class.
  namespace BridgeServer
  {
  //private:
    //-------------------------- PRIVATE VARIABLES ----------------------------------------------
    bool stateConnectionEstablished = false;
    bool stateIsInitialized = false;
    Bridge::SharedStuff sharedStuff;

    //-------------------------- ----------------------------------------------------------------
  //public:
    //-------------------------- PUBLIC DATA ----------------------------------------------------
    BWAPI::StaticGameData* gameData = NULL;

    std::deque<Bridge::CommandEntry::SendText*> sendTextEntries;
    std::deque<Bridge::CommandEntry::UnitOrder*> orderEntries;
    //-------------------------- INIT -----------------------------------------------------------
    void initConnectionServer()
    {
      sharedStuff.pipe.create(Bridge::globalPipeName);
      stateIsInitialized = true;
    }
    //-------------------------- ACCEPT INCOMING CONNECTIONS ------------------------------------
    void acceptIncomingConnections()
    {
      // prerequisites
      if(!stateIsInitialized)
        return;
      if(stateConnectionEstablished)
        return;

      // check for incoming pipe connections
      if(!sharedStuff.pipe.pollIncomingConnection())
        return;

      // create static data
      {
        sharedStuff.staticData.create();
        gameData = &sharedStuff.staticData.get();
      }

      // send handshake
      {
        Bridge::PipeMessage::ServerHandshake handshake;
        handshake.serverVersion = SVN_REV;
        sharedStuff.pipe.sendRawStructure(handshake);
      }

      // receive agent's handshake
      Bridge::PipeMessage::AgentHandshake handshake2;
      {
        Util::Buffer data;
        sharedStuff.pipe.receive(data);
        data.getMemory().readTo(handshake2);
      }

      // acquire agent's process handle
      if(!sharedStuff.remoteProcess.acquire(handshake2.agentProcessId, true))
      {
        throw GeneralException("Could not open agent's process, rejected");
      }

      {
        Bridge::PipeMessage::ServerSharedMemoryInit init;

        // send a handle to this process, so the agent can share it's handles
        init.serverProcessHandle = sharedStuff.remoteProcess.exportOwnHandle();

        // synchronise the static data structure
        init.staticGameDataExport = sharedStuff.staticData.exportToProcess(sharedStuff.remoteProcess, true);

        sharedStuff.pipe.sendRawStructure(init);
      }

      // wait for init response
      Bridge::PipeMessage::AgentSharedMemoryInit init2;
      {
        Util::Buffer data;
        sharedStuff.pipe.receive(data);
        data.getMemory().readTo(init2);
      }

      // initialize the dynamic shared memory
      try
      {
        // reset dynamic objects
        sharedStuff.events.init(4000, true);
        sharedStuff.commands.release();

      }
      catch(GeneralException &e)
      {
        e.append("init shared memory");
        throw;
      }

      // agent successfully attached
      stateConnectionEstablished = true;
      return;
    }
    //-------------------------- DISCONNECT -----------------------------------------------------
    void disconnect()
    {
      if(!stateConnectionEstablished)
        return;


      // TODO: release shared memory here

      sharedStuff.pipe.disconnect();
      sharedStuff.events.release();
      sharedStuff.commands.release();
      stateConnectionEstablished = false;
    }
    //-------------------------- INVOKE ON START MATCH ------------------------------------------
    void invokeOnStartMatch(bool fromBeginning)
    {
      // check prerequisites
      if(!stateConnectionEstablished)
      {
        throw GeneralException(__FUNCTION__ ": connection not established");
      }

      // send onStartMatch event
      {
        // create packet
        Bridge::PipeMessage::ServerMatchInit startMatchEvent;

        startMatchEvent.fromBeginning = fromBeginning;

        // pushlish the shared memory location
        sharedStuff.pipe.sendRawStructure(startMatchEvent);
      }

      // wait untill event is done
      Util::Buffer buffer;
      sharedStuff.pipe.receive(buffer);

      // audit completion
      Util::MemoryFrame packet = buffer.getMemory();
      int packetType = packet.getAs<int>();
      if(packetType != Bridge::PipeMessage::AgentMatchInitDone::_typeId)
      {
        throw GeneralException(Util::Strings::ssprintf(__FUNCTION__ ": received unexpected packet type '%d'", packetType));
      }

      Bridge::PipeMessage::AgentMatchInitDone initMatchDone;
      packet.readTo(initMatchDone);

      initMatchDone;  // yet no data to read
    }
    //-------------------------- UPDATE PACKET HANDLERS -----------------------------------------
    bool handleUpdateCommands(Bridge::PipeMessage::AgentUpdateCommands& packet)
    {
      sharedStuff.commands.importNextUpdate(packet.exp);
      return true;
    }
    bool handleUpdateDrawShapes(Bridge::PipeMessage::AgentUpdateDrawShapes& packet)
    {
      sharedStuff.drawShapes.importNextUpdate(packet.exp);
      return true;
    }
    //-------------------------- COMMAND ENTRY HANDLERS -----------------------------------------
    int sortCommandSendText(Bridge::CommandEntry::SendText& packet, Util::MemoryFrame dynamicData)
    {
      // check for correctness of received c string
      if(dynamicData.endAs<char>()[-1] != 0)
        throw GeneralException("received CommandEntry::SendText's text data is not null terminated");

      sendTextEntries.push_back(&packet);
      return 0;
    }
    int sortCommandOrderUnit(Bridge::CommandEntry::UnitOrder& packet)
    {
      orderEntries.push_back(&packet);
      return 0;
    }
    //-------------------------- NEXT FRAME COMPLETION PACKET HANDLER ---------------------------
    bool handleFrameNextDone(Bridge::PipeMessage::AgentFrameNextDone& packet)
    {
      // iterate over commands that were issued this frame
      // sort them into the correspondant command entry arrays
      {
        // callback based sorting, int return value ignored
        static Util::TypedPacketSwitch<int> packetSwitch;
        if(!packetSwitch.getHandlerCount())
        {
          // init packet switch
          packetSwitch.addHandler(sortCommandSendText);
          packetSwitch.addHandler(sortCommandOrderUnit);
        }

        // prepare arrays
        sendTextEntries.clear();
        orderEntries.clear();

        for(Bridge::SharedStuff::CommandStack::Index index = sharedStuff.commands.begin();
            index.isValid();
            index = sharedStuff.commands.getNext(index))
        {
          packetSwitch.handlePacket(sharedStuff.commands.get(index));
        }
      }

      return false;
    }
    //-------------------------- INVOKE ON FRAME ------------------------------------------------
    void invokeOnFrame()
    {
      // packet handlers have bool (wait for next packet) as return type
      static Util::TypedPacketSwitch<bool> packetSwitch;
      if(!packetSwitch.getHandlerCount())
      {
        // init packet switch
        packetSwitch.addHandler(handleUpdateCommands);
        packetSwitch.addHandler(handleUpdateDrawShapes);
        packetSwitch.addHandler(handleFrameNextDone);
      }

      // check prerequisites
      if(!stateConnectionEstablished)
        throw GeneralException(__FUNCTION__ ": no connection");

      // send next frame invocation packet
      Bridge::PipeMessage::ServerFrameNext nextFrame;
      nextFrame; // no data yet
      sharedStuff.pipe.sendRawStructure(nextFrame);

      // wait untill completion packet received
      bool waitForNextPacket = true;
      while(waitForNextPacket)
      {
        // receive completion notification
        Util::Buffer buffer;
        sharedStuff.pipe.receive(buffer);

        // get packet type
        waitForNextPacket = packetSwitch.handlePacket(buffer.getMemory());
      }

      // just returned from frame invokation
      // clear frame-to-frame buffers
      sharedStuff.events.clear();
    }
    //------------------------------ ADD KNOWN UNIT ----------------------------------------------
    int addKnownUnit(KnownUnit **out_pKnownUnit, UnitAddEventTypeId reason)
    {
      // check prerequisites
      if(!stateConnectionEstablished)
        throw GeneralException(__FUNCTION__ ": connection required");

      // insert new known unit to set
      int index = gameData->units.findEmptySlot();
      KnownUnit *knownUnit = &gameData->units.allocate(index);
      if(out_pKnownUnit)
        *out_pKnownUnit = knownUnit;

      // push known unit event
      Bridge::EventEntry::KnownUnitAdd entry;
      entry.data.unitId = index;
      entry.data.type = reason;
      sharedStuff.events.insert(Util::MemoryFrame::from(entry));

      return index;
    }
    //------------------------------ REMOVE KNOWN UNIT -------------------------------------------
    void removeKnownUnit(int index, UnitRemoveEventTypeId reason)
    {
      // check prerequisites
      if(!stateConnectionEstablished)
        throw GeneralException(__FUNCTION__ ": shared memory not initialized");

      // remove known unit from set
      gameData->units.free(index);

      // push known unit event
      Bridge::EventEntry::KnownUnitRemove entry;
      entry.data.unitId = index;
      entry.data.type = reason;
      sharedStuff.events.insert(Util::MemoryFrame::from(entry));
    }
    //-------------------------- UPDATE REMOTE SHARED MEMORY ------------------------------------
    void updateRemoteSharedMemory()
    {
      // export events updates
      while(sharedStuff.events.isUpdateExportNeeded())
      {
        // create export package
        Bridge::PipeMessage::ServerUpdateEvents packet;
        try
        {
          sharedStuff.events.exportNextUpdate(packet.exp, sharedStuff.remoteProcess);
        }
        catch(GeneralException &e)
        {
          e.append(__FUNCTION__ " exporting events update");
          throw;
        }

        // send update export
        sharedStuff.pipe.sendRawStructure(packet);
      }
    }
    //-------------------------- GET ALL DRAW SHAPES --------------------------------------------
    void enumAllDrawShapes(DrawShapeCallback enumCallback)
    {
      if(!enumCallback)
        return;

      // iterate over all shapes
      Bridge::SharedStuff::DrawShapeStack::Index index = sharedStuff.drawShapes.begin();
      while(index.isValid())
      {
        // the sharedmemory is not visible outside of the BridgeServer
        enumCallback(sharedStuff.drawShapes.get(index));
        index = sharedStuff.drawShapes.getNext(index);
      }
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
    //-------------------------- ----------------------------------------------------------------
  }
}
