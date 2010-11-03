#include "Connect.h"
#include "Common.h"
#include "Threads.h"

DWORD gdwSendCalls;
DWORD gdwSendBytes;
DWORD gdwRecvCalls;
DWORD gdwRecvBytes;

SOCKET   gsRecv;
SOCKADDR gaddrRecv;

SOCKET   gsBCRecv;
SOCKADDR gaddrBCRecv;

SOCKET   gsSend;

SOCKET   gsBroadcast;
SOCKADDR gaddrBroadcast;

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

  gsSend      = MakeUDPSocket();
  gsRecv      = MakeUDPSocket();
  gsBCRecv    = MakeUDPSocket();
  gsBroadcast = MakeUDPSocket();

  InitAddr(&gaddrRecv,      "127.0.0.1",       6112);
  InitAddr(&gaddrBCRecv,    "127.0.0.1",       6111);
  InitAddr(&gaddrBroadcast, "127.255.255.255", 6111);

  // begin recv threads here
  HANDLE hBroadcastThread = CreateThread(NULL, 0, &BroadcastThread, NULL, 0, NULL);
  if ( hBroadcastThread )
    SetThreadPriority(hBroadcastThread, 1);

  HANDLE hRecvThread      = CreateThread(NULL, 0, &RecvThread, NULL, 0, NULL);
  if ( hRecvThread )
    SetThreadPriority(hRecvThread, 2);

  return true;
}

void DestroySockets()
{
  // do cleanup stuff
  if ( gsRecv )
    closesocket(gsRecv);
  if ( gsBCRecv )
    closesocket(gsBCRecv);
  if ( gsSend )
    closesocket(gsSend);
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
