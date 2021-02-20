#include "GameImpl.h"
#include <ctime>
#include <filesystem>

#include "../StringUtil.h"

#include "../Detours.h"

#include "../BW/Pathing.h"
#include "../BW/Offsets.h"

#include "PlayerImpl.h"
#include <BWAPI/Race.h>
#include "RegionImpl.h"

#include <BWAPI/PlayerType.h>
#include <BWAPI4/Event.h>
#include <BWAPI/Flag.h>

namespace BWAPI4
{
  //--------------------------------------------- ON GAME START ----------------------------------------------
  void GameImpl::onGameStart()
  {
    // This function is called at the start of every match
    this->initializeData();

    // Set the speed override
    this->setLocalSpeedDirect(this->speedOverride);

    // initialize the variables
    //this->frameCount      = 0;
    this->onStartCalled   = true;
    this->calledMatchEnd  = false;

    // pre-calculate the map hash
    Map::calculateMapHash();

    // Obtain Broodwar Regions
    if ( BW::BWDATA::SAIPathing )
    {
      u32 rgnCount = BW::BWDATA::SAIPathing->regionCount;
      // Iterate regions and insert into region list
      for (u32 i = 0; i < rgnCount; ++i)
      {
        RegionImpl* r = new BWAPI4::RegionImpl(i);
        this->regionsList.insert(r);
        this->regionMap[i] = r;
      }

      // Iterate regions again and update neighbor lists
      for ( RegionImpl* r : this->regionsList )
        r->UpdateRegionRelations();
    } // if SAI_Pathing

    // roughly identify which players can possibly participate in this game
    // iterate triggers for each player
    for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
    {
      // reset participation and resources
      if (this->players[i])
      {
        this->players[i]->setParticipating(false);
        this->players[i]->resetResources();
      }

      // First check if player owns a unit at start
      for (int u = 0; u < UnitTypes::None; ++u)
      {
        if (BW::BWDATA::Game.unitCounts.all[u][i])
        {
          if (this->players[i])
            this->players[i]->setParticipating();
          break;
        }
      }
    }

    for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
    {
      // Then iterate each trigger
      // checking if a unit can be created or given to the player later in the game
      for ( BW::VCListEntry<BW::Triggers::Trigger> *t = BW::BWDATA::TriggerVectors[i].begin; (u32)t != ~(u32)&BW::BWDATA::TriggerVectors[i].end && (u32)t != (u32)&BW::BWDATA::TriggerVectors[i].begin; t = t->next )
      {
        // check if trigger conditions can be met
        if ( t->container.conditionsCanBeMet() )
        {
          // check participation of players
          for (int p = 0; p < BW::PLAYABLE_PLAYER_COUNT; ++p)
          {
            // Don't bother checking for participation if the player doesn't exist
            // or if the player is already participating (NOT observing)
            if ( !this->players[p] || !this->players[p]->isObserver() )
              continue;
            // Check if trigger actions allow gameplay and set participation if it does.
            if ( t->container.actionsAllowGameplay(i, p) )
              this->players[p]->setParticipating();
          }
        } // conds can be met
      } // trigvector iterator
    } // playercount iterator


    if (BW::BWDATA::InReplay) // set replay flags
    {
      // Set every cheat flag to true
      for (int i = 0; i < Flag::Max; ++i)
        this->flags[i] = true;
    }
    else
    {
      // Get the current player
      BWAPI4::PlayerImpl *thisPlayer = this->_getPlayer(_currentPlayerId());
      if ( !thisPlayer )
        return;

      this->BWAPIPlayer = thisPlayer;

      // find the opponent player
      for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
      {
        if ( (this->players[i]->getType() == PlayerTypes::Computer ||
              this->players[i]->getType() == PlayerTypes::Player   ||
              this->players[i]->getType() == PlayerTypes::EitherPreferComputer) &&
             !this->players[i]->isObserver() &&
             thisPlayer->isEnemy(this->players[i]) )
          this->enemyPlayer = this->players[i];
      }
    }

    // get pre-race info
    BYTE bRaceInfo[12] = { 0 };
    BYTE bOwnerInfo[12] = { 0 };

    Storm::CFile file("staredit\\scenario.chk");
    if ( file.isValid() )
    {
      size_t filesize = file.size();
      void *pData = SMAlloc(filesize);
      if ( pData )
      {
        if ( file.read(pData, filesize) )
        {
          /// @TODO: This map chunk stuff should be done as an iterator of a map file
          struct _mapchunk
          {
            DWORD dwId;
            DWORD dwSize;
            BYTE  bData[1];
          } *mcptr;
          for ( mcptr = static_cast<_mapchunk*>(pData);
            reinterpret_cast<DWORD>(mcptr) < reinterpret_cast<DWORD>(pData) + filesize;
            mcptr = reinterpret_cast<_mapchunk*>(&mcptr->bData[mcptr->dwSize]) )
          {
            switch ( mcptr->dwId )
            {
            case MAKEFOURCC('S', 'I', 'D', 'E'):
              if ( mcptr->dwSize == 12 )
                memcpy(bRaceInfo, mcptr->bData, 12);
              break;
            case MAKEFOURCC('O', 'W', 'N', 'R'):
              if ( mcptr->dwSize == 12 )
                memcpy(bOwnerInfo, mcptr->bData, 12);
              break;
            }
          }
        }
        SMFree(pData);
      }
    }

    for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
    {
      // Skip Start Locations that don't exist
      if (BW::BWDATA::Game.startPositions[i] == Positions::Origin)
        continue;

      // If the game is UMS and player is observer and race is not (UserSelect OR invalid player type), skip
      if ( this->getGameType() == GameTypes::Use_Map_Settings &&
           this->players[i]->isObserver() &&
           (bRaceInfo[i] != Races::Enum::Select ||
           (bOwnerInfo[i] != PlayerTypes::Computer &&
            bOwnerInfo[i] != PlayerTypes::Player   &&
            bOwnerInfo[i] != PlayerTypes::EitherPreferComputer &&
            bOwnerInfo[i] != PlayerTypes::EitherPreferHuman)) )
        continue;

      // add start location
      startLocations.push_back(TilePosition(BW::BWDATA::Game.startPositions[i] - Position(64, 48)));
    }

    // Get Player Objects
    for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; ++i)
    {
      if ( this->players[i] &&
           BW::BWDATA::Players[i].nType != PlayerTypes::None &&
           BW::BWDATA::Players[i].nType <  PlayerTypes::Closed )
      {
        players[i]->setID(server.getPlayerID(players[i]));
        this->playerSet.insert(this->players[i]);
      }
    }

