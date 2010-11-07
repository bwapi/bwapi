#include "Connect.h"
#include "Common.h"
#include "Threads.h"

namespace LUDP
{
  char gszThisIP[16];

  SOCKET      gsSend;
  SOCKET      gsRecv;
  SOCKADDR_IN gaddrSend;
  SOCKADDR_IN gaddrRecv;

  SOCKET      gsBroadcast;
  SOCKADDR_IN gaddrBroadcast;
  SOCKADDR_IN gaddrBCFrom;

  int SendData(SOCKET s, const char *buf, int len, const SOCKADDR_IN *to)
  {
    int rval = sendto(s, buf, len, 0, (SOCKADDR*)to, sizeof(SOCKADDR));
    ++gdwSendCalls;
    gdwSendBytes += len;

    if ( rval == SOCKET_ERROR )
    {
      DWORD dwErr = WSAGetLastError();
      char source[16];

      SOCKADDR_IN name;
      int namelen = sizeof(SOCKADDR);
      getsockname(gsSend, (SOCKADDR*)&name, &namelen);

      SStrCopy(source, inet_ntoa(name.sin_addr), 16);
      Error(dwErr, "Unable to send data: %s->%s", source, inet_ntoa(to->sin_addr));
    }
    return rval;
  }

  bool InitializeSockets()
  {
    // do initialization stuff
    WSADATA wsaData;
    DWORD dwErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if ( dwErr != ERROR_SUCCESS )
    {
      Error(dwErr, "WSAStartup failed.");
      return false;
    }

    // create sockets
    gsSend      = MakeUDPSocket();
    gsRecv      = MakeUDPSocket();
    gsBroadcast = MakeUDPSocket();

    InitAddr(&gaddrRecv,      gdwProcId,         6112);
    InitAddr(&gaddrSend,      gdwProcId,         6112);
    InitAddr(&gaddrBCFrom,    gdwProcId,         6112);
    InitAddr(&gaddrBroadcast, "127.255.255.255", 6112);

    SStrCopy(gszThisIP, inet_ntoa(gaddrSend.sin_addr), 16);

    // bind the sockets
    if ( bind(gsRecv,      (SOCKADDR*)&gaddrRecv,   sizeof(SOCKADDR)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "Unable to bind recv socket.");
    if ( bind(gsSend,      (SOCKADDR*)&gaddrSend,   sizeof(SOCKADDR)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "Unable to bind send socket.");
    if ( bind(gsBroadcast, (SOCKADDR*)&gaddrBCFrom, sizeof(SOCKADDR)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "Unable to bind broadcast socket.");

    // begin recv threads here
    HANDLE hRecvThread = CreateThread(NULL, 0, &RecvThread, NULL, 0, NULL);
    if ( !hRecvThread )
      Error(ERROR_INVALID_HANDLE, "Unable to create the recv thread.");
    SetThreadPriority(hRecvThread, 1);
    return true;
  }

  void DestroySockets()
  {
    // do cleanup stuff
    if ( gsSend )
      closesocket(gsSend);
    if ( gsRecv )
      closesocket(gsRecv);
    if ( gsBroadcast )
      closesocket(gsBroadcast);
    WSACleanup();
  }

  SOCKET MakeUDPSocket()
  {
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if ( s == INVALID_SOCKET )
    {
      Error(WSAGetLastError(), "A socket could not be created.");
      return NULL;
    }

    DWORD dwTrue  = 1;
    if ( setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&dwTrue, sizeof(DWORD)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "The socket option SO_REUSEADDR could not be set.");
    if ( setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&dwTrue, sizeof(DWORD)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "The socket option SO_BROADCAST could not be set.");
    return s;
  }

  SOCKADDR_IN *InitAddr(SOCKADDR_IN *addr, const char *ip, WORD wPort)
  {
    memset(addr, 0, sizeof(SOCKADDR));
    addr->sin_family           = AF_INET;
    addr->sin_port             = htons(wPort);
    addr->sin_addr.S_un.S_addr = inet_addr(ip);
    return addr;
  }

  SOCKADDR_IN *InitAddr(SOCKADDR_IN *addr, DWORD dwSeed, WORD wPort)
  {
    memset(addr, 0, sizeof(SOCKADDR));
    addr->sin_family                 = AF_INET;
    addr->sin_port                   = htons(wPort);
    addr->sin_addr.S_un.S_un_b.s_b1  = 127;
    addr->sin_addr.S_un.S_un_b.s_b2  = (dwSeed >> 16) & 0xFF;
    addr->sin_addr.S_un.S_un_b.s_b3  = (dwSeed >> 8)  & 0xFF;
    addr->sin_addr.S_un.S_un_b.s_b4  = dwSeed & 0xFF;
    if ( addr->sin_addr.S_un.S_un_b.s_b4 == 0 )
    {
      addr->sin_addr.S_un.S_un_b.s_b4++;
      addr->sin_addr.S_un.S_un_b.s_b2 |= 0x40;
    }
    else if ( addr->sin_addr.S_un.S_un_b.s_b4 == 255 )
    {
      addr->sin_addr.S_un.S_un_b.s_b4++;
      addr->sin_addr.S_un.S_un_b.s_b2 |= 0x80;
    }
    return addr;
  }

};

namespace LTST
{
  char gszThisIP[16];

  SOCKET      gsSend;
  SOCKET      gsRecv;
  SOCKADDR_IN gaddrSend;
  SOCKADDR_IN gaddrRecv;

  SOCKET      gsBroadcast;
  SOCKADDR_IN gaddrBroadcast;
  SOCKADDR_IN gaddrBCFrom;

  int SendData(SOCKET s, const char *buf, int len, const SOCKADDR_IN *to)
  {
    int rval = sendto(s, buf, len, 0, (SOCKADDR*)to, sizeof(SOCKADDR));
    ++gdwSendCalls;
    gdwSendBytes += len;

    if ( rval == SOCKET_ERROR )
    {
      DWORD dwErr = WSAGetLastError();
      char source[16];

      SOCKADDR_IN name;
      int namelen = sizeof(SOCKADDR);
      getsockname(gsSend, (SOCKADDR*)&name, &namelen);

      SStrCopy(source, inet_ntoa(name.sin_addr), 16);
      Error(dwErr, "Unable to send data: %s->%s", source, inet_ntoa(to->sin_addr));
    }
    return rval;
  }

  bool InitializeSockets()
  {
    // do initialization stuff
    WSADATA wsaData;
    DWORD dwErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if ( dwErr != ERROR_SUCCESS )
    {
      Error(dwErr, "WSAStartup failed.");
      return false;
    }

    // create sockets
    gsSend      = MakeUDPSocket();
    gsRecv      = MakeUDPSocket();
    gsBroadcast = MakeUDPSocket();

    InitAddr(&gaddrRecv,      gdwProcId,         6112);
    InitAddr(&gaddrSend,      gdwProcId,         6112);
    InitAddr(&gaddrBCFrom,    gdwProcId,         6112);
    InitAddr(&gaddrBroadcast, "127.255.255.255", 6112);

    SStrCopy(gszThisIP, inet_ntoa(gaddrSend.sin_addr), 16);

    // bind the sockets
    if ( bind(gsRecv,      (SOCKADDR*)&gaddrRecv,   sizeof(SOCKADDR)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "Unable to bind recv socket.");
    if ( bind(gsSend,      (SOCKADDR*)&gaddrSend,   sizeof(SOCKADDR)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "Unable to bind send socket.");
    if ( bind(gsBroadcast, (SOCKADDR*)&gaddrBCFrom, sizeof(SOCKADDR)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "Unable to bind broadcast socket.");

    // begin recv threads here
    HANDLE hRecvThread = CreateThread(NULL, 0, &RecvThread, NULL, 0, NULL);
    if ( !hRecvThread )
      Error(ERROR_INVALID_HANDLE, "Unable to create the recv thread.");
    SetThreadPriority(hRecvThread, 1);
    return true;
  }

  void DestroySockets()
  {
    // do cleanup stuff
    if ( gsSend )
      closesocket(gsSend);
    if ( gsRecv )
      closesocket(gsRecv);
    if ( gsBroadcast )
      closesocket(gsBroadcast);
    WSACleanup();
  }

  SOCKET MakeUDPSocket()
  {
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if ( s == INVALID_SOCKET )
    {
      Error(WSAGetLastError(), "A socket could not be created.");
      return NULL;
    }

    DWORD dwTrue  = 1;
    if ( setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&dwTrue, sizeof(DWORD)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "The socket option SO_REUSEADDR could not be set.");
    if ( setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&dwTrue, sizeof(DWORD)) == SOCKET_ERROR )
      Error(WSAGetLastError(), "The socket option SO_BROADCAST could not be set.");
    return s;
  }

  SOCKADDR_IN *InitAddr(SOCKADDR_IN *addr, const char *ip, WORD wPort)
  {
    memset(addr, 0, sizeof(SOCKADDR));
    addr->sin_family           = AF_INET;
    addr->sin_port             = htons(wPort);
    addr->sin_addr.S_un.S_addr = inet_addr(ip);
    return addr;
  }

  SOCKADDR_IN *InitAddr(SOCKADDR_IN *addr, DWORD dwSeed, WORD wPort)
  {
    memset(addr, 0, sizeof(SOCKADDR));
    addr->sin_family                 = AF_INET;
    addr->sin_port                   = htons(wPort);
    addr->sin_addr.S_un.S_un_b.s_b1  = 127;
    addr->sin_addr.S_un.S_un_b.s_b2  = (dwSeed >> 16) & 0xFF;
    addr->sin_addr.S_un.S_un_b.s_b3  = (dwSeed >> 8)  & 0xFF;
    addr->sin_addr.S_un.S_un_b.s_b4  = dwSeed & 0xFF;
    if ( addr->sin_addr.S_un.S_un_b.s_b4 == 0 )
    {
      addr->sin_addr.S_un.S_un_b.s_b4++;
      addr->sin_addr.S_un.S_un_b.s_b2 |= 0x40;
    }
    else if ( addr->sin_addr.S_un.S_un_b.s_b4 == 255 )
    {
      addr->sin_addr.S_un.S_un_b.s_b4++;
      addr->sin_addr.S_un.S_un_b.s_b2 |= 0x80;
    }
    return addr;
  }

};
