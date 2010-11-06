#include "Commands.h"
#include "Connect.h"

namespace LUDP
{
  void BroadcastAdvertisement(SOCKADDR *to)
  {
    EnterCriticalSection(&gCrit);
    if ( gpGameAdvert )
    {
      WORD wBCSize = ((packet*)gpGameAdvert)->wSize;
      if ( to )
        SendData(gsSend, (char*)gpGameAdvert, wBCSize, to);
      else
        SendData(gsBroadcast, (char*)gpGameAdvert, wBCSize, &gaddrBroadcast);
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

    SendData(gsBroadcast, (char*)&pkt, sizeof(packet), &gaddrBroadcast);
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
            SMFree(t->pExtra);
          SMFree((void*)t);
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
            SMFree(g->pExtra);
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

          SMFree((void*)g);
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
      gameStruc *newGame = (gameStruc*)SMAlloc(sizeof(gameStruc));
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
      newGame->pExtra       = SMAlloc(newGame->dwExtraBytes);
      memcpy(newGame->pExtra, &pktData[gamelen + statlen + 2], newGame->dwExtraBytes);

      gpMGameList           = newGame;
    }
    LeaveCriticalSection(&gCrit);
  }
};
