#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include "GameImpl.h"
#include <vector>
#include <string>
#include <Util/Strings.h>

#include "WMode.h"
#include "Detours.h"
#include "DLLMain.h"
#include "Recording.h"
#include "Resolution.h"

#include <BWAPI/BWtoBWAPI.h>
#include <BWAPI/UnitImpl.h>
#include <BWAPI/PlayerImpl.h>
#include <BWAPI/BulletImpl.h>

#include <BW/Bullet.h>

#include "../../Debug.h"

/*
  This files holds all functions of the GameImpl class that are not part of the Game interface.
 */

namespace BWAPI
{
  Game* Broodwar;
  GameImpl BroodwarImpl;

  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  GameImpl::GameImpl()
      : onStartCalled(false)
      , unitsOnTileData(0, 0)
      , enabled(true)
      , client(NULL)
      , startedClient(false)
      , inGame(false)
      , frameCount(-1)
      , endTick(0)
      , pathDebug(false)
      , unitDebug(false)
      , grid(false)
      , wantSelectionUpdate(false)
      , calledMatchEnd(false)
      , autoMapTryCount(0)
      , outOfGame(true)
      , lastAutoMapEntry(0)
      , tournamentAI(NULL)
      , tournamentController(NULL)
      , isTournamentCall(false)
      , commandOptimizerLevel(0)
      , lastEventTime(0)
      , data(server.data)
      , BWAPIPlayer(NULL)
      , enemyPlayer(NULL)
      , bulletCount(0)
      , myDlg(NULL)
      , bTournamentMessageAppeared(false)
      , autoMenuEnemyCount(0)
      , autoMenuMinPlayerCount(0)
      , autoMenuMaxPlayerCount(0)
      , autoMenuWaitPlayerTime(0)
      , accumulatedFrames(0)
      , fps(0)
      , averageFPS(0)
      , botAPM_selects(0)
      , botAPM_noselects(0)
      , botAPMCounter_selects(0)
      , botAPMCounter_noselects(0)
      , textSize(1)
      , externalModuleConnected(false)
  {
    MemZero(savedUnitSelection);
    MemZero(flags);

    BWAPI::Broodwar = static_cast<Game*>(this);

    BWtoBWAPI_init();
    try
    {
      /* iterate through players and create PlayerImpl for each */
      for (int i = 0; i < PLAYER_COUNT; ++i)
        players[i] = new PlayerImpl((u8)i);

      /* iterate through units and create UnitImpl for each */
      for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
        unitArray[i] = new UnitImpl(&BW::BWDATA_UnitNodeTable[i], (u16)i);

      /* iterate through bullets and create BulletImpl for each */
      for (int i = 0; i < BULLET_ARRAY_MAX_LENGTH; ++i)
        bulletArray[i] = new BulletImpl(&BW::BWDATA_BulletNodeTable[i], (u16)i);
    }
    catch (GeneralException& exception)
    {
      BWAPIError("Exception caught inside Game constructor: %s", exception.getMessage().c_str());
    }
    srand(GetTickCount());
    gszDesiredReplayName[0] = 0;
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  GameImpl::~GameImpl()
  {
    /* destroy all UnitImpl */
    for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
      delete unitArray[i];

    /* destroy all PlayerImpl */
    for (int i = 0; i < PLAYER_COUNT; ++i)
      delete players[i];

    /* destroy all bullets */
    for(int i = 0; i < BULLET_ARRAY_MAX_LENGTH; ++i)
      delete bulletArray[i];
  }
  //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
  void GameImpl::refreshSelectionStates()
  {
    for (int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
      this->unitArray[i]->setSelected(false);

    selectedUnitSet.clear();
    for (int i = 0; i < *BW::BWDATA_ClientSelectionCount && i < 12; ++i)
    {
      if ( BW::BWDATA_ClientSelectionGroup[i] )
      {
        BWAPI::UnitImpl *u = UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA_ClientSelectionGroup[i]);
        u->setSelected(true);
        selectedUnitSet.insert(u);
      }
    }
  }
  //--------------------------------------------- IS BATTLE NET ----------------------------------------------
  bool GameImpl::_isBattleNet()
  {
    return *BW::BWDATA_NetMode == 'BNET';
  }
  //-------------------------------------------- IS SINGLE PLAYER --------------------------------------------
  bool GameImpl::_isSinglePlayer() const
  {
    return *BW::BWDATA_NetMode == 0 || *BW::BWDATA_NetMode == -1;
  }
  //------------------------------------------------ IS IN GAME ----------------------------------------------
  bool GameImpl::_isInGame() const
  {
    return *BW::BWDATA_InGame != 0;
  }
  //----------------------------------------------- IN REPLAY ------------------------------------------------
  bool  GameImpl::_isReplay() const
  {
    return *BW::BWDATA_InReplay != 0;
  }
  //------------------------------------------------ MOUSE/KEY INPUT -----------------------------------------
  void GameImpl::pressKey(int key)
  {
    // Don't do anything if key is 0
    // used when auto-menu dialogs are not found, performance
    if ( !key )
      return;

    // Press and release the key
    PostMessage(SDrawGetFrameWindow(), WM_KEYDOWN, (WPARAM)key, NULL);
    PostMessage(SDrawGetFrameWindow(), WM_KEYUP,   (WPARAM)key, 0xC0000000);
  }
  void GameImpl::mouseDown(int x, int y)
  {
    // Press the left mouse button
    PostMessage(SDrawGetFrameWindow(), WM_LBUTTONDOWN, NULL, (LPARAM)MAKELONG(x,y));
  }
  void GameImpl::mouseUp(int x, int y)
  {
    // Release the left mouse button
    PostMessage(SDrawGetFrameWindow(), WM_LBUTTONUP, NULL, (LPARAM)MAKELONG(x,y));
  }
  //------------------------------------------- PLAYER ID CONVERT --------------------------------------------
  int GameImpl::stormIdToPlayerId(int dwStormId)
  {
    /* Translates a storm ID to a player Index */
    for (int i = 0; i < PLAYER_COUNT; ++i)
    {
      if ( BW::BWDATA_Players[i].dwStormId == dwStormId )
        return i;
    }
    return -1;
  }
  //----------------------------------------------- PARSE TEXT -----------------------------------------------
  bool GameImpl::parseText(const char* text)
  {
    /* analyze the string */
    std::string message = text;
    std::vector<std::string> parsed = Util::Strings::splitString(message, " ");

    /* fix for bad referencing */
    while (parsed.size() < 5)
      parsed.push_back("");

    /* commands list */
    if (parsed[0] == "/leave")
    {
      this->leaveGame();
    }
    else if (parsed[0] == "/speed")
    {
      if (parsed[1] != "")
        setLocalSpeed(atoi(parsed[1].c_str()));
      else
        setLocalSpeed();
      printf("Changed game speed");
    }
    else if (parsed[0] == "/fs")
    {
      if (parsed[1] != "")
        setFrameSkip(atoi(parsed[1].c_str()));
      else
        setFrameSkip();
      printf("Altered frame skip");
    }
    else if (parsed[0] == "/cheats")
    {
      sendText("power overwhelming");
      sendText("operation cwal");
      sendText("the gathering");
      sendText("medieval man");
      sendText("black sheep wall");
      sendText("food for thought");
      sendText("modify the phase variance");
      sendText("something for nothing");
      sendText("something for nothing");
      sendText("something for nothing");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
      sendText("show me the money");
    }
    else if (parsed[0] == "/restart")
    {
      restartGame();
    }
    else if (parsed[0] == "/nogui")
    {
      setGUI(!data->hasGUI);
      printf("GUI: %s.", data->hasGUI ? "enabled" : "disabled");
    }
    else if (parsed[0] == "/wmode")
    {
      SetWMode(BW::BWDATA_GameScreenBuffer->wid, BW::BWDATA_GameScreenBuffer->ht, !wmode);
      printf("Toggled windowed mode.");
    }
    else if (parsed[0] == "/grid")
    {
      grid = !grid;
      printf("Matrix grid %s.", grid ? "enabled" : "disabled");
    }
    else if (parsed[0] == "/record")
    {
      if ( !StartVideoRecording(640, 480) )
        MessageBox(NULL, "Recording failed to start.", "Recording failed!", MB_OK | MB_ICONHAND);
    }
    else if (parsed[0] == "/stoprecord")
    {
      StopVideoRecording();
    }
#ifdef _DEBUG
    else if (parsed[0] == "/latency")
    {
      printf("latency: %d", getLatency());
      printf("New latency: %u frames; %ums", getLatencyFrames(), getLatencyTime());
    }
    else if (parsed[0] == "/dlgdebug")
    {
      if ( !myDlg )
      {
        // Create the dialog window
        myDlg = BW::CreateDialogWindow("Test Dialog", 100, 100, 300, 200);

        // Add additional controls to the window
        BW::dialog *test = new BW::dialog(BW::ctrls::cLIST, 1, "testing123", 12, 16, myDlg->width() - 24, myDlg->height() - 56);
        myDlg->addControl(test);
        test->setFlags(CTRL_PLAIN | CTRL_FONT_SMALLEST | CTRL_BTN_NO_SOUND);
        myDlg->setFlags(CTRL_UPDATE | CTRL_DLG_ACTIVE);

        // Initialize the dialog
        myDlg->initialize();
    
        // Add entries to the combo/list box
        test->addListEntry("Test");
        test->addListEntry("Test2");
        test->addListEntry("Test3");
        test->addListEntry("Test4");
        test->addListEntry("Test5");
        test->addListEntry("Test6");
        test->addListEntry("Test7");
        test->addListEntry("Test8");
        test->addListEntry("Test9");
        test->addListEntry("Test10");
        test->addListEntry("Test11");
        test->addListEntry("Test12");
        test->addListEntry("Test13");
        test->addListEntry("Test14");
        test->setSelectedIndex(test->getListCount()-1);
      }
    }
// The following commands are knockoffs of Starcraft Beta's developer mode
    else if (parsed[0] == "/pathdebug")
    {
      pathDebug = !pathDebug;
      printf("pathdebug %s", pathDebug ? "ENABLED" : "DISABLED");
    }
    else if (parsed[0] == "/unitdebug")
    {
      unitDebug = !unitDebug;
      printf("unitdebug %s", unitDebug ? "ENABLED" : "DISABLED");
    }
// end knockoffs
    else if (parsed[0] == "/hud")
    {
      hideHUD = !hideHUD;
      printf("Now %s the HUD.", hideHUD ? "hiding" : "showing");
    }
    else if (parsed[0] == "/resize")
    {
      printf("Done");
      SetResolution(1024, 768);
    }
    else if (parsed[0] == "/test")
    {
      //SetResolution(640, 480);
      //printf("%u", this->elapsedTime());
      printf("Setting revealAll");
      if ( !this->setRevealAll() )
        printf("%s", this->getLastError().c_str());
      //int blah = 0;
      //printf("%u", 32/blah);
    }
#endif
    else
    {
      return false;
    }
    return true;
  }
  //------------------------------------------ copy Map To Shared Memory -------------------------------------
  void GameImpl::copyMapToSharedMemory()
  {
    map.copyToSharedMemory();
  }
  //------------------------------------------- GET PLAYER INTERNAL ------------------------------------------
  PlayerImpl *GameImpl::_getPlayer(int id)
  {
    if ( id < 0 || id >= PLAYER_COUNT )
      return players[PLAYER_COUNT - 1];
    return players[id];
  }
  int GameImpl::_currentPlayerId()
  {
    return (int)*BW::BWDATA_g_LocalHumanID;
  }
  bool GameImpl::tournamentCheck(int type, void *parameter)
  {
    if ( this->tournamentController && !isTournamentCall )
    {
      isTournamentCall  = true;
      bool allow        = this->tournamentController->onAction(type, parameter);
      isTournamentCall  = false;
      return allow;
    }
    return true;
  }

};