#pragma once
#include <windows.h>
#include <Winsock.h>

namespace LUDP
{
  extern SOCKET      gsSend;
  extern SOCKET      gsRecv;
  extern SOCKADDR_IN gaddrSend;
  extern SOCKADDR_IN gaddrRecv;
  extern SOCKADDR_IN gaddrBCFrom;

  extern char gszThisIP[16];

  extern SOCKET      gsBroadcast;
  extern SOCKADDR_IN gaddrBroadcast;

  int SendData(SOCKET s, const char *buf, int len, const SOCKADDR_IN *to);

  SOCKET   MakeUDPSocket();
  SOCKADDR_IN *InitAddr(SOCKADDR_IN *addr, const char *ip = "127.0.0.1", WORD wPort = 6112);
  SOCKADDR_IN *InitAddr(SOCKADDR_IN *addr, DWORD dwSeed, WORD wPort = 6112);

  bool InitializeSockets();
  void DestroySockets();

};

namespace LTST
{
  int SendData(const char *buf, int len, const SOCKADDR_IN *to);
};
