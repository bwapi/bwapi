#include "SNPModule.h"

#include "CriticalSection.h"
#include <Util\MemoryFrame.h>

#include <queue>
#include <list>

namespace SNP
{
  //------------------------------------------------------------------------------------------------------------------------------------
  Network<SOCKADDR> *pluggedNetwork = NULL;

  bool fatalError = false;

  clientInfo gameAppInfo;

  CriticalSection critSec;
  CriticalSection::Lock *critSecExLock = NULL;
#define INTERLOCKED CriticalSection::Lock critSecLock(critSec);

  struct GamePacket
  {
    SOCKADDR  sender;
    int       packetSize;
    DWORD     timeStamp;
    char      data[512];
  };
  std::queue<GamePacket> incomingGamePackets;
  HANDLE receiveEvent;

  struct AdFile
  {
    gameStruc gameInfo;
    char extraBytes[32];
  };
  std::list<AdFile> gameList;
  int nextGameAdID = 1;

  AdFile hostedGame;
//------------------------------------------------------------------------------------------------------------------------------------
  void passAdvertisement(const SOCKADDR& host, Util::MemoryFrame ad)
  {
    INTERLOCKED;

    // find if the list already has an ad from this host
    AdFile *adFile = NULL;
    for(std::list<AdFile>::iterator currAd = gameList.begin(); currAd != gameList.end(); currAd++ )
    {
      // if peer IDs equal
      if(!memcmp(&currAd->gameInfo.saHost, &host, sizeof(SOCKADDR)))
      {
        adFile = &(*currAd);
        break;
      }
    }

    // if the ad is from a new host, create a new adFile
    if( !adFile )
    {
      AdFile g;
      gameList.push_back(g);
      adFile = &gameList.back();
      adFile->gameInfo.dwIndex = ++nextGameAdID;
    }

    // init the new entry
    Util::MemoryFrame::from(adFile->gameInfo).writeAs(ad.readAs<gameStruc>());
    Util::MemoryFrame::from(adFile->extraBytes).write(ad);
    adFile->gameInfo.dwTimer = GetTickCount();
    adFile->gameInfo.saHost  = host;
    adFile->gameInfo.pExtra  = adFile->extraBytes;
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
    gamePacket.sender     = sender;
    gamePacket.timeStamp  = GetTickCount();

    incomingGamePackets.push(gamePacket);
    SetEvent(receiveEvent);
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiInitialize(clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, HANDLE hEvent)
  {
    // Called when the module is loaded
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
      MessageBox(NULL, e.getMessage().c_str(), "ERROR", MB_ICONERROR | MB_OK);
      return false;
    }
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiDestroy()
  {
    // called when you leave back to the network module selection menu
    try
    {
      pluggedNetwork->destroy();
    }
    catch(GeneralException &e)
    {
      fatalError = true;
      MessageBox(NULL, e.getMessage().c_str(), "ERROR", MB_ICONERROR | MB_OK);
      return false;
    }
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiLockGameList(int a1, int a2, gameStruc **ppGameList)
  {
    critSecExLock = new CriticalSection::Lock(critSec);
    // Storm locks the game list to access it
    // interlink gamelist entries (for storm)
    AdFile *lastAd = NULL;
    for(std::list<AdFile>::iterator currAd = gameList.begin(); currAd != gameList.end(); currAd++ )
    {
      currAd->gameInfo.pExtra = currAd->extraBytes;
      if(lastAd)
        lastAd->gameInfo.pNext = &currAd->gameInfo;
      lastAd = &(*currAd);
    }
    if(lastAd)
      lastAd->gameInfo.pNext = NULL;

    // remove outdated entries
    std::list<AdFile>::iterator nextAd = gameList.begin();
    std::list<AdFile>::iterator currAd;
    while ( nextAd != gameList.end() )
    {
      currAd = nextAd;
      nextAd++;

      if(GetTickCount() > currAd->gameInfo.dwTimer + 2000)
        gameList.erase(currAd);
    }

    try
    { // return game list
      if( !gameList.empty() )
        *ppGameList = &gameList.begin()->gameInfo;
      else
        *ppGameList = NULL;
    }
    catch(GeneralException &e)
    {
      fatalError = true;
      MessageBox(NULL, e.getMessage().c_str(), "ERROR", MB_ICONERROR | MB_OK);
      return false;
    }
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  DWORD gdwLastTickCount;
  bool __stdcall spiUnlockGameList(gameStruc *pGameList, DWORD *a2)
  {
    // when storm is done reading from the gamelist
    // release critical section
    delete critSecExLock;
    critSecExLock = NULL;

    if ( a2 )
      *a2 = 300;

    DWORD dwThisTickCount = GetTickCount();
    if ( dwThisTickCount - gdwLastTickCount <= 200 )
      return true;
    gdwLastTickCount = dwThisTickCount;

    try
    {
      pluggedNetwork->requestAds();
    }
    catch(GeneralException &e)
    {
      fatalError = true;
      MessageBox(NULL, e.getMessage().c_str(), "ERROR", MB_ICONERROR | MB_OK);
      return false;
    }
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStatString, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, void *pExtraBytes, DWORD dwExtraBytesCount)
  {
    INTERLOCKED;
    // Begin game advertisement; Called when you create a game
    memset(&hostedGame, 0, sizeof(hostedGame));
    strcpy_s(hostedGame.gameInfo.szGameName,       sizeof(hostedGame.gameInfo.szGameName),        pszGameName);
    strcpy_s(hostedGame.gameInfo.szGameStatString, sizeof(hostedGame.gameInfo.szGameStatString),  pszGameStatString);
    hostedGame.gameInfo.dwGameState = dwGameState;
    hostedGame.gameInfo.dwProduct   = gameAppInfo.dwProduct;
    hostedGame.gameInfo.dwVersion   = gameAppInfo.dwVerbyte;
    hostedGame.gameInfo.dwUnk_1C    = 0x0050;
    hostedGame.gameInfo.dwUnk_24    = 0x00a7;

    memcpy(hostedGame.extraBytes, pExtraBytes, dwExtraBytesCount);
    hostedGame.gameInfo.dwExtraBytes = dwExtraBytesCount;
    hostedGame.gameInfo.pExtra = hostedGame.extraBytes;

    pluggedNetwork->startAdvertising(Util::MemoryFrame::from(hostedGame));
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiStopAdvertisingGame()
  {
    INTERLOCKED;
    // Called when you stop hosting a game
    pluggedNetwork->stopAdvertising();
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiGetGameInfo(DWORD dwFindIndex, char *pszFindGameName, int a3, gameStruc *pGameResult)
  {
    INTERLOCKED;
    // returns game info for the game we are about to join
    // search for the game based on the gamename or index
    std::list<AdFile>::iterator currAd = gameList.begin();
    for(; currAd != gameList.end(); currAd++ )
    {
      if(currAd->gameInfo.dwIndex == dwFindIndex)
      {
        *pGameResult = currAd->gameInfo;
        return true;
      }
    }
    // not found
    SetLastError(STORM_ERROR_GAME_NOT_FOUND);
    return false;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiSend(DWORD addrCount, SOCKADDR * *addrList, char *buf, DWORD bufLen)
  {
    if ( !addrCount )
      return true;

    try
    {
      // support for 1 peer for now
      SOCKADDR him = *(addrList[0]);

      // send packet over the network module
      pluggedNetwork->sendAsyn(him, Util::MemoryFrame(buf, bufLen));
    }
    catch(GeneralException &e)
    {
      MessageBox(NULL, e.getMessage().c_str(), "ERROR", MB_ICONERROR | MB_OK);
      return false;
    }
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiReceive(SOCKADDR **senderPeer, char **data, DWORD *databytes)
  {
    INTERLOCKED;
    // Passes pointers from queued receive data to storm
    *senderPeer = NULL;
    *data       = NULL;
    *databytes  = 0;

    try
    {
      pluggedNetwork->receive();

      while(true)
      {
        // check if packets available
        if(!incomingGamePackets.size())
        {
          SetLastError(STORM_ERROR_NO_MESSAGES_WAITING);
          return false;
        }

        // save the packet before removing it from queue
        GamePacket *loan = new GamePacket();
        *loan = incomingGamePackets.front();
        incomingGamePackets.pop();

        // paket outdated?
        if( GetTickCount() > loan->timeStamp + 10000 )
        {
          //Sleep(1);
          continue;
        }

        // give saved data to storm
        *senderPeer =&loan->sender;
        *data       = loan->data;
        *databytes  = loan->packetSize;
        break;
      }
    }
    catch(GeneralException &e)
    {
      MessageBox(NULL, e.getMessage().c_str(), "ERROR", MB_ICONERROR | MB_OK);
      return false;
    }
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiFree(SOCKADDR * addr, char *data, DWORD databytes)
  {
    INTERLOCKED;
    // called after spiReceive, to free the reserved memory
    BYTE *loan = (BYTE*)addr;
    if ( loan )
      delete loan;
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiCompareNetAddresses(SOCKADDR * addr1, SOCKADDR * addr2, DWORD *dwResult)
  {
    INTERLOCKED;

    if ( dwResult )
      *dwResult = 0;
    if ( !addr1 || !addr2 || !dwResult )
    {
      SErrSetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }

    *dwResult = (0 == memcmp(&addr1, &addr2, sizeof(SOCKADDR)));
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------

//
// the following functions are useless to us but are called by Storm, so we need working stubs
//

  bool __stdcall spiLockDeviceList(DWORD *a1)
  {
    *a1 = 0;
    return true;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiUnlockDeviceList() { return true; }
  bool __stdcall spiFreeExternalMessage(SOCKADDR * addr, char *data, DWORD databytes) { return false; }
  bool __stdcall spiGetPerformanceData(DWORD dwType, DWORD *dwResult, int a3, int a4) { return false; }
  bool __stdcall spiInitializeDevice(int a1, void *a2, void *a3, DWORD *a4, void *a5) { return false; }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiReceiveExternalMessage(SOCKADDR * *addr, char **data, DWORD *databytes)
  {
    if ( addr )
      *addr = NULL;
    if ( data )
      *data = NULL;
    if ( databytes )
      *databytes = 0;
    SetLastError(STORM_ERROR_NO_MESSAGES_WAITING);
    return false;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  bool __stdcall spiSelectGame(int a1, clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, int a6) { return false; }
  bool __stdcall spiSendExternalMessage(int a1, int a2, int a3, int a4, int a5) { return false; }
  bool __stdcall spiLeagueGetName(char *pszDest, DWORD dwSize) { return true; }
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
