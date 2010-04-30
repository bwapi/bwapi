#include "BWAPIClient.h"

namespace BWAPI
{
  Client BWAPIClient;
  Client::Client()
  {
    connected=false;
  }
  Client::~Client()
  {
    if (connected)
    {
      disconnect();
      connected=false;
    }
  }
  bool Client::isConnected()
  {
    return connected;
  }
  bool Client::connect()
  {
    if (this->connected) return true;
    this->pipeObjectHandle=CreateFileA("\\\\.\\pipe\\bwapi_pipe",GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
    COMMTIMEOUTS c;
    c.ReadIntervalTimeout = 100;
    c.ReadTotalTimeoutMultiplier = 100;
    c.ReadTotalTimeoutConstant = 2000;
    c.WriteTotalTimeoutMultiplier = 100;
    c.WriteTotalTimeoutConstant = 2000;
    if (this->pipeObjectHandle == INVALID_HANDLE_VALUE)
      return false;
    this->connected=true;
    SetCommTimeouts(this->pipeObjectHandle,&c);
    printf("Connected\n");
    return true;
  }
  void Client::disconnect()
  {
    if (!connected) return;
    CloseHandle(this->pipeObjectHandle);
    connected=false;
    printf("Disconnected\n");
  }
  void Client::update()
  {
    DWORD writtenByteCount;
    int code=4;
    WriteFile(this->pipeObjectHandle,&code,sizeof(int),&writtenByteCount,NULL);
    while (code!=3)
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(this->pipeObjectHandle,&code,sizeof(int),&receivedByteCount,NULL);
      if (!success)
      {
        disconnect();
        return;
      }
    }
  }
  /*
  // singleton class
  namespace Client
  {
  //private:

    BWAPIC::SharedStuff sharedStuff;

    // Bridge Client state
    bool connectionEstablished = false;
    RpcState rpcState = Intermediate;

  //public:
    //----------------------------------------- PUBLIC DATA -----------------------------------------------------
    // public access to shared memory
    BWAPIC::GameData* gameData;

    // additional data for RPC states
    bool isMatchStartFromBeginning = false;
    //----------------------------------------- CONNECT ---------------------------------------------------------
    bool connect()
    {
      printf("try to connect\n");
      CreateFile("\\.\pipe\bwapi_pipe",
      
      BOOL listening = ConnectNamedPipe(this->pipeObjectHandle, NULL);
      // try to connect

      if(!sharedStuff.pipe.connect(BWAPIC::globalPipeName))
      {
        return false;
      }
      printf("about to receive handshake\n");

      // receive handshake
      {
        Util::Buffer data;
        sharedStuff.pipe.receive(data);
        printf("received data\n");

        BWAPIC::PipeMessage::ServerHandshake handshake;
        data.getMemory().readTo(handshake);
        printf("put data into handshake object\n");

        // check if this agent is same version
        if(handshake.serverVersion != SVN_REV)
        {
          sharedStuff.pipe.disconnect();
          throw GeneralException(__FUNCTION__ ": BWAPI.dll("
            + Util::Strings::intToString(handshake.serverVersion)
            + ") and BWAgent.dll(" SVN_REV_STR ") version mismatch");
        }
        printf("checked server version\n");
      }
        printf("handshake successful\n");

      // handshake successfull

      try
      {
        printf("releasing data\n");
        // release everything, just to be sure
        sharedStuff.data.release();

        printf("sending hand shake packet\n");
        // send handshake packet (this handshake is part of the protocoll)
        {
          BWAPIC::PipeMessage::AgentHandshake handshake;

          // transmit process id
          handshake.agentProcessId = ::GetCurrentProcessId();
          sharedStuff.pipe.sendRawStructure(handshake);
        }
        printf("waiting for initialization packet\n");

        // receive the initialisation packet
        {
          Util::Buffer data;
          sharedStuff.pipe.receive(data);
          printf("received data\n");

          BWAPIC::PipeMessage::ServerSharedMemoryInit init;
          data.getMemory().readTo(init);
          printf("read data into sever shared memory init packet\n");

          // import static data
          sharedStuff.data.import(init.gameDataExport);
          gameData = &sharedStuff.data.get();
          printf("imported game data\n");

          // store the process handle for shared memory handle duplication
          sharedStuff.remoteProcess.importHandle(init.serverProcessHandle);
          printf("imported server process handle\n");
        }

        // send the initialisation packet
        {
          BWAPIC::PipeMessage::AgentSharedMemoryInit init;
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
      printf("connection Established\n");

      return true;
    }
    //----------------------------------------- DISCONNECT ------------------------------------------------------
    void disconnect()
    {
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
      /*
      // TODO: DRY this code
      // export commands updates
      while(sharedStuff.data.isUpdateExportNeeded())
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
      * /
    }
    //----------------------------------------- PIPE PACKET HANDLERS --------------------------------------------
    bool handleFrameNext(BWAPIC::PipeMessage::ServerFrameNext& packet)
    {
      rpcState = OnFrame;
      // do not wait for next packet
      return false;
    }
    //----------------------------------------- WAIT FOR EVENT --------------------------------------------------
    void update()
    {
      waitForEvent();
    }
    void waitForEvent()
    {
      printf("waitForEvent()");
      // packet handlers have bool (wait for next packet) as return type
      static Util::TypedPacketSwitch<bool> packetSwitch;
      if(!packetSwitch.getHandlerCount())
      {
        // init packet switch
        packetSwitch.addHandler(handleFrameNext);
      }

      printf("sending AgentFrameNextDone");

      // send readyness packet to complete previous waitForEvent call
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
          BWAPIC::PipeMessage::AgentFrameNextDone done;
          sharedStuff.pipe.sendRawStructure(done);
        }break;
      }
      printf("waiting for next packet");

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
    //----------------------------------------- -----------------------------------------------------------------
  }
  */
}
