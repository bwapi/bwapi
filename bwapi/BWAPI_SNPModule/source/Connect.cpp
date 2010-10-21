#include "Connect.h"

DWORD gdwSendCalls = 0;
DWORD gdwSendBytes = 0;
DWORD gdwRecvCalls = 0;
DWORD gdwRecvBytes = 0;

SOCKET gsGame      = NULL;
SOCKET gsBroadcast = NULL;

void InitializeSockets()
{
  // do initialization stuff
  WSADATA wsaData;
  if ( WSAStartup(MAKEWORD(2, 2), &wsaData) == 0 )
    e("Error in WSAStartup.");

  gsGame      = MakeUDPSocket();
  gsBroadcast = MakeUDPSocket();

  // begin recv thread here
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

DWORD WINAPI ListenToBroadcasts(LPVOID)
{
  // bind the socket
  SOCKADDR bindAddr;
  InitAddr(&bindAddr, "127.0.0.1", 6111);
  bind(gsBroadcast, &bindAddr, sizeof(SOCKADDR));
  while (1)
  {
    // create receiving sockaddr
    SOCKADDR saFrom;
    int dwSaFromLen = sizeof(SOCKADDR);

    // recvfrom
    char szBuffer[512];
    int rVal = recvfrom(gsBroadcast, szBuffer, 512, 0, &saFrom, &dwSaFromLen);

    if ( rVal <= 0 )
      break;

    ++gdwRecvCalls;
    gdwRecvBytes += rVal;

    i("Got it!");
  } // loop
  w("recvfrom failed?");
}