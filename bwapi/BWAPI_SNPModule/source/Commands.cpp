#include "Commands.h"
#include "CommandTypes.h"
#include "Common.h"

std::queue<pktq*> recvQueue;
DWORD gdwBroadcastCount;

int BroadcastCommand(DWORD dwCmdID, char *data, DWORD dwDataSize)
{
  SOCKADDR addr;
  InitAddr(&addr, "127.255.255.255");
  return SendCommand(&addr, dwCmdID, data, dwDataSize);
}

int SendCommand(SOCKADDR *dstAddr, DWORD dwCmdID, char *data, DWORD dwDataSize)
{
  char buff[512];
  pkt  *p = (pkt*)buff;
  if ( data )
    memcpy(p->bData, data, dwDataSize < 508 ? dwDataSize : 508);
  p->dwCommand = dwCmdID;

  int r = sendto(gsSend, (const char*)p, 512, 0, dstAddr, sizeof(SOCKADDR));
  if ( r == SOCKET_ERROR || r == 0 )
    Error(WSAGetLastError(), "Error in sendto.");
  return r;
}

void ParseCommand(SOCKADDR *from, char *data, DWORD dwDataSize)
{
  if ( dwDataSize < 4 )
    return;

  pkt         *p = (pkt*)data;
  SOCKADDR_IN *f = (SOCKADDR_IN*)&from;

  switch ( p->dwCommand )
  {
  case CMD_STORM:
    {
      pktq *pktsave = (pktq*)SMemAlloc(sizeof(pktq), __FILE__, __LINE__, 0);
      memcpy(&pktsave->addr, from, sizeof(SOCKADDR));
      memcpy(&pktsave->packet, data, dwDataSize);
      pktsave->dwLength = dwDataSize;
      recvQueue.push(pktsave);
      break;
    }
  case CMD_PING:
    SendCommand(from, CMD_PONG);
    break;
  case CMD_PONG:
    break;
  case CMD_BROADCAST_PING:
    BroadcastCommand(CMD_BROADCAST_PONG);
    break;
  case CMD_BROADCAST_PONG:
    ++gdwBroadcastCount;
    break;
  default:
    Error(ERROR_INVALID_PARAMETER, "Bad packet");
    break;
  }

}