    if ( this->BWAPIPlayer )
    {
      for (Player p : players)
      {
        if ( p->leftGame() || p->isDefeated() || p == BWAPIPlayer )
          continue;
        if ( BWAPIPlayer->isAlly(p) )
          _allies.insert(p);
        if ( BWAPIPlayer->isEnemy(p) )
          _enemies.insert(p);
        if ( p->isObserver() )
          _observers.insert(p);
      }
    }

    if ( this->players[11] )
      this->playerSet.insert(this->players[11]);

    for (int p = 0; p < BW::PLAYABLE_PLAYER_COUNT; ++p) {
        if (this->players[p]) this->players[p]->force = nullptr;
    }

    // Get Force Objects, assign Force<->Player relations
    ForceImpl *pNeutralForce = new ForceImpl("");
    if (this->players[11])
    {
      pNeutralForce->players.insert(this->players[11]);
      this->players[11]->force = pNeutralForce;
    }

    for ( int f = 1; f <= 4; ++f )
    {
      ForceImpl *newForce = new ForceImpl( BW::BWDATA::Game.forceNames[f-1].data() );
      this->forces.insert( newForce );
      for (int p = 0; p < BW::PLAYABLE_PLAYER_COUNT; ++p)
      {
        if ( this->players[p] && BW::BWDATA::Players[p].nTeam == f )
        {
          this->players[p]->force = newForce;
          if ( BW::BWDATA::Players[p].nType != PlayerTypes::None &&
               BW::BWDATA::Players[p].nType <  PlayerTypes::Closed )
            newForce->players.insert(this->players[p]);
        }
      }
    }

    // Assign neutral force to players that do not have one
    for (int p = 0; p < BW::PLAYABLE_PLAYER_COUNT; ++p)
    {
      if ( this->players[p] && !this->players[p]->force )
        this->players[p]->force = pNeutralForce;
    }

    // Get info for replay naming
    rn_GameResult = "loss"; // Game is counted as lost by default

