#include "GameImpl.h"
#include <vector>
#include <string>
#include <sstream>

#include "../Detours.h"
#include "../DLLMain.h"

#include "BWtoBWAPI.h"
#include "UnitImpl.h"
#include "PlayerImpl.h"
#include "BulletImpl.h"
#include "RegionImpl.h"
#include "Command.h"

#include "../BW/CBullet.h"
#include "../BW/CUnit.h"
#include "../BW/Dialog.h"
#include "../BW/Offsets.h"

/*
  This files holds all functions of the GameImpl class that are not part of the Game interface.
 */

namespace BWAPI4
{
  GameImpl BroodwarImpl;

  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  GameImpl::GameImpl()
  {
    BWtoBWAPI_init();

    // iterate through players and create PlayerImpl for each
    for (u8 i = 0; i < BW::PLAYER_COUNT; ++i)
      players[i] = new PlayerImpl(i);

    // iterate through units and create UnitImpl for each
    for (u16 i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; ++i)
      unitArray[i] = new UnitImpl(&BW::BWDATA::UnitNodeTable[i], i);

    // iterate through bullets and create BulletImpl for each
    for (u16 i = 0; i < BW::BULLET_ARRAY_MAX_LENGTH; ++i)
      bulletArray[i] = new BulletImpl(&BW::BWDATA::BulletNodeTable[i], i);

    this->initializeData();
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  GameImpl::~GameImpl()
  {
    this->initializeData();

    // destroy all UnitImpl
    for (UnitImpl* u : unitArray)
    {
      if (u) delete u;
    }
    unitArray.fill(nullptr);

    // destroy all PlayerImpl
    for (int i = 0; i < std::extent<decltype(players)>::value; ++i)
    {
      if (players[i]) delete players[i];
      players[i] = nullptr;
    }

    // destroy all bullets
    for (BulletImpl* b : bulletArray)
    {
      if (b) delete b;
    }
    bulletArray.fill(nullptr);
  }
  //---------------------------------------- REFRESH SELECTION STATES ----------------------------------------
  void GameImpl::refreshSelectionStates()
  {
    for (UnitImpl* u : unitArray)
    {
      if (u) u->setSelected(false);
    }

    selectedUnitSet.clear();
    for (int i = 0; i < BW::BWDATA::ClientSelectionCount && i < BW::MAX_SELECTION_COUNT; ++i)
    {
      BWAPI4::UnitImpl *u = UnitImpl::BWUnitToBWAPIUnit(BW::BWDATA::ClientSelectionGroup[i]);
      if (u)
      {
        u->setSelected(true);
        selectedUnitSet.insert(u);
      }
    }
  }
  void GameImpl::dropPlayers()
  {
    for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
    {
      if (BW::BWDATA::playerStatusArray[i] & 0x10000)
      {
        int iplr = this->stormIdToPlayerId(i);
        if (iplr != -1 && iplr != BW::BWDATA::g_LocalHumanID)
        {
          this->droppedPlayers.push_back(this->players[iplr]);
          SNetDropPlayer(i, 0x40000006);  // The value used when dropping
        }
      }
    }
  }
  //------------------------------------------------ MOUSE/KEY INPUT -----------------------------------------
  void GameImpl::pressKey(int key)
  {
    // Don't do anything if key is 0
    // used when auto-menu dialogs are not found, performance
    if (!key)
      return;

    // Press and release the key
    PostMessage(SDrawGetFrameWindow(), WM_CHAR, (WPARAM)key, NULL);
  }
  //------------------------------------------- PLAYER ID CONVERT --------------------------------------------
  int GameImpl::stormIdToPlayerId(int dwStormId)
  {
    /* Translates a storm ID to a player Index */
    for (int i = 0; i < BW::PLAYER_COUNT; ++i)
    {
      if (BW::BWDATA::Players[i].dwStormId == dwStormId)
        return i;
    }
    return -1;
  }
  //----------------------------------------------- PARSE TEXT -----------------------------------------------
  bool GameImpl::parseText(const std::string &text)
  {
    // analyze the string
    std::stringstream ss(text);
    std::string cmd;
    int n;

    ss >> cmd;

    // commands list
    if (cmd == "/leave")
    {
      this->leaveGame();
    }
    else if (cmd == "/speed")
    {
      n = -1;
      ss >> n;
      setLocalSpeedDirect(n);
      BroodwarImpl.printf("Changed game speed");
    }
    else if (cmd == "/fs")
    {
      n = 1;
      ss >> n;
      setFrameSkip(n);
      BroodwarImpl.printf("Altered frame skip");
    }
    else if (cmd == "/cheats")
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
    else if (cmd == "/restart")
    {
      restartGame();
    }
    else if (cmd == "/nogui")
    {
      setGUI(!data->hasGUI);
      BroodwarImpl.printf("GUI: %s", data->hasGUI ? "enabled" : "disabled");
    }
    else if (cmd == "/grid")
    {
      grid = !grid;
      BroodwarImpl.printf("Matrix grid %s", grid ? "enabled" : "disabled");
    }
    else if (cmd == "/fps")
    {
      this->showfps = !this->showfps;
      BroodwarImpl.printf("FPS display %s", showfps ? "enabled" : "disabled");
    }
    else
    {
      return false;
    }
    return true;
  }
  //------------------------------------------- GET PLAYER INTERNAL ------------------------------------------
  PlayerImpl *GameImpl::_getPlayer(int id)
  {
    if (id < 0 || id >= BW::PLAYER_COUNT)
      return players[BW::PLAYER_COUNT - 1];
    return players[id];
  }
  int GameImpl::_currentPlayerId()
  {
    return BW::BWDATA::g_LocalHumanID;
  }
  void GameImpl::initializeData()
  {
    // Delete forces
    for (auto f = this->forces.begin(); f != this->forces.end(); ++f)
      delete *f;
    this->forces.clear();

    // Remove player references
    this->BWAPIPlayer = nullptr;
    this->enemyPlayer = nullptr;

    // Set random seed
    srand(GetTickCount());

    // clear all sets
    this->aliveUnits.clear();
    this->dyingUnits.clear();
    this->discoverUnits.clear();
    this->accessibleUnits.clear();
    this->evadeUnits.clear();
    this->selectedUnitSet.clear();
    this->startLocations.clear();
    this->playerSet.clear();
    this->minerals.clear();
    this->geysers.clear();
    this->neutralUnits.clear();
    this->bullets.clear();
    this->pylons.clear();
    this->staticMinerals.clear();
    this->staticGeysers.clear();
    this->staticNeutralUnits.clear();
    this->_allies.clear();
    this->_enemies.clear();
    this->_observers.clear();

    // Reset saved selection
    this->savedUnitSelection.fill(nullptr);
    this->wantSelectionUpdate = false;

    // Disable all game flags
    flags.fill(false);

    // Clear the latency buffer
    this->commandBuffer.clear();
    this->commandBuffer.reserve(16);

    // Delete all dead units
    for (Unitset::iterator d = this->deadUnits.begin(); d != this->deadUnits.end(); ++d)
      delete static_cast<UnitImpl*>(*d);
    this->deadUnits.clear();

    // Delete all regions
    for (auto r = this->regionsList.begin(); r != this->regionsList.end(); ++r)
      delete *r;
    this->regionsList.clear();
    this->regionMap.clear();

    // Reset game speeds and text size
    this->setLocalSpeedDirect(std::numeric_limits<int>::min());
    this->setFrameSkip(1);
    this->setTextSize();
    this->setGUI(true);

    // Reset all Unit objects in the unit array
    for (UnitImpl* u : unitArray)
    {
      if (!u) continue;
      u->clear();
      u->userSelected = false;
      u->isAlive = false;
      u->wasAlive = false;
      u->wasCompleted = false;
      u->wasAccessible = false;
      u->wasVisible = false;
      u->nukeDetected = false;
      u->lastType = UnitTypes::Unknown;
      u->lastPlayer = nullptr;

      u->setID(-1);
    }

    BulletImpl::nextId = 0;
    this->cheatFlags = 0;
    //this->frameCount  = -1;
    this->frameCount = 0;

    //reload auto menu data (in case the AI set the location of the next map/replay)
    this->loadAutoMenuData();

    //clear everything in the server
    this->server.clearAll();

    // clear messages so they are not stored until the next match
    this->sentMessages.clear();

    // Some other variables
    apmCounter.init();
    fpsCounter.init();
    shapes.clear();

    this->startedClient = false;
  }

  void GameImpl::queueSentMessage(std::string const &message)
  {
    if (!message.empty())
      this->sentMessages.emplace_back(message);
  }
};
