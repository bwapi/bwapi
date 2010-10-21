#include "Connect.h"
#include "Common.h"
#include "Threads.h"

DWORD gdwSendCalls = 0;
DWORD gdwSendBytes = 0;
DWORD gdwRecvCalls = 0;
DWORD gdwRecvBytes = 0;

SOCKET   gsGame    = NULL;
SOCKADDR gaddrRecvGame;

SOCKET   gsBroadcast = NULL;
SOCKADDR gaddrRecvBroadcast;

SOCKET gsSendSock = NULL;

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

  gsGame      = MakeUDPSocket();
  gsBroadcast = MakeUDPSocket();
  gsSendSock  = MakeUDPSocket();

  InitAddr(&gaddrRecvBroadcast, "127.0.0.1", 6111);
  InitAddr(&gaddrRecvGame);

  // begin recv threads here
  HANDLE hThread = CreateThread(NULL, 0, ListenToBroadcasts, NULL, 0, NULL);
  if ( hThread )
  {
    SetThreadPriority(hThread, 1);
  }
  return true;
}

void DestroySockets()
{
  // do cleanup stuff
  if ( gsGame )
    closesocket(gsGame);
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
  dwTrue = 1;
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