    if ( !this->isReplay() )
    {
      if ( BWAPIPlayer )
      {
        rn_BWAPIName = BWAPIPlayer->getName();
        rn_BWAPIRace = BWAPIPlayer->getRace().getName().substr(0, 1);
      }
      rn_MapName   = mapName().substr(0, 16);
      rn_AlliesNames.clear();
      rn_AlliesRaces.clear();
      rn_EnemiesNames.clear();
      rn_EnemiesRaces.clear();
      for ( Player p : this->_allies )
      {
        if ( p )
        {
          rn_AlliesNames += p->getName().substr(0, 6);
          rn_AlliesRaces += p->getRace().getName().substr(0, 1);
        }
      }
      for ( Player p : this->_enemies )
      {
        if ( p )
        {
          rn_EnemiesNames += p->getType() != PlayerTypes::Computer ? p->getName().substr(0, 6) : "Comp";
          rn_EnemiesRaces += p->getRace().getName().substr(0, 1);
        }
      }
    } // !isReplay
  }
  //--------------------------------------------------- ON SAVE ------------------------------------------------
  void GameImpl::onSaveGame(const char *name)
  {
    // called when the game is being saved
    events.push_back(Event::SaveGame(name));
  }
  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  void GameImpl::onSendText(const std::string &text)
  {
    if ( text.empty() ) return;

    if ( !parseText(text) )
    {
      if ( externalModuleConnected )
      {
        events.push_back(Event::SendText(text));
      }
      else if( isFlagEnabled(BWAPI4::Flag::UserInput) )
      {
        sendText("%s", text.c_str());
      }
    }
  }
  //---------------------------------------------- ON RECV TEXT ----------------------------------------------
  void GameImpl::onReceiveText(int playerId, const std::string &text)
  {
    // Do onReceiveText
    int realId = stormIdToPlayerId(playerId);
    if ( realId != -1 &&
         (!this->BWAPIPlayer ||
          realId != this->BWAPIPlayer->getIndex() ) &&
         this->isFlagEnabled(BWAPI4::Flag::UserInput) )
    {
      events.push_back(Event::ReceiveText(this->players[realId], text));
    }
  }
  void ignore_invalid_parameter(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t)
  {}
  //---------------------------------------------- ON GAME END -----------------------------------------------
  void GameImpl::onGameEnd()
  {
    //this is called at the end of every match
    if ( !this->onStartCalled )
      return;

    if ( autoMenuManager.autoMenuSaveReplay != "" && !this->isReplay() )
    {
      // Set replay envvars
      SetEnvironmentVariableA("BOTNAME",    rn_BWAPIName.c_str());
      SetEnvironmentVariableA("BOTNAME6",   rn_BWAPIName.substr(0,6).c_str());
      SetEnvironmentVariableA("BOTRACE",    rn_BWAPIRace.c_str());
      SetEnvironmentVariableA("MAP",        rn_MapName.c_str());
      SetEnvironmentVariableA("ALLYNAMES",  rn_AlliesNames.c_str());
      SetEnvironmentVariableA("ALLYRACES",  rn_AlliesRaces.c_str());
      SetEnvironmentVariableA("ENEMYNAMES", rn_EnemiesNames.c_str());
      SetEnvironmentVariableA("ENEMYRACES", rn_EnemiesRaces.c_str());
      SetEnvironmentVariableA("GAMERESULT", rn_GameResult.c_str ());

      // Expand environment strings to szInterPath
      char szTmpPath[MAX_PATH] = { 0 };
      ExpandEnvironmentStringsA(autoMenuManager.autoMenuSaveReplay.c_str(), szTmpPath, MAX_PATH);

      std::string pathStr(szTmpPath);

      // Double any %'s remaining in the string so that strftime executes correctly
      {
        size_t tmp = std::string::npos;
        while (tmp = pathStr.find_last_of('%', tmp - 1), tmp != std::string::npos)
          pathStr.insert(tmp, "%");
      }

      // Replace the placeholder $'s with %'s for the strftime call
      std::replace(pathStr.begin(), pathStr.end(), '$', '%');

      // Get time
      time_t tmpTime = std::time(nullptr);
      tm *timeInfo = std::localtime(&tmpTime);

      // Expand time strings, add a handler for this specific task to ignore errors in the format string
      // TODO: Replace with boost time format
      _invalid_parameter_handler old = _set_invalid_parameter_handler(&ignore_invalid_parameter);
      std::strftime(szTmpPath, sizeof(szTmpPath), pathStr.c_str(), timeInfo);
      _set_invalid_parameter_handler(old);
      pathStr = szTmpPath;

      // Remove illegal characters
      pathStr.erase(std::remove_if(pathStr.begin(), pathStr.end(),
                                   [](char c) {
                                     return iscntrl(reinterpret_cast<unsigned char&>(c)) || c == '?' || c == '*' ||
                                         c == '<' || c == '|' || c == '>' || c == '"';
                                   }), pathStr.end());

      std::filesystem::path parent_p = std::filesystem::path(pathStr).parent_path();
      std::filesystem::create_directories(parent_p);

      // Copy to global desired replay name
      gDesiredReplayName = pathStr;
    }

    if ( !this->calledMatchEnd )
    {
      this->calledMatchEnd = true;
      events.push_back(Event::MatchFrame());
      events.push_back(Event::MatchEnd(false));
      server.update();
      this->inGame = false;
      events.push_back(Event::MenuFrame());
      server.update();
    }

    // player-specific game end
    for (int i = 0; i < BW::PLAYER_COUNT; ++i)
      if ( this->players[i] )
        this->players[i]->onGameEnd();

    this->onStartCalled = false;

    this->initializeData();
    this->autoMenuManager.chooseNewRandomMap();
  }
}

