#include <windows.h>

#include "Threads.h"
#include "Connect.h"
#include "Commands.h"

#include "../../LPIP_Server/SharedMemory.h"

namespace LUDP
{
  bool gbWantExit = false;
  volatile pktq *gpRecvQueue;
  volatile void *gpGameAdvert;

  DWORD WINAPI RecvThread(LPVOID)
  {
    for (;;)
    {
      // create receiving sockaddr
      SOCKADDR_IN saFrom;
      int dwSaFromLen = sizeof(SOCKADDR);

      // recvfrom
      char szBuffer[PKT_SIZE + sizeof(packet)];
      memset(szBuffer, 0, PKT_SIZE + sizeof(packet));
      int rVal = recvfrom(gsRecv, szBuffer, PKT_SIZE + sizeof(packet), 0, (SOCKADDR*)&saFrom, &dwSaFromLen);
      if ( gbWantExit )
        return 0;

      ++gdwRecvCalls;
      gdwRecvBytes += rVal;

      switch ( rVal )
      {
      case SOCKET_ERROR:
        Error(WSAGetLastError(), "recvfrom (thread) failed");
      case 0: // closed connection
        return 0;
      }

      memset(saFrom.sin_zero, 0, sizeof(saFrom.sin_zero));
      packet *bc  = (packet*)&szBuffer;
      if ( bc->wSize >= sizeof(packet) && bc->wSize == rVal )
      {
        switch ( bc->wType )
        {
        case CMD_ADDGAME: // add/update game
        case CMD_REMOVEGAME: // remove game
          // advertise game/response
          UpdateGameList(&saFrom, szBuffer, bc->wType == CMD_REMOVEGAME);
          break;
        case CMD_GETLIST:
          // request list
          BroadcastAdvertisement(&saFrom);
          break;
        case CMD_STORM:
          {
            pktq *recvPkt = (pktq*)SMAlloc(sizeof(pktq));
            if ( !recvPkt )
              Error(ERROR_NOT_ENOUGH_MEMORY, "Recv Allocation error");

            memcpy(&recvPkt->saFrom, &saFrom, sizeof(SOCKADDR));
            recvPkt->dwLength    = rVal - sizeof(packet);
            memcpy(recvPkt->bData, &szBuffer[sizeof(packet)], recvPkt->dwLength);
            recvPkt->pNext       = NULL;

            EnterCriticalSection(&gCrit);
            if ( gpRecvQueue )
            {
              volatile pktq *i = gpRecvQueue;
              while ( i->pNext )
              {
                i = i->pNext;
              }
              i->pNext = recvPkt;
            }
            else
            {
              gpRecvQueue = recvPkt;
            }
            SetEvent(ghRecvEvent);
            LeaveCriticalSection(&gCrit);
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
  }
};

namespace LPIP
{
  bool gbWantExit = false;
  volatile pktq *gpRecvQueue;

  SharedMemory *s;

  DWORD WINAPI RecvThread(LPVOID)
  {
    for (;;)
    {
      // @TODO: Receive any data here
      DWORD fromProcessID;
      char buffer[PKT_SIZE];
      int length = s->receiveData(buffer, PKT_SIZE, &fromProcessID, true, &gbWantExit);
      if ( gbWantExit || length == 0 )
        return 0;
      if ( length == -1 )
      {
        Error(GetLastError(), "Pipe failed to receive!");
        return 0;
      }

      ++gdwRecvCalls;
      gdwRecvBytes += length;

      pktq *recvPkt = (pktq*)SMAlloc(sizeof(pktq));
      if ( !recvPkt )
      {
        Error(ERROR_NOT_ENOUGH_MEMORY, "Recv Allocation error");
        return 0;
      }

      // @TODO: Copy received data to this structure
      memcpy(recvPkt->bData, buffer, PKT_SIZE);
      recvPkt->dwLength = length;
      recvPkt->dwProcID = fromProcessID;

      recvPkt->pNext = NULL;
      // the rest of this adds it to the queue and sets an event that I don't know too much about
      EnterCriticalSection(&gCrit);
      if ( gpRecvQueue )
      {
        volatile pktq *i = gpRecvQueue;
        while ( i->pNext )
          i = i->pNext;
        i->pNext = recvPkt;
      }
      else
      {
        gpRecvQueue = recvPkt;
      }
      LeaveCriticalSection(&gCrit);
      SetEvent(ghRecvEvent);
    } // loop
  }
};
