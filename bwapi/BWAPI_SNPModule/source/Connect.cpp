#include "Connect.h"
#include "Common.h"
#include "Threads.h"

DWORD gdwSendCalls;
DWORD gdwSendBytes;
DWORD gdwRecvCalls;
DWORD gdwRecvBytes;

SOCKET   gsBCSend;
SOCKET   gsBCRecv;
SOCKADDR gaddrBCSend;
SOCKADDR gaddrBCRecv;

SOCKET   gsBroadcast;
SOCKADDR gaddrBroadcast;
SOCKADDR gaddrBCFrom;

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
  gsBCSend    = MakeUDPSocket();
  gsBCRecv    = MakeUDPSocket();
  gsBroadcast = MakeUDPSocket();

  InitAddr(&gaddrBCRecv,    gdwProcId,         6111);
  InitAddr(&gaddrBCSend,    gdwProcId,         6111);
  InitAddr(&gaddrBCFrom,    gdwProcId,         6111);
  InitAddr(&gaddrBroadcast, "127.255.255.255", 6111);

  // bind the sockets
  bind(gsBCRecv,    &gaddrBCRecv, sizeof(SOCKADDR));
  bind(gsBCSend,    &gaddrBCSend, sizeof(SOCKADDR));
  bind(gsBroadcast, &gaddrBCFrom, sizeof(SOCKADDR));

  // begin recv threads here
  HANDLE hRecvThread = CreateThread(NULL, 0, &RecvThread, NULL, 0, NULL);
  if ( hRecvThread )
    SetThreadPriority(hRecvThread, 1);

  return true;
}

void DestroySockets()
{
  // do cleanup stuff
  if ( gsBCSend )
    closesocket(gsBCSend);
  if ( gsBCRecv )
    closesocket(gsBCRecv);
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

  DWORD dwTrue = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&dwTrue, sizeof(DWORD));
  setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&dwTrue, sizeof(DWORD));
  return s;
}

SOCKADDR *InitAddr(SOCKADDR *addr, const char *ip, WORD wPort)
{
  sockaddr_in *_addr = (sockaddr_in*)addr;
  memset(addr, 0, sizeof(SOCKADDR));
  _addr->sin_family           = AF_INET;
  _addr->sin_port             = htons(wPort);
  _addr->sin_addr.S_un.S_addr = inet_addr(ip);
  return addr;
}

SOCKADDR *InitAddr(SOCKADDR *addr, DWORD dwSeed, WORD wPort)
{
  sockaddr_in *_addr = (sockaddr_in*)addr;
  memset(addr, 0, sizeof(SOCKADDR));
  _addr->sin_family                 = AF_INET;
  _addr->sin_port                   = htons(wPort);
  _addr->sin_addr.S_un.S_un_b.s_b1  = 127;
  _addr->sin_addr.S_un.S_un_b.s_b2  = (dwSeed >> 16) & 0xFF;
  _addr->sin_addr.S_un.S_un_b.s_b3  = (dwSeed >> 8)  & 0xFF;
  _addr->sin_addr.S_un.S_un_b.s_b4  = dwSeed & 0xFF;
  if ( _addr->sin_addr.S_un.S_un_b.s_b4 == 0 )
  {
    _addr->sin_addr.S_un.S_un_b.s_b4++;
    _addr->sin_addr.S_un.S_un_b.s_b2 |= 0x40;
  }
  else if ( _addr->sin_addr.S_un.S_un_b.s_b4 == 255 )
  {
    _addr->sin_addr.S_un.S_un_b.s_b4++;
    _addr->sin_addr.S_un.S_un_b.s_b2 |= 0x80;
  }
  return addr;
}
