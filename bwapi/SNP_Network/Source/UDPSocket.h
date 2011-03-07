#pragma once 

class UDPSocket;

#include <windows.h>
#include <winsock.h>

#include <Util\Exceptions.h>
#include <Util\MemoryFrame.h>

class TWSAInitializer
{
public:
	TWSAInitializer();
	~TWSAInitializer();

public:
	static HANDLE completion_port;
};

extern TWSAInitializer _init_wsa;

typedef sockaddr_in UDPAddr;

class UDPSocket
{

	// constructors
public:
	UDPSocket();
	~UDPSocket();

	// state
private:
	SOCKET _s;
  int _state;
  int _bound;

	// methods
public:
	void init();
	void release() throw();
  void bind(int port);
	void sendPacket(const UDPAddr &target, Util::MemoryFrame data);
	Util::MemoryFrame receivePacket(UDPAddr &target, Util::MemoryFrame dest);
	int getNextPacketSize();
  void setBlockingMode(bool block);
  int getState() const;
  int getBoundPort() const; // returns 0 if not bound

	// service
public:
	static sockaddr_in getLocalAddress();
	static sockaddr_in resolveDNS(const char* dns_name);
};
