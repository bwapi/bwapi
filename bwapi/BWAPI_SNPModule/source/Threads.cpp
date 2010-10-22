#include "Threads.h"
#include <windows.h>

#include "Commands.h"
#include "Connect.h"

bool gbWantExit = false;

DWORD WINAPI RecvThread(LPVOID)
{
  // bind the socket
  bind(gsRecv, &gaddrRecv, sizeof(SOCKADDR));
  while (1)
  {
    // create receiving sockaddr
    SOCKADDR saFrom;
    int dwSaFromLen = sizeof(SOCKADDR);

    // recvfrom
    char szBuffer[512];
    int rVal = recvfrom(gsRecv, szBuffer, 512, 0, &saFrom, &dwSaFromLen);
    if ( gbWantExit )
      return 0;

    switch ( rVal )
    {
    case SOCKET_ERROR:
      Error(WSAGetLastError(), "recvfrom failed");
      return 1;
    case 0: // closed connection
      return 0;
    default:
      ParseCommand(&saFrom, szBuffer, rVal);
      break;
    }
    ++gdwRecvCalls;
    gdwRecvBytes += rVal;

    SOCKADDR_IN *from = (SOCKADDR_IN*)&saFrom;
    Log("Received data from %s:%u", inet_ntoa(from->sin_addr), from->sin_port);
  } // loop
  return 0;
}
