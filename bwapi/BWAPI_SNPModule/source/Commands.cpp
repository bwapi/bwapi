#include "Commands.h"
#include "Connect.h"

void BroadcastAdvertisement(SOCKADDR *to)
{
  if ( gpGameAdvert )
  {
    WORD wBCSize = ((broadcastPkt*)gpGameAdvert)->wSize;
    if ( to )
      sendto(gsBCSend, (char*)gpGameAdvert, wBCSize, 0, to, sizeof(SOCKADDR));
    else
      sendto(gsBroadcast, (char*)gpGameAdvert, wBCSize, 0, &gaddrBroadcast, sizeof(SOCKADDR));
    ++gdwSendCalls;
    gdwSendBytes += wBCSize;
  }
}

void BroadcastGameListRequest()
{
  broadcastPkt pkt;
  pkt.wSize       = sizeof(broadcastPkt);
  pkt.wType       = 2;
  pkt.wReserved   = 0;
  pkt.dwVersion   = gdwVerbyte;
  pkt.dwProduct   = gdwProduct;
  pkt.dwGameState = 0;

  // @TODO: Checksum
  pkt.wChecksum   = 0;

  sendto(gsBroadcast, (char*)&pkt, sizeof(broadcastPkt), 0, &gaddrBroadcast, sizeof(SOCKADDR));
  ++gdwSendCalls;
  gdwSendBytes += sizeof(broadcastPkt);
}

void CleanGameList(DWORD dwTimeout)
{
  if ( !gpMGameList )
    return;

  DWORD dwThisTickCount = GetTickCount();

  gameStruc **g = &gpMGameList;
  while ( *g )
  {
    gameStruc *t = *g;
    if ( dwThisTickCount - (*g)->dwTimer <= dwTimeout )
    {
      g = &t->pNext;
    }
    else
    {
      *g = t->pNext;
      if ( t->pExtra )
        SMemFree(t->pExtra, __FILE__, __LINE__, 0);
      SMemFree(t, __FILE__, __LINE__, 0);
    }
  }
}

DWORD gdwListIndex;
void UpdateGameList(SOCKADDR_IN *from, char *data, bool remove)
{
  DWORD _dwIndex = 0;
  // Clear all games owned by the incoming address
  if ( gpMGameList )
  {
    gameStruc *g = gpMGameList;
    do
    {
      if ( !memcmp(&g->saHost, from, sizeof(SOCKADDR)) )
      {
        gameStruc *_next = g->pNext;
        _dwIndex         = g->dwIndex;
        if ( g->pExtra )
          SMemFree(g->pExtra, __FILE__, __LINE__, 0);
        // NOTE: The following lines are actually some kind of single-line destructor/finder
        gameStruc **t = &gpMGameList;
        gameStruc *i;
        do
        {
          i = *t;
          if ( *t == g )
            break;
          t = &i->pNext;
        } while ( i->pNext );
        if ( *t )
          *t = (*t)->pNext;

        SMemFree(g, __FILE__, __LINE__, 0);
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

  if ( remove )
    return;

  broadcastPkt *pktHd   = (broadcastPkt*)data;
  char         *pktData = data + sizeof(broadcastPkt);
  gameStruc    *newGame = (gameStruc*)SMemAlloc(sizeof(gameStruc), __FILE__, __LINE__, 0);
  if ( !newGame )
    return;

  memcpy(&newGame->saHost, from, sizeof(SOCKADDR));

  newGame->dwIndex      = _dwIndex;
  newGame->dwGameState  = pktHd->dwGameState;
  newGame->dwUnk_1C     = 50;
  newGame->dwTimer      = GetTickCount();
  newGame->dwVersion    = pktHd->dwVersion;
  newGame->dwProduct    = pktHd->dwProduct;
  newGame->pNext        = gpMGameList;

  int gamelen           = SStrCopy(newGame->szGameName, pktData, sizeof(newGame->szGameName));
  int statlen           = SStrCopy(newGame->szGameStatString, &pktData[gamelen+1], sizeof(newGame->szGameStatString));

  newGame->dwExtraBytes = pktHd->wSize - (sizeof(broadcastPkt) + gamelen + statlen + 2);
  newGame->pExtra       = SMemAlloc(newGame->dwExtraBytes, __FILE__, __LINE__, 0);
  memcpy(newGame->pExtra, &pktData[gamelen + statlen + 2], newGame->dwExtraBytes);

  gpMGameList           = newGame;
}
