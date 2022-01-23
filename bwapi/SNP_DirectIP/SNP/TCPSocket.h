#pragma once 

class TCPSocket;

#include "Socket.h"

class TCPSocket : public Socket
{
  // constructors
public:
  TCPSocket();
  ~TCPSocket();

  // methods
public:
  void init() override;
  void release() throw() override;

  void bind(int port) override;
  //void connect(const Addr& target) override;
  //SocketMode getMode(void) const override;

  void sendPacket(const Addr& target, Util::MemoryFrame data) override;
  Util::MemoryFrame receivePacket(Addr &target, Util::MemoryFrame dest) override;
  int getNextPacketSize() override;
  void setBlockingMode(bool block) override;
};
