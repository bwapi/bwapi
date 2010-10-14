#include "Connect.h"

DWORD dwThisClientId;

void InitializeConnection()
{
  // do initialization stuff
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void DestroyConnection()
{
  // do cleanup stuff
  WSACleanup();
}

DWORD WINAPI ListenToBroadcasts(LPVOID)
{
  // Create the socket
  SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // make address re-usable
  DWORD dwTrue = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&dwTrue, sizeof(DWORD));

  // Create sockaddr
  sockaddr_in saIn;
  saIn.sin_family           = AF_INET;
  saIn.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
  saIn.sin_port             = htons(BASE_PORT);
  
  // bind the socket
  bind(s, (const sockaddr*)&saIn, sizeof(sockaddr));

  while (1)
  {
    // create receiving sockaddr
    sockaddr_in saFrom;
    int dwSaFromLen = sizeof(sockaddr);

    // recvfrom
    char szBuffer[512];
    int rVal = recvfrom(s, szBuffer, 512, 0, (sockaddr*)&saFrom, &dwSaFromLen);
    if ( rVal <= 0 )
      break;

    i("Got it!");
  } // loop
  w("recvfrom failed?");
}