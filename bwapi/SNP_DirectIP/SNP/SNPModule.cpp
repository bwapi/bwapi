#include "SNPModule.h"

#include "CriticalSection.h"
#include "Output.h"
#include <Util/MemoryFrame.h>
#include <storm.h>

#include <queue>
#include <list>

namespace SNP
{
  //------------------------------------------------------------------------------------------------------------------------------------
  Network<SOCKADDR> *pluggedNetwork = NULL;

  bool fatalError = false;

  client_info gameAppInfo;

  CriticalSection critSec;
  CriticalSection::Lock *critSecExLock = NULL;
#define INTERLOCKED CriticalSection::Lock critSecLock(critSec);

  struct GamePacket
  {
    SOCKADDR sender;
    int packetSize;
    DWORD timeStamp;
    char data[512];
  };
  std::queue<GamePacket> incomingGamePackets;
  HANDLE receiveEvent;

  struct AdFile
  {
    game gameInfo;
    char extraBytes[32];
  };
  std::list<AdFile> gameList;
  int nextGameAdID = 1;

  AdFile hostedGame;
  //------------------------------------------------------------------------------------------------------------------------------------
/*

Calling behaviour:

spiInitialize

spiLockDeviceList       // we provide no devices, so everything device related is not called from here on
spiUnlockDeviceList

each second
{
  spiLockGameList       // we try to connect to target computer and if do, show the game
  spiUnlockGameList
}

*/

