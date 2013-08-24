#include "Server.h"

#include <cstdio>
#include <ctime>
#include <Util/Foreach.h>
#include <Util/Convenience.h>
#include <cassert>
#include <sstream>
#include <AclAPI.h>

#include "GameImpl.h"
#include "PlayerImpl.h"
#include "UnitImpl.h"
#include "BulletImpl.h"
#include "RegionImpl.h"
#include <BWAPI/Client/GameData.h>
#include <BWAPI/Client/GameTable.h>

#include <BW/Pathing.h>

#include "../Config.h"
#include "../../../svnrev.h"

#include "../../../Debug.h"

namespace BWAPI
{
  #define PIPE_TIMEOUT 3000
  #define PIPE_SYSTEM_BUFFER_SIZE 4096

  const BWAPI::GameInstance GameInstance_None(0xFFFFFFFF, false, 0);
  Server::Server()
    : connected(false)
    , localOnly(false)
    , data(nullptr)
    , gameTable(nullptr)
    , gameTableIndex(-1)
    , mapFileHandle(nullptr)
    , pEveryoneSID(NULL)
    , pACL(NULL)
    , pSD(NULL)
  {
    // Local variables
    int size  = sizeof(GameData);
    DWORD processID = GetCurrentProcessId();

    if ( serverEnabled )
    {
      // Try to open the game table
      gameTableFileHandle = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(GameTable), "Local\\bwapi_shared_memory_game_list" );
      DWORD dwFileMapErr = GetLastError();
      if ( gameTableFileHandle )
      {
        gameTable = (GameTable*) MapViewOfFile(gameTableFileHandle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(GameTable));

        if ( gameTable )
        {
          if ( dwFileMapErr != ERROR_ALREADY_EXISTS )
          {
            // If we created it, initialize it
            for(int i = 0; i < GameTable::MAX_GAME_INSTANCES; ++i)
              gameTable->gameInstances[i] = GameInstance_None;
          } // If does not already exist

          // Check to see if we are already in the table
          for(int i = 0; i < GameTable::MAX_GAME_INSTANCES; ++i)
          {
            if (gameTable->gameInstances[i].serverProcessID == processID)
            {
              gameTableIndex = i;
              break;
            }
          }
          // If not, try to find an empty row
          if (gameTableIndex == -1)
          {
            for(int i = 0; i < GameTable::MAX_GAME_INSTANCES; ++i)
            {
              if (gameTable->gameInstances[i].serverProcessID == 0xFFFFFFFF)
              {
                gameTableIndex = i;
                break;
              }
            }
          }
          // If we can't find an empty row, take over the row with the oldest keep alive time
          if (gameTableIndex == -1)
          {
            DWORD oldest = gameTable->gameInstances[0].lastKeepAliveTime;
            gameTableIndex = 0;
            for(int i = 1; i < GameTable::MAX_GAME_INSTANCES; ++i)
            {
              if (gameTable->gameInstances[i].lastKeepAliveTime < oldest)
              {
                oldest = gameTable->gameInstances[i].lastKeepAliveTime;
                gameTableIndex = i;
              }
            }
          }
          //We have a game table index now, initialize our row
          gameTable->gameInstances[gameTableIndex].serverProcessID = processID;
          gameTable->gameInstances[gameTableIndex].isConnected = false;
          gameTable->gameInstances[gameTableIndex].lastKeepAliveTime = GetTickCount();
        } // if gameTable
      } // if gameTableFileHandle

      // Create the share name
      std::stringstream ssShareName;
      ssShareName << "Local\\bwapi_shared_memory_";
      ssShareName << processID;

      // Create the file mapping and shared memory
      mapFileHandle = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, ssShareName.str().c_str() );
      if ( mapFileHandle )
        data = (GameData*)MapViewOfFile(mapFileHandle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, size);
    } // if serverEnabled

    // check if memory was created or if we should create it locally
    if ( !data )
    {
      data      = (GameData*)malloc(size);
      localOnly = true;
    }
    initializeSharedMemory();

    if ( serverEnabled )
    {
	    //--------------------------------------------------------------------------------------------------------
	    // Security Structure hobbled together from this document:
	    // http://msdn.microsoft.com/en-us/library/aa446595%28VS.85%29.aspx
	    //

	    this->pEveryoneSID = NULL;
	    SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
	    
      // Create a well-known SID for the Everyone group.
      if( !AllocateAndInitializeSid( &SIDAuthWorld, 
                                    1,
                                    SECURITY_WORLD_RID,
                                    0, 0, 0, 0, 0, 0, 0,
                                    &this->pEveryoneSID) )
      {
        // AllocateAndInitializeSid failed
        //Util::Logger::globalLog->log("Error: AllocateAndInitializeSid");
		    //printf("AllocateAndInitializeSid Error %u\n", GetLastError());
      }

      // Initialize an EXPLICIT_ACCESS structure for an ACE.
      // The ACE will allow Everyone access.
      EXPLICIT_ACCESS ea = {};
      ea.grfAccessPermissions  = GENERIC_ALL;
	    ea.grfAccessMode         = GRANT_ACCESS;
      ea.grfInheritance        = NO_INHERITANCE;
      ea.Trustee.TrusteeForm   = TRUSTEE_IS_SID;
      ea.Trustee.TrusteeType   = TRUSTEE_IS_WELL_KNOWN_GROUP;
      ea.Trustee.ptstrName     = (LPTSTR)this->pEveryoneSID;

	    this->pACL = NULL;  //a NULL DACL is assigned to the security descriptor, which allows all access to the object

      // Create a new ACL that contains the new ACEs.
      DWORD dwRes = SetEntriesInAcl(1, &ea, NULL, &this->pACL);
      if (ERROR_SUCCESS != dwRes) 
      {
        // SetEntriesInAcl failed
        //Util::Logger::globalLog->log("Error: SetEntriesInAcl");
		    //printf("SetEntriesInAcl Error %u\n", GetLastError());
      }

      // Initialize a security descriptor.  
      this->pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH); 
      if ( NULL == this->pSD ) 
      { 
        // LocalAlloc failed
        //Util::Logger::globalLog->log("Error: LocalAlloc");
		    //printf("LocalAlloc Error %u\n", GetLastError()); 
      } 
 
      if ( !InitializeSecurityDescriptor(this->pSD, SECURITY_DESCRIPTOR_REVISION) ) 
      {
        // InitializeSecurityDescriptor failed
        //Util::Logger::globalLog->log("Error: InitializeSecurityDescriptor");
		    //printf("InitializeSecurityDescriptor Error %u\n",GetLastError()); 
      } 

	    // Add the ACL to the security descriptor. 
      if ( !SetSecurityDescriptorDacl(this->pSD, 
									                    TRUE,     // bDaclPresent flag   
									                    this->pACL, 
									                    FALSE) )   // not a default DACL 
      {
        // SetSecurityDescriptorDacl failed
		    //Util::Logger::globalLog->log("Error: InitializeSecurityDescriptor");
		    //printf("SetSecurityDescriptorDacl Error %u\n",GetLastError());
      } 

      // Initialize a security attributes structure.
      SECURITY_ATTRIBUTES sa = { 0 };
	    sa.nLength = sizeof(sa);
      sa.lpSecurityDescriptor = this->pSD;
      sa.bInheritHandle = FALSE;
	    //--------------------------------------------------------------------------------------------------------

      std::stringstream communicationPipe;
      communicationPipe << "\\\\.\\pipe\\bwapi_pipe_";
      communicationPipe << processID;
      
      pipeObjectHandle = CreateNamedPipe(communicationPipe.str().c_str(),
                                         PIPE_ACCESS_DUPLEX,
                                         PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                                         PIPE_UNLIMITED_INSTANCES,
                                         PIPE_SYSTEM_BUFFER_SIZE,
                                         PIPE_SYSTEM_BUFFER_SIZE,
                                         PIPE_TIMEOUT,
                                         &sa);
    }
  }
  Server::~Server()
  {
    if ( pipeObjectHandle && pipeObjectHandle != INVALID_HANDLE_VALUE )
      DisconnectNamedPipe(pipeObjectHandle);

    if ( localOnly && data )
    {
      free(data);
      data = nullptr;
    }

    if ( this->pEveryoneSID )
      FreeSid(this->pEveryoneSID);
    if ( this->pACL )
      LocalFree(this->pACL);
    if ( this->pSD )
      LocalFree(this->pSD);
  }
  void Server::update()
  {
    // Reset data coming in to server
    data->stringCount      = 0;
    data->commandCount     = 0;
    data->unitCommandCount = 0;
    data->shapeCount       = 0;
    if (gameTable && gameTableIndex >= 0)
    {
      gameTable->gameInstances[gameTableIndex].lastKeepAliveTime = GetTickCount();
      gameTable->gameInstances[gameTableIndex].isConnected = connected;
    }
    if (connected)
    {
      // Update BWAPI Client
      updateSharedMemory();
      callOnFrame();
      processCommands();
    }
    else
    {
      // Update BWAPI DLL
      BroodwarImpl.processEvents();

      static_cast<GameImpl*>(BroodwarPtr)->events.clear();
      if (!static_cast<GameImpl*>(BroodwarPtr)->startedClient)
        checkForConnections();
    }
    // Reset data going out to client
    data->eventCount = 0;
    data->eventStringCount = 0;
  }
  bool Server::isConnected()
  {
    return connected;
  }
  int Server::addString(const char* text)
  {
    StrCopy(data->eventStrings[data->eventStringCount], text);
    return data->eventStringCount++;
  }
  int Server::addEvent(BWAPI::Event e)
  {
    assert(data->eventCount < GameData::MAX_EVENTS);
    BWAPIC::Event* e2 = &(data->events[data->eventCount++]);
    int id   = data->eventCount;
    e2->type = e.getType();
    e2->v1   = 0;
    e2->v2   = 0;
    switch (e.getType())
    {
    case BWAPI::EventType::MatchEnd:
      e2->v1 = e.isWinner();
      break;
    case BWAPI::EventType::SendText:
    case BWAPI::EventType::SaveGame:
      e2->v1 = addString(e.getText().c_str());
      break;
    case BWAPI::EventType::PlayerLeft:
      e2->v1 = getPlayerID(e.getPlayer());
      break;
    case BWAPI::EventType::ReceiveText:
      e2->v1  = getPlayerID(e.getPlayer());
      e2->v2  = addString(e.getText().c_str());
      break;
    case BWAPI::EventType::NukeDetect:
      e2->v1 = e.getPosition().x;
      e2->v2 = e.getPosition().y;
      break;
    case BWAPI::EventType::UnitDiscover:
    case BWAPI::EventType::UnitEvade:
    case BWAPI::EventType::UnitCreate:
    case BWAPI::EventType::UnitDestroy:
    case BWAPI::EventType::UnitMorph:
    case BWAPI::EventType::UnitShow:
    case BWAPI::EventType::UnitHide:
    case BWAPI::EventType::UnitRenegade:
    case BWAPI::EventType::UnitComplete:
      e2->v1 = getUnitID(e.getUnit());
      break;
    default:
      break;
    }
    return id;
  }

  void Server::setWaitForResponse(bool wait)
  {
    if ( !pipeObjectHandle || pipeObjectHandle == INVALID_HANDLE_VALUE )
      return;

    DWORD dwMode = PIPE_READMODE_MESSAGE | (wait ? PIPE_WAIT : PIPE_NOWAIT);
    SetNamedPipeHandleState(pipeObjectHandle, &dwMode, NULL, NULL);
  }
  void Server::checkForConnections()
  {
    if (connected || localOnly || !pipeObjectHandle || pipeObjectHandle == INVALID_HANDLE_VALUE )
      return;
    BOOL success = ConnectNamedPipe(pipeObjectHandle, nullptr);
    if (!success && GetLastError() != ERROR_PIPE_CONNECTED)
      return;
    if (GetLastError() == ERROR_PIPE_CONNECTED)
      connected = true;
    if (!connected)
      return;
    setWaitForResponse(true);
  }
  void Server::initializeSharedMemory()
  {
    //called once when Starcraft starts. Not at the start of every match.
    data->instanceID       = gdwProcNum;
    data->revision         = SVN_REV;
    data->isDebug          = (BUILD_DEBUG == 1);
    data->eventCount       = 0;
    data->eventStringCount = 0;
    data->commandCount     = 0;
    data->unitCommandCount = 0;
    data->shapeCount       = 0;
    data->stringCount      = 0;
    data->mapFileName[0]   = 0;
    data->mapPathName[0]   = 0;
    data->mapName[0]       = 0;
    data->mapHash[0]       = 0;
    data->hasGUI           = true;
    data->hasLatCom        = true;
    clearAll();
  }
  void Server::onMatchStart()
  {
    data->self          = getPlayerID(Broodwar->self());
    data->enemy         = getPlayerID(Broodwar->enemy());
    data->neutral       = getPlayerID(Broodwar->neutral());
    data->isMultiplayer = Broodwar->isMultiplayer();
    data->isBattleNet   = Broodwar->isBattleNet();
    data->isReplay      = Broodwar->isReplay();

    // Locally store the map size
    TilePosition mapSize( Broodwar->mapWidth(), Broodwar->mapHeight() );
    WalkPosition mapWalkSize( mapSize );

    // Load walkability
    for ( int x = 0; x < mapWalkSize.x; ++x )
      for ( int y = 0; y < mapWalkSize.y; ++y )
      {
        data->isWalkable[x][y] = Broodwar->isWalkable(x, y);
      }

    // Load buildability, ground height, tile region id
    for ( int x = 0; x < mapSize.x; ++x )
      for ( int y = 0; y < mapSize.y; ++y )
      {
        data->isBuildable[x][y]     = Broodwar->isBuildable(x, y);
        data->getGroundHeight[x][y] = Broodwar->getGroundHeight(x, y);
        if ( *BW::BWDATA::SAIPathing )
          data->mapTileRegionId[x][y] = (*BW::BWDATA::SAIPathing)->mapTileRegionId[y][x];
        else
          data->mapTileRegionId[x][y] = 0;
      }

    // Load pathing info
    if ( *BW::BWDATA::SAIPathing )
    {
      data->regionCount = (*BW::BWDATA::SAIPathing)->regionCount;
      for(int i = 0; i < 5000; ++i)
      {
        data->mapSplitTilesMiniTileMask[i] = (*BW::BWDATA::SAIPathing)->splitTiles[i].minitileMask;
        data->mapSplitTilesRegion1[i]      = (*BW::BWDATA::SAIPathing)->splitTiles[i].rgn1;
        data->mapSplitTilesRegion2[i]      = (*BW::BWDATA::SAIPathing)->splitTiles[i].rgn2;
        // Region hack using a possibly unused variable (feeling risky, babe :) )
        if ( (*BW::BWDATA::SAIPathing)->regions[i].unk_28 )
          data->regions[i] = *((RegionImpl*)(*BW::BWDATA::SAIPathing)->regions[i].unk_28)->getData();
        else
          MemZero(data->regions[i]);
      }
    }

    // Store the map size
    data->mapWidth  = mapSize.x;
    data->mapHeight = mapSize.y;

    // Retrieve map strings
    StrCopy(data->mapFileName, Broodwar->mapFileName());
    StrCopy(data->mapPathName, Broodwar->mapPathName());
    StrCopy(data->mapName, Broodwar->mapName());
    StrCopy(data->mapHash, Broodwar->mapHash());

    data->startLocationCount = Broodwar->getStartLocations().size();
    int i = 0;
    foreach(TilePosition t, Broodwar->getStartLocations())
    {
      data->startLocations[i].x = t.x;
      data->startLocations[i].y = t.y;
      i++;
    }

    //static force data
    data->forces[0].name[0] = '\0';
    foreach(Force i, Broodwar->getForces())
    {
      int id = getForceID(i);
      StrCopy(data->forces[id].name, i->getName());
    }

    //static player data
    foreach(PlayerImpl *i, Broodwar->getPlayers())
    {
      int id = getPlayerID(i);
      PlayerData* p = &(data->players[id]);
      PlayerData* p2 = i->self;

      StrCopy(p->name, i->getName());
      p->race  = i->getRace();
      p->type  = i->getType();
      p->force = getForceID(i->getForce());
      p->color = p2->color;

      for(int j = 0; j < 12; ++j)
      {
        p->isAlly[j]  = false;
        p->isEnemy[j] = false;
      }
      foreach(Player j, Broodwar->getPlayers())
      {
        p->isAlly[getPlayerID(j)]  = i->isAlly(j);
        p->isEnemy[getPlayerID(j)]  = i->isEnemy(j);
      }
      p->isNeutral    = i->isNeutral();
      p->startLocationX  = i->getStartLocation().x;
      p->startLocationY  = i->getStartLocation().y;
    }

    data->forceCount    = forceVector.size();
    data->playerCount    = playerVector.size();
    data->initialUnitCount  = unitVector.size();

    data->botAPM_noselects  = 0;
    data->botAPM_selects  = 0;
  }
  void Server::clearAll()
  {
    //clear force info
    data->forceCount = 0;
    forceVector.clear();
    forceLookup.clear();

    //clear player info
    data->playerCount = 0;
    playerVector.clear();
    playerLookup.clear();

    //clear unit info
    data->initialUnitCount = 0;
    unitVector.clear();
    unitLookup.clear();
  }

  void Server::updateSharedMemory()
  {
    bool matchStarting = false;
    
    // iterate events
    foreach(Event e, BroodwarImpl.events)
    {
      // Add the event to the server queue
      addEvent(e);
      if (e.getType() == EventType::MatchStart)
        matchStarting = true;

      // ignore if tournament AI not loaded
      if ( !BroodwarImpl.tournamentAI )
        continue;

      // call the tournament module callbacks for server/client
      BroodwarImpl.isTournamentCall = true;
      switch ( e.getType() )
      {
      case EventType::MatchStart:
        BroodwarImpl.tournamentAI->onStart();
        break;
      case EventType::MatchEnd:
        BroodwarImpl.tournamentAI->onEnd(e.isWinner());
        break;
      case EventType::MatchFrame:
        BroodwarImpl.tournamentAI->onFrame();
        break;
      case EventType::MenuFrame:
        break;
      case EventType::SendText:
        BroodwarImpl.tournamentAI->onSendText(e.getText());
        break;
      case EventType::ReceiveText:
        BroodwarImpl.tournamentAI->onReceiveText(e.getPlayer(), e.getText());
        break;
      case EventType::PlayerLeft:
        BroodwarImpl.tournamentAI->onPlayerLeft(e.getPlayer());
        break;
      case EventType::NukeDetect:
        BroodwarImpl.tournamentAI->onNukeDetect(e.getPosition());
        break;
      case EventType::UnitDiscover:
        BroodwarImpl.tournamentAI->onUnitDiscover(e.getUnit());
        break;
      case EventType::UnitEvade:
        BroodwarImpl.tournamentAI->onUnitEvade(e.getUnit());
        break;
      case EventType::UnitCreate:
        BroodwarImpl.tournamentAI->onUnitCreate(e.getUnit());
        break;
      case EventType::UnitDestroy:
        BroodwarImpl.tournamentAI->onUnitDestroy(e.getUnit());
        break;
      case EventType::UnitMorph:
        BroodwarImpl.tournamentAI->onUnitMorph(e.getUnit());
        break;
      case EventType::UnitShow:
        BroodwarImpl.tournamentAI->onUnitShow(e.getUnit());
        break;
      case EventType::UnitHide:
        BroodwarImpl.tournamentAI->onUnitHide(e.getUnit());
        break;
      case EventType::UnitRenegade:
        BroodwarImpl.tournamentAI->onUnitRenegade(e.getUnit());
        break;
      case EventType::SaveGame:
        BroodwarImpl.tournamentAI->onSaveGame(e.getText());
        break;
      case EventType::UnitComplete:
        BroodwarImpl.tournamentAI->onUnitComplete(e.getUnit());
        break;
      default:
        break;
      }
      BroodwarImpl.isTournamentCall = false;
    }
    foreach(UnitImpl* u, BroodwarImpl.lastEvadedUnits)
      data->units[u->getID()] = u->data;

    static_cast<GameImpl*>(BroodwarPtr)->events.clear();

    data->frameCount              = Broodwar->getFrameCount();
    data->replayFrameCount        = Broodwar->getReplayFrameCount();
    data->fps                     = Broodwar->getFPS();
    data->botAPM_noselects        = Broodwar->getAPM();
    data->botAPM_selects          = Broodwar->getAPM(true);
    data->latencyFrames           = Broodwar->getLatencyFrames();
    data->latencyTime             = Broodwar->getLatencyTime();
    data->remainingLatencyFrames  = Broodwar->getRemainingLatencyFrames();
    data->remainingLatencyTime    = Broodwar->getRemainingLatencyTime();
    data->elapsedTime             = Broodwar->elapsedTime();
    data->countdownTimer          = Broodwar->countdownTimer();
    data->averageFPS              = Broodwar->getAverageFPS();
    data->mouseX                  = Broodwar->getMousePosition().x;
    data->mouseY                  = Broodwar->getMousePosition().y;
    data->isInGame                = Broodwar->isInGame();
    if (Broodwar->isInGame())
    {
      data->gameType  = Broodwar->getGameType();
      data->latency   = Broodwar->getLatency();
      
      // Copy the mouse states
      for(int i = 0; i < M_MAX; ++i)
        data->mouseState[i]  = Broodwar->getMouseState((MouseButton)i);
      
      // Copy the key states
      for(int i = 0; i < K_MAX; ++i)
        data->keyState[i]  = Broodwar->getKeyState((Key)i);

      // Copy the screen position
      data->screenX  = Broodwar->getScreenPosition().x;
      data->screenY  = Broodwar->getScreenPosition().y;

      for ( int i = 0; i < BWAPI::Flag::Max; ++i )
        data->flags[i] = Broodwar->isFlagEnabled(i);
      data->isPaused          = Broodwar->isPaused();
      data->selectedUnitCount = Broodwar->getSelectedUnits().size();
      int i = 0;
      foreach(Unit t, Broodwar->getSelectedUnits())
        data->selectedUnits[i++] = getUnitID(t);

      //dynamic map data
      BroodwarImpl.copyMapToSharedMemory();
      //(no dynamic force data)

      //dynamic player data
      foreach(PlayerImpl *i, Broodwar->getPlayers())
      {
        int id         = getPlayerID(i);
        if ( id >= 12 )
          continue;
        PlayerData* p  = &(data->players[id]);
        PlayerData* p2 = i->self;

        p->isVictorious     = i->isVictorious();
        p->isDefeated       = i->isDefeated();
        p->leftGame         = i->leftGame();
        p->minerals         = p2->minerals;
        p->gas              = p2->gas;
        p->gatheredMinerals = p2->gatheredMinerals;
        p->gatheredGas      = p2->gatheredGas;
        p->repairedMinerals = p2->repairedMinerals;
        p->repairedGas      = p2->repairedGas;
        p->refundedMinerals = p2->refundedMinerals;
        p->refundedGas      = p2->refundedGas;
        for(int j = 0; j < 3; ++j)
        {
          p->supplyTotal[j]  = p2->supplyTotal[j];
          p->supplyUsed[j]  = p2->supplyUsed[j];
        }
        for(int j = 0; j < UnitTypes::Enum::MAX; ++j)
        {
          p->allUnitCount[j]        = p2->allUnitCount[j];
          p->visibleUnitCount[j]    = p2->visibleUnitCount[j];
          p->completedUnitCount[j]  = p2->completedUnitCount[j];
          p->deadUnitCount[j]       = p2->deadUnitCount[j];
          p->killedUnitCount[j]     = p2->killedUnitCount[j];
        }
        p->totalUnitScore     = p2->totalUnitScore;
        p->totalKillScore     = p2->totalKillScore;
        p->totalBuildingScore = p2->totalBuildingScore;
        p->totalRazingScore   = p2->totalRazingScore;
        p->customScore        = p2->customScore;

        for(int j = 0; j < 63; ++j)
        {
          p->upgradeLevel[j] = p2->upgradeLevel[j];
          p->isUpgrading[j]  = p2->isUpgrading[j];
        }

        for(int j = 0; j < 47; ++j)
        {
          p->hasResearched[j] = p2->hasResearched[j];
          p->isResearching[j] = p2->isResearching[j];
        }
        memcpy(p->isResearchAvailable, p2->isResearchAvailable, sizeof(p->isResearchAvailable));
        memcpy(p->isUnitAvailable, p2->isUnitAvailable, sizeof(p->isUnitAvailable));
        memcpy(p->maxUpgradeLevel, p2->maxUpgradeLevel, sizeof(p->maxUpgradeLevel));
      }

      //dynamic unit data
      foreach(UnitImpl *i, Broodwar->getAllUnits())
        data->units[i->getID()] = i->data;

      for(int i = 0; i < UNIT_ARRAY_MAX_LENGTH; ++i)
      {
        Unit u = Broodwar->indexToUnit(i);
        int id = -1;
        if ( u )
          id = u->getID();
        data->unitArray[i] = id;
      }

      unitFinder     *xf   = data->xUnitSearch;
      unitFinder     *yf   = data->yUnitSearch;
      const BW::unitFinder *bwxf = BW::BWDATA::UnitOrderingX;
      const BW::unitFinder *bwyf = BW::BWDATA::UnitOrderingY;
      int bwSearchSize = *BW::BWDATA::UnitOrderingCount;

      for ( int i = 0; i < bwSearchSize; ++i, bwxf++, bwyf++ )
      {
        if ( bwxf->unitIndex > 0 && bwxf->unitIndex <= UNIT_ARRAY_MAX_LENGTH )
        {
          UnitImpl *u = BroodwarImpl.unitArray[bwxf->unitIndex-1];
          if ( u && u->canAccess() )
          {
            xf->searchValue = bwxf->searchValue;
            xf->unitIndex   = u->getID();
            xf++;
          }
        } // x index

        if ( bwyf->unitIndex > 0 && bwyf->unitIndex <= UNIT_ARRAY_MAX_LENGTH )
        {
          UnitImpl *u = BroodwarImpl.unitArray[bwyf->unitIndex-1];
          if ( u && u->canAccess() )
          {
            yf->searchValue = bwyf->searchValue;
            yf->unitIndex   = u->getID();
            yf++;
          }
        } // x index

      } // loop unit finder
      
      // Set size
      data->unitSearchSize = xf - data->xUnitSearch; // we assume and equal number of y values was put into the array
      

      //dynamic bullet data
      for(int id = 0; id < 100; ++id)
        data->bullets[id] = BroodwarImpl.getBulletFromIndex(id)->data;
      
      //dynamic nuke dot data
      int j = 0;
      data->nukeDotCount = Broodwar->getNukeDots().size();
      foreach(Position nd, Broodwar->getNukeDots())
      {
        data->nukeDots[j].x = nd.x;
        data->nukeDots[j].y = nd.y;
        ++j;
      }

    }
    if (matchStarting)
      Server::onMatchStart();
  }

  int Server::getForceID(Force force)
  {
    if ( !force )
      return -1;
    if (forceLookup.find(force) == forceLookup.end())
    {
      forceLookup[force] = (int)(forceVector.size());
      forceVector.push_back(force);
    }
    return forceLookup[force];
  }
  Force Server::getForce(int id) const
  {
    if ((int)forceVector.size() <= id || id < 0)
      return nullptr;
    return forceVector[id];
  }
  int Server::getPlayerID(Player player)
  {
    if ( !player )
      return -1;
    if (playerLookup.find(player) == playerLookup.end())
    {
      playerLookup[player] = (int)(playerVector.size());
      playerVector.push_back(player);
    }
    return playerLookup[player];
  }
  Player Server::getPlayer(int id) const
  {
    if ((int)playerVector.size() <= id || id < 0)
      return nullptr;
    return playerVector[id];
  }

  int Server::getUnitID(Unit unit)
  {
    if ( !unit )
      return -1;
    if (unitLookup.find(unit) == unitLookup.end())
    {
      unitLookup[unit] = (int)(unitVector.size());
      unitVector.push_back(unit);
    }
    return unitLookup[unit];
  }
  Unit Server::getUnit(int id) const
  {
    if ((int)unitVector.size() <= id || id < 0)
      return nullptr;
    return unitVector[id];
  }

  void Server::callOnFrame()
  { 
    DWORD writtenByteCount;
    int code = 2;
    WriteFile(pipeObjectHandle, &code, sizeof(int), &writtenByteCount, NULL);
    while (code != 1)
    {
      DWORD receivedByteCount;
      BOOL success = ReadFile(pipeObjectHandle, &code, sizeof(int), &receivedByteCount,NULL);
      if (!success)
      {
        DisconnectNamedPipe(pipeObjectHandle);
        connected = false;
        setWaitForResponse(false);
        break;
      }
    }
  }
  void Server::processCommands()
  {
    for(int i = 0; i < data->commandCount; ++i)
    {
      BWAPIC::CommandType::Enum c = data->commands[i].type;
      int v1 = data->commands[i].value1;
      int v2 = data->commands[i].value2;
      switch (c)
      {
      case BWAPIC::CommandType::SetScreenPosition:
        if (Broodwar->isInGame())
          Broodwar->setScreenPosition(v1,v2);
        break;
      case BWAPIC::CommandType::PingMinimap:
        if (Broodwar->isInGame())
          Broodwar->pingMinimap(v1,v2);
        break;
      case BWAPIC::CommandType::EnableFlag:
        if (Broodwar->isInGame())
          Broodwar->enableFlag(v1);
        break;
      case BWAPIC::CommandType::Printf:
        if (Broodwar->isInGame())
          Broodwar->printf("%s", data->strings[v1]);
        break;
      case BWAPIC::CommandType::SendText:
        if (Broodwar->isInGame())
          Broodwar->sendText("%s", data->strings[v1]);
        break;
      case BWAPIC::CommandType::PauseGame:
        if (Broodwar->isInGame())
          Broodwar->pauseGame();
        break;
      case BWAPIC::CommandType::ResumeGame:
        if (Broodwar->isInGame())
          Broodwar->resumeGame();
        break;
      case BWAPIC::CommandType::LeaveGame:
        if (Broodwar->isInGame())
          Broodwar->leaveGame();
        break;
      case BWAPIC::CommandType::RestartGame:
        if (Broodwar->isInGame())
          Broodwar->restartGame();
        break;
      case BWAPIC::CommandType::SetLocalSpeed:
        if (Broodwar->isInGame())
          Broodwar->setLocalSpeed(v1);
        break;
      case BWAPIC::CommandType::SetLatCom:
        Broodwar->setLatCom(v1 == 1);
        break;
      case BWAPIC::CommandType::SetGui:
        Broodwar->setGUI(v1 == 1);
        break;
      case BWAPIC::CommandType::SetFrameSkip:
        if (Broodwar->isInGame())
          Broodwar->setFrameSkip(v1);
        break;
      case BWAPIC::CommandType::SetMap:
        Broodwar->setMap(data->strings[v1]);
        break;
      case BWAPIC::CommandType::SetAllies:
        if (Broodwar->isInGame())
          Broodwar->setAlliance(getPlayer(v1), v2 != 0, v2 == 2);
        break;
      case BWAPIC::CommandType::SetVision:
        if (Broodwar->isInGame())
          Broodwar->setVision(getPlayer(v1), v2 != 0);
        break;
      case BWAPIC::CommandType::SetCommandOptimizerLevel:
        if (Broodwar->isInGame())
          Broodwar->setCommandOptimizationLevel(v1);
        break;
      case BWAPIC::CommandType::SetRevealAll:
        if ( Broodwar->isInGame() )
          Broodwar->setRevealAll(v1 != 0);
        break;
      default:
        break;
      }
    }
    if ( Broodwar->isInGame() )
    {
      for ( int i = 0; i < data->unitCommandCount; ++i )
      {
        if (data->unitCommands[i].unitIndex < 0 || data->unitCommands[i].unitIndex >= (int)unitVector.size())
          continue;
        Unit unit   = unitVector[data->unitCommands[i].unitIndex];
        Unit target = nullptr;
        if (data->unitCommands[i].targetIndex >= 0 && data->unitCommands[i].targetIndex < (int)unitVector.size())
          target = unitVector[data->unitCommands[i].targetIndex];

        unit->issueCommand(UnitCommand(unit, data->unitCommands[i].type, target, data->unitCommands[i].x, data->unitCommands[i].y, data->unitCommands[i].extra));
      }
    } // if isInGame
  }

}