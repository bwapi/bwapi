#include "Pipe.h"

namespace Util
{
  //----------------------- CONSTRUCTION -------------------------------
  Pipe::Pipe()
    : pipeObjectHandle(INVALID_HANDLE_VALUE)
    , connected(false)
    , listening(false)  // yes, it's initialized in the error state
  {
  }

  Pipe::~Pipe()
  {
    this->discard();
  }
  //----------------------- CREATE -------------------------------------
  bool Pipe::create(std::string globalPipeName)
  {
    discard();  // just in case

    // next state for all except the last return
    this->connected = false;
    this->listening = false;

    Pipe::_fixPipeName(globalPipeName);

    this->pipeObjectHandle = ::CreateNamedPipeA(
      globalPipeName.c_str(),   // system wide pipe name
      PIPE_ACCESS_DUPLEX    |
      FILE_FLAG_FIRST_PIPE_INSTANCE,
      PIPE_TYPE_MESSAGE     |
      PIPE_READMODE_MESSAGE |
      PIPE_WAIT,
      1,                        // allow max 1 server with the given name
      PIPE_SYSTEM_BUFFER_SIZE,
      PIPE_SYSTEM_BUFFER_SIZE,
      PIPE_TIMEOUT,
      NULL);                    // no inheritance
    if(this->pipeObjectHandle == INVALID_HANDLE_VALUE)
    {
      return false;
    }

    this->connected = false;
    if(!this->_listen())
    {
      ::CloseHandle(this->pipeObjectHandle);
      return false;
    }

    // next state already set by _listen()
    return true;
  }
  //----------------------- CONNECT ------------------------------------
  bool Pipe::connect(std::string globalPipeName)
  {
    discard();    // just in case

    // next state for all except the last return
    this->connected = false;
    this->listening = false;

    Pipe::_fixPipeName(globalPipeName);

    this->pipeObjectHandle = ::CreateFileA( 
      globalPipeName.c_str(), // pipe name 
      GENERIC_READ |          // read and write access 
      GENERIC_WRITE, 
      0,                      // no sharing 
      NULL,                   // default security attributes
      OPEN_EXISTING,          // opens existing pipe 
      0,                      // default attributes 
      NULL);                  // no template file 

    if (this->pipeObjectHandle == INVALID_HANDLE_VALUE)
    {
      if (::GetLastError() == ERROR_PIPE_BUSY) 
      {
        // when the named pipe exists, but is already connected to
        // by another process. possible other error code
      }
      return false;
    }

    // With CreateFileA, the pipe is initially in the stream mode
    // needed to be set the correct mode before I/O
    bool success = this->_setMode();
    if (!success) 
    {
      return false;
    }

    // connected successfully
    this->connected = true;
    this->listening = false;
    return true;
  }
  //----------------------- DISCONNECT ---------------------------------
  void Pipe::disconnect()
  {
    DisconnectNamedPipe(this->pipeObjectHandle);
    this->connected = false;
    this->listening = false;

    // this call sets the next state for us
    _listen();
  }
  //----------------------- DISCARD ------------------------------------
  void Pipe::discard()
  {
    if(this->pipeObjectHandle != INVALID_HANDLE_VALUE)
    {
      ::CloseHandle(this->pipeObjectHandle);
    }
    this->pipeObjectHandle = INVALID_HANDLE_VALUE;
    this->connected = false;
    this->listening = false;
  }
  //----------------------- POLL FOR INCOMING CONNECTION ---------------
  bool Pipe::pollIncomingConnection()
  {
    if(this->connected)
    {
      return true;
    }
    if(!this->listening)
      return false;     // isError() case

    // only if listening
    // => already in non-blocking mode

    // poll connection state
    BOOL listening = ::ConnectNamedPipe(this->pipeObjectHandle, NULL);
    if(listening || GetLastError() == ERROR_PIPE_LISTENING)
    {
      // still listening
      return false;
    }
    if(::GetLastError() == ERROR_PIPE_CONNECTED)
    {
      // pipe finally connected
      this->connected = true;
      this->listening = false;
      // for I/O, pipe must be in blocking mode
      this->_setMode(true);
      return true;
    }
    // neither listening nor connected
    this->connected = false;
    this->listening = false;
    return false;
  }
  //----------------------- IS ERROR -----------------------------------
  bool Pipe::isError() const
  {
    // a healthy Pipe object is always either listening or connected
    return !listening && !connected;
  }
  //----------------------- IS CONNECTED -------------------------------
  bool Pipe::isConnected() const
  {
    return connected;
  }
  //----------------------- IS LISTENING -------------------------------
  bool Pipe::isListening() const
  {
    return listening;
  }
  //----------------------- IS MESSAGE AVAILABLE -----------------------
  bool Pipe::isMessageIncoming() const
  {
    DWORD availableByteCount;
    BOOL success = ::PeekNamedPipe(
      this->pipeObjectHandle,
      NULL,
      NULL,
      NULL,
      &availableByteCount,
      NULL);

    if(!success)
      return false;

    return availableByteCount > 0;
  }
  //----------------------- WAIT INCOMING CONNECTION -------------------
  bool Pipe::waitIncomingConnection()
  {
    // to wait, switch to blocking mode
    this->_setMode(true);
    BOOL success = ::ConnectNamedPipe(this->pipeObjectHandle, NULL);
    if(!success)
    {
      if(GetLastError() != ERROR_PIPE_CONNECTED)
      {
        // returned but not connected
        this->connected = false;
        this->listening = false;
        return false;
      }
      // pipe was already connected
    }
    this->connected = true;
    this->listening = false;
    return true;
  }
  //----------------------- SEND ---------------------------------------
  bool Pipe::send(const MemoryFrame &memory)
  {
/*    if(memory.isEmpty())
    {
      // sending NULL packets might produce ambiguous results
      return false;
    }*/
    DWORD writtenByteCount;
    BOOL success = ::WriteFile(
      this->pipeObjectHandle,
      memory.begin(),
      memory.size(),
      &writtenByteCount,
      NULL);

    if(!success)
    {
      return false;
    }

    if(writtenByteCount != memory.size())
    {
      return false;
    }

    return true;
  }
  //----------------------- RECEIVE ------------------------------------
  bool Pipe::receive(Buffer &out)
  {
    // wait for a message
    {
      DWORD provisoricBuffer;
      DWORD receivedByteCount;
      BOOL success = ::ReadFile(
        this->pipeObjectHandle,   // handle
        &provisoricBuffer,        // buffer start
        sizeof(DWORD),            // buffer size
        &receivedByteCount,       // out: received bytes
        NULL);
      
      out.release();
      out.append(Util::MemoryFrame::from(provisoricBuffer).getSubFrame(0, receivedByteCount));

      if(success)
      {
        // the provisoric DWORD was enough
        return true;
      }
    }

    if(::GetLastError() != ERROR_MORE_DATA)
    {
      return false;
    }

    // allocate the right space
    int middle = out.size();
    {
      DWORD availableByteCount;
      BOOL success = ::PeekNamedPipe(
        this->pipeObjectHandle,
        NULL,
        0,
        NULL,
        &availableByteCount,
        NULL);

      if(!success)
        return false;

      out.setSize(out.size() + availableByteCount);
    }

    // read message to memory
    {
      MemoryFrame dest = out.getMemory();
      dest = dest.getSubFrameByLimits(middle, dest.size());

      DWORD receivedByteCount;
      BOOL success = ::ReadFile(
        this->pipeObjectHandle,   // handle
        dest.begin(),             // buffer start
        dest.size(),              // buffer size
        &receivedByteCount,       // out: received bytes
        NULL);

      if(!success)
        return false;

      if(receivedByteCount != dest.size())
        return false;
    }

    return true;
  }
  //----------------------- WAIT TILL FLUSHED --------------------------
  bool Pipe::waitTillFlushed() const
  {
    BOOL success = ::FlushFileBuffers(this->pipeObjectHandle);
    if(!success)
    {
      return false;
    }
    return true;
  }
  //----------------------- CLIENT PROCESS ID --------------------------
/*  RemoteProcessId Pipe::getClientProcessId() const
  {
    // specifically DWORD
    // because later RemoteProcessId could become a structure
    DWORD clientProcessId;

    GetNamedPipeClientProcessId(
      this->pipeObjectHandle,
      &clientProcessId);

    return clientProcessId;
  }*/
  //----------------------- FIX PIPE NAME ------------------------------
  void Pipe::_fixPipeName(std::string &pipeName)
  {
    pipeName = "\\\\.\\pipe\\" + pipeName;
  }
  //----------------------- LISTEN -------------------------------------
  bool Pipe::_listen()
  {
    // while listening, use nonblocking mode to enable polling
    this->_setMode(false);

    BOOL listening = ::ConnectNamedPipe(this->pipeObjectHandle, NULL);
    if(listening || GetLastError() == ERROR_PIPE_LISTENING)
    {
      // listening
      this->connected = false;
      this->listening = true;
      return true;
    }
    if(GetLastError() == ERROR_PIPE_CONNECTED)
    {
      // pipe was already connected
      // happens between creation/recycling and _listen()
      this->connected = true;
      this->listening = false;
      // for I/O, pipe must be in blocking mode
      this->_setMode(true);
      return true;
    }
    // neither listening nor connected
    this->connected = false;
    this->listening = false;
    return false;
  }
  //----------------------- SET BLOCKING MODE --------------------------
  bool Pipe::_setMode(bool blocking)
  {
    DWORD dwMode = PIPE_READMODE_MESSAGE | ( blocking ? PIPE_WAIT : PIPE_NOWAIT ); 
    BOOL success = ::SetNamedPipeHandleState( 
      this->pipeObjectHandle,   // pipe handle 
      &dwMode,                  // new pipe mode 
      NULL,                     // don't set maximum bytes 
      NULL);                    // don't set maximum time 
    if (!success) 
    {
      return false;
    }
    return true;
  }
  //----------------------- --------------------------------------------
}
