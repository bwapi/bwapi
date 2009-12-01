#pragma once
////////////////////////////////////////////////////////////////////////
//  Message based inter process communication stream                  //
//                                                                    //
////////////////////////////////////////////////////////////////////////

namespace Util { class Pipe; }

#define PIPE_TIMEOUT 2000
#define PIPE_SYSTEM_BUFFER_SIZE 4096

#include "Buffer.h"
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

    bool create(std::string globalPipeName);    // create a server pipe (server)
    bool connect(std::string globalPipeName);   // connect to a server pipe (client)
    bool waitIncomingConnection();              // blocks, serverside (server)
    void disconnect();                          // disconnect and reuse
    void discard();                             // disconnect and remove

    bool send(const MemoryFrame &in);           // blocks only if sent tooo much
    template<typename T>
      bool sendStructure(const T &data)
      {
        return send(MemoryFrame::from(data));
      }
    bool receive(Buffer &out);                  // blocks if no message available
    bool waitTillFlushed() const;               // blocks

    bool pollIncomingConnection();              // does not block. returns true and inits connection when incoming
    bool isError() const;
    bool isConnected() const;                   // does not initialize incoming connections, use pollIncomingConnection()
    bool isListening() const;
    bool isMessageIncoming() const;
//    RemoteProcessId getClientProcessId() const;

  private:
    HANDLE pipeObjectHandle;
    bool connected;
    bool listening;

    bool _listen();
    bool _setMode(bool blocking = true);
    static void _fixPipeName(std::string&);
  };
}
