#include "Threads.h"
#include <windows.h>

#include "Connect.h"

bool gbWantExit = false;

DWORD WINAPI ListenToBroadcasts(LPVOID)
{
  // bind the socket
  bind(gsBroadcast, &gaddrRecvBroadcast, sizeof(SOCKADDR));
  while (1)
  {
    // create receiving sockaddr
    SOCKADDR saFrom;
    int dwSaFromLen = sizeof(SOCKADDR);

    // recvfrom
    char szBuffer[512];
    int rVal = recvfrom(gsBroadcast, szBuffer, 512, 0, &saFrom, &dwSaFromLen);
    if ( gbWantExit )
      return 0;

    if ( rVal <= 0 )
      break;

    ++gdwRecvCalls;
    gdwRecvBytes += rVal;

    i("Got it!");
  } // loop
  w("recvfrom failed?");
  return 0;
}

