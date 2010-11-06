#include "Commands.h"
#include "Connect.h"

void BroadcastAdvertisement(SOCKADDR *to)
{
  EnterCriticalSection(&gCrit);
  if ( gpGameAdvert )
  {
    WORD wBCSize = ((packet*)gpGameAdvert)->wSize;
    if ( to )
    {
      if ( sendto(gsSend, (char*)gpGameAdvert, wBCSize, 0, to, sizeof(SOCKADDR)) == SOCKET_ERROR )
        Error(WSAGetLastError(), "Unable to send game advertisement in response to request");
    }
    else
    {
      if ( sendto(gsBroadcast, (char*)gpGameAdvert, wBCSize, 0, &gaddrBroadcast, sizeof(SOCKADDR)) == SOCKET_ERROR )
        Error(WSAGetLastError(), "Unable to broadcast game advertisement");
    }
    ++gdwSendCalls;
    gdwSendBytes += wBCSize;
  }
  LeaveCriticalSection(&gCrit);
}

void BroadcastGameListRequest()
{
  EnterCriticalSection(&gCrit);
  packet pkt;
  pkt.wSize       = sizeof(packet);
  pkt.wType       = CMD_GETLIST;
  pkt.dwGameState = 0;

  sendto(gsBroadcast, (char*)&pkt, sizeof(packet), 0, &gaddrBroadcast, sizeof(SOCKADDR));
  ++gdwSendCalls;
  gdwSendBytes += sizeof(packet);
  LeaveCriticalSection(&gCrit);
}

void CleanGameList(DWORD dwTimeout)
{
  EnterCriticalSection(&gCrit);
  if ( gpMGameList )
  {
    DWORD dwThisTickCount = GetTickCount();

    volatile gameStruc **g = &gpMGameList;
    while ( *g )
    {
      volatile gameStruc *t = *g;
      if ( dwThisTickCount - (*g)->dwTimer <= dwTimeout )
      {
        g = (volatile gameStruc**)&t->pNext;
      }
      else
      {
        *g = t->pNext;
        if ( t->pExtra )
          SMemFree(t->pExtra, __FILE__, __LINE__, 0);
        SMemFree((void*)t, __FILE__, __LINE__, 0);
      }
    }
  }
  LeaveCriticalSection(&gCrit);
}

DWORD gdwListIndex;
void UpdateGameList(SOCKADDR_IN *from, char *data, bool remove)
{
  EnterCriticalSection(&gCrit);
  DWORD _dwIndex = 0;
  // Clear all games owned by the incoming address
  if ( gpMGameList )
  {
    volatile gameStruc *g = gpMGameList;
    do
    {
      if ( !memcmp((void*)&g->saHost, from, sizeof(SOCKADDR)) )
      {
        gameStruc *_next = g->pNext;
        _dwIndex         = g->dwIndex;
        if ( g->pExtra )
          SMemFree(g->pExtra, __FILE__, __LINE__, 0);
        // NOTE: The following lines are actually some kind of single-line destructor/finder
        volatile gameStruc **t = &gpMGameList;
        volatile gameStruc *i;
        do
        {
          i = *t;
          if ( *t == g )
            break;
          t = (volatile gameStruc**)&i->pNext;
        } while ( i->pNext );
        if ( *t )
          *t = (*t)->pNext;

        SMemFree((void*)g, __FILE__, __LINE__, 0);
        // end destructor
        g = _next;
      } // memcmp true
      else
      {
        g = g->pNext;
      } // memcmp false
    } while ( g );
  } // if gamelist

  if ( !_dwIndex )
  {
    ++gdwListIndex;
    _dwIndex = gdwListIndex;
    if ( gdwListIndex == 0xFFFFFFFF )
    {
      gdwListIndex = 1;
      _dwIndex     = 1;
    }
  }

  if ( !remove )
  {
    packet    *pktHd   = (packet*)data;
    char      *pktData = data + sizeof(packet);
    gameStruc *newGame = (gameStruc*)SMemAlloc(sizeof(gameStruc), __FILE__, __LINE__, 0);
    if ( !newGame )
      Error(ERROR_NOT_ENOUGH_MEMORY, "Could not allocate memory for game list.");

    memcpy(&newGame->saHost, from, sizeof(SOCKADDR));

    newGame->dwIndex      = _dwIndex;
    newGame->dwGameState  = pktHd->dwGameState;
    newGame->dwUnk_1C     = 50; // latency timeout?
    newGame->dwTimer      = GetTickCount();
    newGame->dwVersion    = gdwVerbyte;
    newGame->dwProduct    = gdwProduct;
    newGame->pNext        = (gameStruc*)gpMGameList;

    int gamelen           = SStrCopy(newGame->szGameName, pktData, sizeof(newGame->szGameName));
    int statlen           = SStrCopy(newGame->szGameStatString, &pktData[gamelen+1], sizeof(newGame->szGameStatString));

    newGame->dwExtraBytes = pktHd->wSize - (sizeof(packet) + gamelen + statlen + 2);
    newGame->pExtra       = SMemAlloc(newGame->dwExtraBytes, __FILE__, __LINE__, 0);
    memcpy(newGame->pExtra, &pktData[gamelen + statlen + 2], newGame->dwExtraBytes);

    gpMGameList           = newGame;
  }
  LeaveCriticalSection(&gCrit);
}
