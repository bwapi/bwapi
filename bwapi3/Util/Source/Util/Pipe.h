#pragma once
////////////////////////////////////////////////////////////////////////
//  Message based inter process communication stream                  //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util { class Pipe; }

#define PIPE_TIMEOUT 2000
#define PIPE_SYSTEM_BUFFER_SIZE 4096

#include "Buffer.h"
#include "Exceptions.h"
#include "RemoteProcess.h"
#include "RemoteProcessId.h"
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#include <string>


namespace Util
{
  class Pipe
  {
  public:
    Pipe();
    ~Pipe();

    void create(std::string globalPipeName);    // create a server pipe (server)
    bool connect(std::string globalPipeName) throw();   // connect to a server pipe (client)
    void waitIncomingConnection();              // blocks, serverside (server)
    void disconnect();                          // disconnect and reuse
    void discard();                             // disconnect and remove

    void send(const MemoryFrame &in);           // blocks only if sent tooo much
    template<typename T>
      void sendRawStructure(const T &data)
      {
        send(MemoryFrame::from(data));
      }
    void receive(Buffer &out);                  // blocks if no message available
    void waitTillFlushed() const;               // blocks

    bool pollIncomingConnection();              // does not block. returns true and inits connection when incoming
    bool isError() const;
    bool isConnected() const;                   // does not initialize incoming connections, use pollIncomingConnection()
    bool isListening() const;
    bool isMessageIncoming() const;
//    RemoteProcessId getClientProcessId() const;

    template<typename T> void receiveOnlyAs(T &dest)
    {
      // receive
      Util::Buffer buffer;
      this->receive(buffer);
      Util::MemoryFrame mem = buffer.getMemory();

      // check size
      if(mem.size() != sizeof(T))
        throw GeneralException(__FUNCTION__ ": wrong packet size");
      
      // read data
      mem.readTo<T>(dest);
      return true;
    }

  private:
    HANDLE pipeObjectHandle;
    bool connected;
    bool listening;

    void _listen();
    void _setMode(bool blocking = true);
    static void _fixPipeName(std::string&);
  };
}

