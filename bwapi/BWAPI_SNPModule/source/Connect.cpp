#include "Connect.h"

DWORD dwThisClientId;

SOCKET   recvSocket;
SOCKADDR recvAddr;

SOCKET sendSocket;

SOCKET   broadcastSocket;
SOCKADDR broadcastAddr;

void InitializeConnection()
{
  // do initialization stuff
  WSADATA wsaData;
  if ( WSAStartup(MAKEWORD(1, 1), &wsaData) == 0 )
    e("Error in WSAStartup.");

  recvSocket      = MakeUDPSocket();
  InitAddr(&recvAddr, "127.0.0.1");

  sendSocket      = MakeUDPSocket();

  broadcastSocket = MakeUDPSocket();
  InitAddr(&broadcastAddr, "127.255.255.255");
}

void DestroyConnection()
{
  // do cleanup stuff
  WSACleanup();
}

SOCKADDR *InitAddr(SOCKADDR *addr, const char *ip)
{
  sockaddr_in *_addr = (sockaddr_in*)addr;
  memset(addr, 0, sizeof(SOCKADDR));
  _addr->sin_family           = AF_INET;
  _addr->sin_port             = htons(BASE_PORT);
  _addr->sin_addr.S_un.S_addr = inet_addr(ip);
  return addr;
}

SOCKET MakeUDPSocket()
{
  SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  DWORD dwTrue = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&dwTrue, sizeof(DWORD));
  setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&dwTrue, sizeof(DWORD));
  return s;
}

DWORD WINAPI ListenToBroadcasts(LPVOID)
{
  // bind the socket
  bind(recvSocket, &recvAddr, sizeof(SOCKADDR));
  while (1)
  {
    // create receiving sockaddr
    SOCKADDR saFrom;
    int dwSaFromLen = sizeof(SOCKADDR);

    // recvfrom
    char szBuffer[512];
    int rVal = recvfrom(recvSocket, szBuffer, 512, 0, &saFrom, &dwSaFromLen);
    if ( rVal <= 0 )
      break;

    i("Got it!");
  } // loop
  w("recvfrom failed?");
}