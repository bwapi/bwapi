#include <windows.h>

#include "Threads.h"
#include "Connect.h"
#include "Commands.h"

bool gbWantExit = false;
std::deque<pktq*> recvQueue;
void *gpGameAdvert;

DWORD WINAPI RecvThread(LPVOID)
{
  // bind the socket
  bind(gsRecv, &gaddrRecv, sizeof(SOCKADDR));
  while (1)
  {
    // recvfrom
    int dwSaFromLen   = sizeof(SOCKADDR);
    pktq *recvPkt     = (pktq*)SMemAlloc(sizeof(pktq), __FILE__, __LINE__, 0);
    recvPkt->dwLength = recvfrom(gsRecv, recvPkt->bData, sizeof(recvPkt->bData), 0, &recvPkt->addr, &dwSaFromLen);
    if ( gbWantExit )
    {
      recvQueue.clear();
      return 0;
    }

    switch ( recvPkt->dwLength )
    {
    case SOCKET_ERROR:
      Error(WSAGetLastError(), "recvfrom (game) failed");
    case 0: // closed connection
      recvQueue.clear();
      return 0;
    }
    ++gdwRecvCalls;
    gdwRecvBytes += recvPkt->dwLength;

    recvQueue.push_back(recvPkt);

    SOCKADDR_IN *from = (SOCKADDR_IN*)&recvPkt->addr;
    Log("Received data from %s:%u", inet_ntoa(from->sin_addr), from->sin_port);
  } // loop
  return 0;
}

DWORD WINAPI BroadcastThread(LPVOID)
{
  // First broadcast
  //BroadcastGameListRequest();

  // bind the socket
  bind(gsBCRecv, &gaddrBCRecv, sizeof(SOCKADDR));
  while (1)
  {
    // create receiving sockaddr
    SOCKADDR saFrom;
    int dwSaFromLen = sizeof(SOCKADDR);

    // recvfrom
    char szBuffer[LOCL_PKT_SIZE + sizeof(broadcastPkt)];
    memset(szBuffer, 0, LOCL_PKT_SIZE + sizeof(broadcastPkt));
    int rVal = recvfrom(gsBCRecv, szBuffer, LOCL_PKT_SIZE + sizeof(broadcastPkt), 0, &saFrom, &dwSaFromLen);
    if ( gbWantExit )
      return 0;

    switch ( rVal )
    {
    case SOCKET_ERROR:
      Error(WSAGetLastError(), "recvfrom (broadcast) failed");
    case 0: // closed connection
      return 0;
    }
    ++gdwRecvCalls;
    gdwRecvBytes += rVal;

    SOCKADDR_IN *from = (SOCKADDR_IN*)&saFrom;
    broadcastPkt *bc  = (broadcastPkt*)&szBuffer;
    if ( bc->wSize >= sizeof(broadcastPkt) && bc->wSize == rVal ) // @TODO: also match checksum
    {
      switch ( bc->wType )
      {
      case 0: // add/update game
      case 1: // remove game
        // advertise game/response
        Log("Received game advertisement update from %s:%u", inet_ntoa(from->sin_addr), from->sin_port);
        break;
      case 2:
        // request list
        BroadcastAdvertisement();
        break;
      default:
        Error(ERROR_INVALID_PARAMETER, "Unidentified broadcast type %04X", bc->wType);
        break;
      }
    }
    else
    {
      Error(ERROR_INVALID_PARAMETER, "Bad broadcast packet");
    }
  } // loop
  return 0;
}
