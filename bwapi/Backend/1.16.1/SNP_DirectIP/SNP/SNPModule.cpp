#include "SNPNetwork.h"

#include "CriticalSection.h"
#include "Output.h"
#include "../Util/MemoryFrame.h"
#include <storm.h>

#include <queue>
#include <list>

namespace SNP
{
  //------------------------------------------------------------------------------------------------------------------------------------
  BaseNetwork *pluggedNetwork = nullptr;

  SNETPROGRAMDATA gameAppInfo;

  CriticalSection critSec;
  CriticalSection::Lock *critSecExLock = nullptr;
#define INTERLOCKED CriticalSection::Lock critSecLock(critSec);

  struct GamePacket
  {
    SNETADDR sender;
    int packetSize;
    DWORD timeStamp;
    char data[512];
  };
  std::queue<GamePacket> incomingGamePackets;
  HANDLE receiveEvent;

  struct AdFile
  {
    SNETSPI_GAMELIST gameInfo;
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

    spiStartAdvertisingGame   // when a game is hosted
    spiStopAdvertisingGame          // when you, the host, leaves

    spiGetGameInfo      // retrieve game info

    spiSend             // send packet

    spiReceive          // allocate and receive
    spiFree             // free allocated
  */
  //------------------------------------------------------------------------------------------------------------------------------------
  void passAdvertisement(const SNETADDR& host, Util::MemoryFrame ad)
  {
    INTERLOCKED;

    // find if the list already has an ad from this host
    AdFile *adFile = nullptr;
    for(auto &g : gameList)
    {
      // if peer IDs equal
      if ( !memcmp(&g.gameInfo.owner, &host, sizeof(SNETADDR)) )
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
      adFile->gameInfo.gameid = ++nextGameAdID;
    }

    // init the new entry
    Util::MemoryFrame::from(adFile->gameInfo).writeAs(ad.readAs<SNETSPI_GAMELIST>());
    Util::MemoryFrame::from(adFile->extraBytes).write(ad);
    adFile->gameInfo.ownerlasttime = GetTickCount();
    adFile->gameInfo.owner = host;
    adFile->gameInfo.clientdata = adFile->extraBytes;
  }
  void removeAdvertisement(const SNETADDR& host)
  {
  }
  void passPacket(const SNETADDR& sender, Util::MemoryFrame packet)
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
  BOOL __stdcall spiInitialize(SNETPROGRAMDATAPTR gameClientInfo,
                                SNETPLAYERDATAPTR userData,
                                SNETUIDATAPTR bnCallbacks,
                                SNETVERSIONDATAPTR moduleData,
                                HANDLE hEvent)
  {
    // Called when the module is loaded
//    DropMessage(0, "spiInitialize");

    gameAppInfo = *gameClientInfo;

    receiveEvent = hEvent;

    critSec.init();

    try
    {
      pluggedNetwork->initialize();
    }
    catch(GeneralException &e)
    {
      DropLastError(__FUNCTION__ "%s unhandled exception: %s", e.getMessage().c_str());
      return FALSE;
    }

    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiDestroy()
  {
    // called when you leave back to the network module selection menu
//    DropMessage(0, "spiDestroy");

    try
    {
      pluggedNetwork->destroy();
    }
    catch(GeneralException &e)
    {
      DropLastError(__FUNCTION__ " unhandled exception: %s", e.getMessage().c_str());
      return FALSE;
    }

    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiLockGameList(DWORD, DWORD, SNETSPI_GAMELISTPTR *ppGameList)
  {
    critSecExLock = new CriticalSection::Lock(critSec);
    // Strom locks the game list to access it
//    DropMessage(0, "spiLockGameList");

    // interlink gamelist entries (for storm)
    AdFile *lastAd = nullptr;
    for ( auto &it : gameList)
    {
      it.gameInfo.clientdata = it.extraBytes;
      if ( lastAd )
        lastAd->gameInfo.next = &it.gameInfo;
        
      lastAd = &it;
    }

    if(lastAd)
      lastAd->gameInfo.next = nullptr;

    // remove outdated entries
    //std::list<AdFile>::iterator nextAd = gameList.begin();
    //std::list<AdFile>::iterator currAd;
    auto currAd = gameList.begin();
    while ( currAd != gameList.end() )
    {
      if(GetTickCount() > currAd->gameInfo.ownerlasttime + 2000)
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
      DropLastError(__FUNCTION__ " unhandled exception: %s", e.getMessage().c_str());
      return FALSE;
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
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  DWORD gdwLastTickCount;
  BOOL __stdcall spiUnlockGameList(SNETSPI_GAMELISTPTR pGameList, DWORD*)
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
      DropLastError(__FUNCTION__ " unhandled exception: %s", e.getMessage().c_str());
      return FALSE;
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
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiStartAdvertisingGame(
    LPCSTR gamename, LPCSTR gamepassword, LPCSTR gamedescription, 
    DWORD gamemode, DWORD gameage, DWORD gamecategorybits, DWORD optcategorybits, DWORD,
    LPCVOID clientdata, DWORD clientdatabytes)
  {
    INTERLOCKED;
//    DropMessage(0, "spiStartAdvertisingGame");
    // Begin game advertisement
    // Called when you create a game

    memset(&hostedGame, 0, sizeof(hostedGame));
    strcpy_s(hostedGame.gameInfo.gamename, SNETSPI_MAXSTRINGLENGTH, gamename);
    strcpy_s(hostedGame.gameInfo.gamedescription, SNETSPI_MAXSTRINGLENGTH, gamedescription);
    hostedGame.gameInfo.gamemode = gamemode;
    hostedGame.gameInfo.productid   = gameAppInfo.programid;
    hostedGame.gameInfo.version     = gameAppInfo.versionid;
    hostedGame.gameInfo.ownerlatency     = 0x0050;
    hostedGame.gameInfo.gamecategorybits = 0x00a7;
//    hostedGame.dwGameType = dwGameType;
//    hostedGame.dwPlayerCount = dwPlayerCount;

    memcpy(hostedGame.extraBytes, clientdata, clientdatabytes);
    hostedGame.gameInfo.clientdata = hostedGame.extraBytes;
    hostedGame.gameInfo.clientdatabytes = clientdatabytes;

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
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiStopAdvertisingGame()
  {
    INTERLOCKED;
    // Called when you stop hosting a game
//    DropMessage(0, "spiStopAdvertisingGame");
    pluggedNetwork->stopAdvertising();
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiGetGameInfo(DWORD dwFindIndex, LPCSTR pszFindGameName, LPCSTR, SNETSPI_GAMELIST* pGameResult)
  {
    INTERLOCKED;
    // returns game info for the game we are about to join
//    DropMessage(0, "spiGetGameInfo");

    // search for the game based on the gamename or index
    for ( auto &it : gameList )
    {
      if ( it.gameInfo.gameid == dwFindIndex )
      {
        *pGameResult = it.gameInfo;
        return TRUE;
      }
    }

    // found game
    SErrSetLastError(STORM_ERROR_GAME_NOT_FOUND);
    return FALSE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiSend(DWORD addresses, SNETADDRPTR* addrlist, LPVOID data, DWORD databytes)
  {
//    DropMessage(0, "spiSend %d", GetCurrentThreadId());

    if(!addresses)
      return TRUE;

    if(addresses > 1)
      DropMessage(1, "spiSend, multicast not supported");

    try
    {
      // support for 1 peer for now
      SNETADDR him = *(addrlist[0]);

      // send packet over the network module
      pluggedNetwork->sendAsyn(him, Util::MemoryFrame(data, databytes));

      // debug
//      DropMessage(0, "Sent storm packet %d bytes", bufLen);
//      DropMessage(0, "S> %s", sprintfBytes(buf, bufLen));
    }
    catch(GeneralException &e)
    {
      DropLastError("spiSend failed: %s", e.getMessage().c_str());
      return FALSE;
    }
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiReceive(SNETADDRPTR* addr, LPVOID* data, DWORD* databytes)
  {
    INTERLOCKED;
//    DropMessage(0, "spiReceive %d", GetCurrentThreadId());
    // Passes pointers from queued receive data to storm

    *addr = nullptr;
    *data = nullptr;
    *databytes = 0;

    try
    {
      pluggedNetwork->receive();

      while(true)
      {
        // check if packets available
        if(incomingGamePackets.empty())
        {
          SErrSetLastError(STORM_ERROR_NO_MESSAGES_WAITING);
          return FALSE;
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
        *addr       = &loan->sender;
        *data       = loan->data;
        *databytes  = loan->packetSize;
//        DropMessage(0, "R %s", sprintfBytes(*data, *databytes));
//        DropMessage(0, "Received storm packet %d bytes", *databytes);
        break;
      }
    }
    catch(GeneralException &e)
    {
      DropLastError("spiLockGameList failed: %s", e.getMessage().c_str());
      return FALSE;
    }
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiFree(SNETADDRPTR addr, LPVOID data, DWORD databytes)
  {
    INTERLOCKED;
    // called after spiReceive, to free the reserved memory
//    DropMessage(0, "spiFree");

    BYTE *loan = (BYTE*)addr;
    if(loan)
      delete loan;
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiCompareNetAddresses(SNETADDRPTR addr1, SNETADDRPTR addr2, DWORD* diffmagnitude)
  {
    INTERLOCKED;
    DropMessage(0, "spiCompareNetAddresses");

    if (diffmagnitude)
      *diffmagnitude = 0;
    if ( !addr1 || !addr2 || !diffmagnitude)
    {
      SErrSetLastError(ERROR_INVALID_PARAMETER);
      return FALSE;
    }

    *diffmagnitude = (0 == memcmp(addr1, addr2, sizeof(SNETADDR)));
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------




//
// the following functions are useless to us but are called by Storm, so we need working stubs
//





  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiLockDeviceList(SNETSPI_DEVICELISTPTR* devicelist)
  {
//    DropMessage(0, "spiLockDeviceList");
    // This function is complete
    *devicelist = 0;
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiUnlockDeviceList(SNETSPI_DEVICELISTPTR devicelist)
  {
//    DropMessage(0, "spiUnlockDeviceList");
    // This function is complete
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiFreeExternalMessage(LPCSTR addr, LPCSTR data, LPCSTR databytes)
  {
    DropMessage(0, "spiFreeExternalMessage");
    /*
    // This function is complete
    SetLastError(ERROR_INVALID_PARAMETER);
    */
    return FALSE;
  }


  
//
// the following functions we have marked unused so we don't implement them
//






  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiGetPerformanceData(DWORD counterid, DWORD* countervalue, LARGE_INTEGER* measurementtime, LARGE_INTEGER* measurementfreq)
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
    return FALSE;
  }

  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiInitializeDevice(DWORD deviceid, SNETPROGRAMDATAPTR programdata, SNETPLAYERDATAPTR playerdata, SNETUIDATAPTR itnerfacedata, SNETVERSIONDATAPTR versiondata)
  {
    DropMessage(0, "spiInitializeDevice");
    // This function is complete
    return FALSE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiReceiveExternalMessage(SNETADDRPTR* addr, LPVOID* data, DWORD* databytes)
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
    return FALSE;
  }

  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiSelectGame(DWORD flags, SNETPROGRAMDATAPTR programdata, SNETPLAYERDATAPTR playerdata, SNETUIDATAPTR interfacedata, SNETVERSIONDATAPTR versiondata, DWORD* playerid)
  {
    DropMessage(0, "spiSelectGame");
    // Looks like an old function and doesn't seem like it's used anymore
    // UDPN's function Creates an IPX game select dialog window
    return FALSE;
  }

  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiSendExternalMessage(LPCSTR senderpath, LPCSTR sendername, LPCSTR targetpath, LPCSTR targetname, LPCSTR message)
  {
    DropMessage(0, "spiSendExternalMessage");
    // This function is complete
    return FALSE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  BOOL __stdcall spiLeagueGetName(LPSTR leaguebuffer, DWORD leaguechars)
  {
    DropMessage(0, "spiLeagueGetName");
    // This function is complete
    return TRUE;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  SNETSPI spiFunctions = {
        sizeof(SNETSPI),
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
  /*n*/ &SNP::spiStartAdvertisingGame,
  /*n*/ &SNP::spiStopAdvertisingGame,
  /*e*/ &SNP::spiUnlockDeviceList,
        &SNP::spiUnlockGameList,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
  /*n*/ &SNP::spiLeagueGetName
  };
}