  /*
  Overview:

    spiInitialize       // lifetime start
    spiDestroy          // lifetime end
    
    spiLockGameList     // retrieve games list
    spiUnlockGameList   // free

    spiStartAdvertisingLadderGame   // when a game is hosted
    spiStopAdvertisingGame          // when you, the host, leaves

    spiGetGameInfo      // retrieve game info

    spiSend             // send packet

    spiReceive          // allocate and receive
    spiFree             // free allocated
  */
  //------------------------------------------------------------------------------------------------------------------------------------
  void passAdvertisement(const SOCKADDR& host, Util::MemoryFrame ad)
  {
    INTERLOCKED;

    // find if the list already has an ad from this host
    AdFile *adFile = nullptr;
    for(auto &g : gameList)
    {
      // if peer IDs equal
      if ( !memcmp(&g.gameInfo.saHost, &host, sizeof(SOCKADDR)) )
      {
        adFile = &g;
        break;
      }
    }

    // if the ad is from a new host, create a new adFile
    if(!adFile)
    {
      AdFile g;
      gameList.push_back(g);
      adFile = &gameList.back();
      adFile->gameInfo.dwIndex = ++nextGameAdID;
    }

    // init the new entry
    Util::MemoryFrame::from(adFile->gameInfo).writeAs(ad.readAs<game>());
    Util::MemoryFrame::from(adFile->extraBytes).write(ad);
    adFile->gameInfo.dwTimer = GetTickCount();
    adFile->gameInfo.saHost = host;
    adFile->gameInfo.pExtra = adFile->extraBytes;
  }
  void removeAdvertisement(const SOCKADDR& host)
  {
  }
  void passPacket(const SOCKADDR& sender, Util::MemoryFrame packet)
  {
    INTERLOCKED;
    GamePacket gamePacket;
    memcpy(gamePacket.data, packet.begin(), packet.size());
    gamePacket.packetSize = packet.size();
    gamePacket.sender = sender;
    gamePacket.timeStamp = GetTickCount();

    incomingGamePackets.push(gamePacket);

    SetEvent(receiveEvent);
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiInitialize( client_info *gameClientInfo, 
                                user_info *userData, 
                                battle_info *bnCallbacks, 
                                module_info *moduleData, 
                                HANDLE hEvent)
  {
    // Called when the module is loaded
//    DropMessage(0, "spiInitialize");

    fatalError = false;
    gameAppInfo = *gameClientInfo;

    receiveEvent = hEvent;

    critSec.init();

    try
    {
      pluggedNetwork->initialize();
    }
    catch(GeneralException &e)
    {
      fatalError = true;
      DropLastError(__FUNCTION__ " unhandled exception: %s", e.getMessage());
      return false;
    }

    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiDestroy()
  {
    // called when you leave back to the network module selection menu
//    DropMessage(0, "spiDestroy");

    try
    {
      pluggedNetwork->destroy();
    }
    catch(GeneralException &e)
    {
      fatalError = true;
      DropLastError(__FUNCTION__ " unhandled exception: %s", e.getMessage());
      return false;
    }

    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiLockGameList(int a1, int a2, game **ppGameList)
  {
    critSecExLock = new CriticalSection::Lock(critSec);
    // Strom locks the game list to access it
//    DropMessage(0, "spiLockGameList");

    // interlink gamelist entries (for storm)
    AdFile *lastAd = nullptr;
    for ( auto &it : gameList)
    {
      it.gameInfo.pExtra = it.extraBytes;
      if ( lastAd )
        lastAd->gameInfo.pNext = &it.gameInfo;
        
      lastAd = &it;
    }

    if(lastAd)
      lastAd->gameInfo.pNext = nullptr;

    // remove outdated entries
    //std::list<AdFile>::iterator nextAd = gameList.begin();
    //std::list<AdFile>::iterator currAd;
    auto currAd = gameList.begin();
    while ( currAd != gameList.end() )
    {
      if(GetTickCount() > currAd->gameInfo.dwTimer + 2000)
      {
        // outdated, remove
        currAd = gameList.erase(currAd);
      }
      else  // otherwise continue
      {
        ++currAd;
      }
    }

    try
    {
      // return game list
      *ppGameList = nullptr;
      if ( !gameList.empty() )
        *ppGameList = &gameList.begin()->gameInfo;
    }
    catch(GeneralException &e)
    {
      fatalError = true;
      DropLastError(__FUNCTION__ " unhandled exception: %s", e.getMessage());
      return false;
    }
    /*
    if ( !ppGameList )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }
    // ClearGameList func embedded
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
    EnterCriticalSection(&gCrit);
    *ppGameList = (gameStruc*)gpMGameList;
    */
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  DWORD gdwLastTickCount;
  bool __stdcall spiUnlockGameList(game *pGameList, DWORD *a2)
  {
    // when storm is done reading from the gamelist
//    DropMessage(0, "spiUnlockGameList");

    // release critical section
    delete critSecExLock;
    critSecExLock = NULL;

    try
    {
      pluggedNetwork->requestAds();
    }
    catch(GeneralException &e)
    {
      fatalError = true;
      DropLastError(__FUNCTION__ " unhandled exception: %s", e.getMessage());
      return false;
    }

    /*
    // Unlocks the game list and makes requests to update the list internally 
    if ( pGameList != gpMGameList )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }

    LeaveCriticalSection(&gCrit);
    if ( a2 )
      *a2 = 300;

    DWORD dwThisTickCount = GetTickCount();
    if ( dwThisTickCount - gdwLastTickCount > 200 )
    {
      gdwLastTickCount = dwThisTickCount;
      BroadcastGameListRequest();
    }
    */
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, void *pExtraBytes, DWORD dwExtraBytesCount)
  {
    INTERLOCKED;
//    DropMessage(0, "spiStartAdvertisingLadderGame");
    // Begin game advertisement
    // Called when you create a game

    memset(&hostedGame, 0, sizeof(hostedGame));
    strcpy_s(hostedGame.gameInfo.szGameName,       sizeof(hostedGame.gameInfo.szGameName),        pszGameName);
    strcpy_s(hostedGame.gameInfo.szGameStatString, sizeof(hostedGame.gameInfo.szGameStatString),  pszGameStatString);
    hostedGame.gameInfo.dwGameState = dwGameState;
    hostedGame.gameInfo.dwProduct   = gameAppInfo.dwProduct;
    hostedGame.gameInfo.dwVersion   = gameAppInfo.dwVerbyte;
    hostedGame.gameInfo.dwUnk_1C    = 0x0050;
    hostedGame.gameInfo.dwUnk_24    = 0x00a7;
//    hostedGame.dwGameType = dwGameType;
//    hostedGame.dwPlayerCount = dwPlayerCount;

    memcpy(hostedGame.extraBytes, pExtraBytes, dwExtraBytesCount);
    hostedGame.gameInfo.dwExtraBytes = dwExtraBytesCount;
    hostedGame.gameInfo.pExtra = hostedGame.extraBytes;

    pluggedNetwork->startAdvertising(Util::MemoryFrame::from(hostedGame));

    /*
    if ( !pszGameName || !pszGameStatString )
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }

    EnterCriticalSection(&gCrit);
    if ( !gpGameAdvert )
    {
      gpGameAdvert = SMAlloc(PKT_SIZE + sizeof(packet));
      if ( !gpGameAdvert )
      {
        LeaveCriticalSection(&gCrit);
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        Error(ERROR_NOT_ENOUGH_MEMORY, "Could not allocate game advertisement packet");
        return false;
      }
    }
    memset((void*)gpGameAdvert, 0, PKT_SIZE + sizeof(packet));
    packet    *pktHd   = (packet*)   gpGameAdvert;
    gameInfo  *pktData = (gameInfo*)((DWORD)gpGameAdvert + sizeof(packet));

    // +2 is for the two null terminators
    pktHd->wSize       = (WORD)(strlen(pszGameName) + strlen(pszGameStatString) + dwPlayerCount + sizeof(packet) + 2);
    pktHd->wType       = CMD_ADDGAME;
    
    pktData->dwGameState = dwGameState;
    SStrCopy(pktData->info, pszGameName, 128);
    SStrCopy(&pktData->info[strlen(pktData->info)+1], pszGameStatString, 128);

    LeaveCriticalSection(&gCrit);
    BroadcastAdvertisement();
    */
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiStopAdvertisingGame()
  {
    INTERLOCKED;
    // Called when you stop hosting a game
//    DropMessage(0, "spiStopAdvertisingGame");
    pluggedNetwork->stopAdvertising();
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiGetGameInfo(DWORD dwFindIndex, char *pszFindGameName, int a3, game *pGameResult)
  {
    INTERLOCKED;
    // returns game info for the game we are about to join
//    DropMessage(0, "spiGetGameInfo");

    // search for the game based on the gamename or index
    for ( auto &it : gameList )
    {
      if ( it.gameInfo.dwIndex == dwFindIndex )
      {
        *pGameResult = it.gameInfo;
        return true;
      }
    }

    // found game
    SErrSetLastError(STORM_ERROR_GAME_NOT_FOUND);
    return false;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiSend(DWORD addrCount, SOCKADDR * *addrList, char *buf, DWORD bufLen)
  {
//    DropMessage(0, "spiSend %d", GetCurrentThreadId());

    if(!addrCount)
      return true;

    if(addrCount > 1)
      DropMessage(1, "spiSend, multicast not supported");

    try
    {
      // support for 1 peer for now
      SOCKADDR him = *(addrList[0]);

      // send packet over the network module
      pluggedNetwork->sendAsyn(him, Util::MemoryFrame(buf, bufLen));

      // debug
//      DropMessage(0, "Sent storm packet %d bytes", bufLen);
//      DropMessage(0, "S> %s", sprintfBytes(buf, bufLen));
    }
    catch(GeneralException &e)
    {
      DropLastError("spiSend failed: %s", e.getMessage());
      return false;
    }
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiReceive(SOCKADDR **senderPeer, char **data, DWORD *databytes)
  {
    INTERLOCKED;
//    DropMessage(0, "spiReceive %d", GetCurrentThreadId());
    // Passes pointers from queued receive data to storm

    *senderPeer = nullptr;
    *data       = nullptr;
    *databytes  = 0;

    try
    {
      pluggedNetwork->receive();

      while(true)
      {
        // check if packets available
        if(incomingGamePackets.empty())
        {
          SErrSetLastError(STORM_ERROR_NO_MESSAGES_WAITING);
          return false;
        }

        // save the packet before removing it from queue
        GamePacket *loan = new GamePacket();
        *loan = incomingGamePackets.front();
        incomingGamePackets.pop();

        // paket outdated?
        if(GetTickCount() > loan->timeStamp + 10000)
        {
          DropMessage(1, "Dropped outdated packet (%dms delay)", GetTickCount() - loan->timeStamp);
          continue;
        }

        // give saved data to storm
        *senderPeer =&loan->sender;
        *data       = loan->data;
        *databytes  = loan->packetSize;
//        DropMessage(0, "R %s", sprintfBytes(*data, *databytes));
//        DropMessage(0, "Received storm packet %d bytes", *databytes);
        break;
      }
    }
    catch(GeneralException &e)
    {
      DropLastError("spiLockGameList failed: %s", e.getMessage());
      return false;
    }
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiFree(SOCKADDR * addr, char *data, DWORD databytes)
  {
    INTERLOCKED;
    // called after spiReceive, to free the reserved memory
//    DropMessage(0, "spiFree");

    BYTE *loan = (BYTE*)addr;
    if(loan)
      delete loan;
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiCompareNetAddresses(SOCKADDR * addr1, SOCKADDR * addr2, DWORD *dwResult)
  {
    INTERLOCKED;
    DropMessage(0, "spiCompareNetAddresses");

    if ( dwResult )
      *dwResult = 0;
    if ( !addr1 || !addr2 || !dwResult )
    {
      SErrSetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }

    *dwResult = (0 == memcmp(addr1, addr2, sizeof(SOCKADDR)));
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------




//
// the following functions are useless to us but are called by Storm, so we need working stubs
//





  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiLockDeviceList(DWORD *a1)
  {
//    DropMessage(0, "spiLockDeviceList");
    // This function is complete
    *a1 = 0;
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiUnlockDeviceList()
  {
//    DropMessage(0, "spiUnlockDeviceList");
    // This function is complete
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiFreeExternalMessage(SOCKADDR * addr, char *data, DWORD databytes)
  {
    DropMessage(0, "spiFreeExternalMessage");
    /*
    // This function is complete
    SetLastError(ERROR_INVALID_PARAMETER);
    */
    return false;
  }


  
//
// the following functions we have marked unused so we don't implement them
//






  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiGetPerformanceData(DWORD dwType, DWORD *dwResult, int a3, int a4)
  {
    DropMessage(0, "spiGetPerformanceData");
    /*
    // Returns performance data in dwResult
    switch ( dwType )
    {
    case 12:    // Total number of calls made to sendto
      *dwResult = gdwSendCalls;
      return true;
    case 13:    // Total number of calls made to recvfrom
      *dwResult = gdwRecvCalls;
      return true;
    case 14:    // Total number of bytes sent using sendto
      *dwResult = gdwSendBytes;
      return true;
    case 15:    // Total number of bytes received using recvfrom
      *dwResult = gdwRecvBytes;
      return true;
    default:
      return false;
    }
    */
    return false;
  }

  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiInitializeDevice(int a1, void *a2, void *a3, DWORD *a4, void *a5)
  {
    DropMessage(0, "spiInitializeDevice");
    // This function is complete
    return false;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiReceiveExternalMessage(SOCKADDR * *addr, char **data, DWORD *databytes)
  {
//    DropMessage(0, "spiReceiveExternalMessage");
    // This function is complete
    if ( addr )
      *addr = NULL;
    if ( data )
      *data = NULL;
    if ( databytes )
      *databytes = 0;
    SErrSetLastError(STORM_ERROR_NO_MESSAGES_WAITING);
    return false;
  }

  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiSelectGame( int a1, 
                                client_info *gameClientInfo, 
                                user_info *userData, 
                                battle_info *bnCallbacks, 
                                module_info *moduleData, 
                                int a6)
  {
    DropMessage(0, "spiSelectGame");
    // Looks like an old function and doesn't seem like it's used anymore
    // UDPN's function Creates an IPX game select dialog window
    return false;
  }

  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiSendExternalMessage(int a1, int a2, int a3, int a4, int a5)
  {
    DropMessage(0, "spiSendExternalMessage");
    // This function is complete
    return false;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiLeagueGetName(char *pszDest, DWORD dwSize)
  {
    DropMessage(0, "spiLeagueGetName");
    // This function is complete
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  SNP::NetFunctions spiFunctions = {
        sizeof(SNP::NetFunctions),
  /*n*/ &SNP::spiCompareNetAddresses,
        &SNP::spiDestroy,
        &SNP::spiFree,
  /*e*/ &SNP::spiFreeExternalMessage,
        &SNP::spiGetGameInfo,
  /*n*/ &SNP::spiGetPerformanceData,
        &SNP::spiInitialize,
  /*e*/ &SNP::spiInitializeDevice,
  /*e*/ &SNP::spiLockDeviceList,
        &SNP::spiLockGameList,
        &SNP::spiReceive,
  /*n*/ &SNP::spiReceiveExternalMessage,
  /*e*/ &SNP::spiSelectGame,
        &SNP::spiSend,
  /*e*/ &SNP::spiSendExternalMessage,
  /*n*/ &SNP::spiStartAdvertisingLadderGame,
  /*n*/ &SNP::spiStopAdvertisingGame,
  /*e*/ &SNP::spiUnlockDeviceList,
        &SNP::spiUnlockGameList,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
  /*n*/ &SNP::spiLeagueGetName
  };
}
