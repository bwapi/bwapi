#pragma once 

class Socket;

#include <windows.h>
#include <winsock.h>

#include <Util/Exceptions.h>
#include <Util/MemoryFrame.h>

class TWSAInitializer
{
public:
  TWSAInitializer();
  ~TWSAInitializer();

public:
  static HANDLE completion_port;
};

extern TWSAInitializer _init_wsa;

typedef sockaddr_in Addr;

enum SocketMode { CLIENT, SERVER, BOTH };

class Socket
{
  // constructors
public:
  Socket();
  virtual ~Socket();

  // state
protected:
  SOCKET _s;
  int _state;
  int _bound;
  Addr target;

  // methods
public:
  virtual void init() = 0;
  virtual void release() throw() = 0;
  
  virtual void bind(int port) = 0;
  //virtual void connect(const Addr& target) = 0;

  virtual void sendPacket(const Addr &target, Util::MemoryFrame data) = 0;
  virtual Util::MemoryFrame receivePacket(Addr &target, Util::MemoryFrame dest) = 0;
  
  
  virtual int getNextPacketSize() = 0;
  virtual void setBlockingMode(bool block) = 0;

  //virtual SocketMode getMode(void) const = 0;
  
  int getState() const;
  int getBoundPort() const; // returns 0 if not bound


  // service
public:
  static sockaddr_in getLocalAddress();
  static sockaddr_in resolveDNS(const char* dns_name);
};
