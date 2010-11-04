#include <windows.h>

#include "Threads.h"
#include "Connect.h"
#include "Commands.h"

bool gbWantExit = false;
std::deque<pktq*> recvQueue;
void *gpGameAdvert;

DWORD WINAPI BroadcastThread(LPVOID)
{
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
      recvQueue.clear();
      return 0;
    }
    ++gdwRecvCalls;
    gdwRecvBytes += rVal;

    SOCKADDR_IN *from = (SOCKADDR_IN*)&saFrom;
    SMemZero(from->sin_zero, sizeof(from->sin_zero));
    broadcastPkt *bc  = (broadcastPkt*)&szBuffer;
    if ( bc->wSize >= sizeof(broadcastPkt) && bc->wSize == rVal ) // @TODO: also match checksum
    {
      switch ( bc->wType )
      {
      case 0: // add/update game
      case 1: // remove game
        // advertise game/response
        UpdateGameList(from, szBuffer, bc->wType != 0);
        break;
      case 2:
        // request list
        BroadcastAdvertisement(&saFrom);
        break;
      case 3:
        {
          pktq *recvPkt = (pktq*)SMemAlloc(sizeof(pktq), __FILE__, __LINE__, 0);
          memcpy(&recvPkt->addr, &saFrom, sizeof(SOCKADDR));
          recvPkt->dwLength = rVal - sizeof(broadcastPkt);
          memcpy(recvPkt->bData, &szBuffer[sizeof(broadcastPkt)], recvPkt->dwLength);
          recvQueue.push_back(recvPkt);
          break;
        }
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
