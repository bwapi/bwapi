#include "Server.h"

#include <cstdio>
#include <ctime>
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
#include <BW/Offsets.h>

#include "../Config.h"
#include "../../../svnrev.h"

#include "../../../Debug.h"

namespace BWAPI
{
  const int PIPE_TIMEOUT = 3000;
  const int PIPE_SYSTEM_BUFFER_SIZE = 4096;

  const BWAPI::GameInstance GameInstance_None(0, false, 0);
  Server::Server()
  {
    // Local variables
    const DWORD processID = GetCurrentProcessId();

    // Try to open the game table
    gameTableFileHandle = CreateFileMappingA( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(GameTable), "Local\\bwapi_shared_memory_game_list" );
    DWORD dwFileMapErr = GetLastError();
    if ( gameTableFileHandle )
    {
      gameTable = static_cast<GameTable*>(MapViewOfFile(gameTableFileHandle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(GameTable)));

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
            if (gameTable->gameInstances[i].serverProcessID == 0)
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
    mapFileHandle = CreateFileMappingA( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(GameData), ssShareName.str().c_str() );
    if ( mapFileHandle )
      data = static_cast<GameData*>(MapViewOfFile(mapFileHandle, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(GameData)));

    // check if memory was created or if we should create it locally
    if ( !data )
    {
      data = new GameData;
      localOnly = true;
    }
    initializeSharedMemory();

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
      
    pipeObjectHandle = CreateNamedPipeA(communicationPipe.str().c_str(),
                                        PIPE_ACCESS_DUPLEX,
                                        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                                        PIPE_UNLIMITED_INSTANCES,
                                        PIPE_SYSTEM_BUFFER_SIZE,
                                        PIPE_SYSTEM_BUFFER_SIZE,
                                        PIPE_TIMEOUT,
                                        &sa);
  }
  Server::~Server()
  {
    if ( pipeObjectHandle && pipeObjectHandle != INVALID_HANDLE_VALUE )
      DisconnectNamedPipe(pipeObjectHandle);

    if ( localOnly && data )
    {
      delete data;
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

      BroodwarImpl.events.clear();
      if (!BroodwarImpl.startedClient)
        checkForConnections();
    }
    // Reset data going out to client
    data->eventCount = 0;
    data->eventStringCount = 0;
  }
  bool Server::isConnected() const
  {
    return connected;
  }
  int Server::addString(const char* text)
  {
    StrCopy(data->eventStrings[data->eventStringCount], text);
    return data->eventStringCount++;
  }
  int Server::addEvent(const BWAPI::Event& e)
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
    data->client_version   = CLIENT_VERSION;
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
    data->self          = getPlayerID(BroodwarImpl.self());
    data->enemy         = getPlayerID(BroodwarImpl.enemy());
    data->neutral       = getPlayerID(BroodwarImpl.neutral());
    data->isMultiplayer = BroodwarImpl.isMultiplayer();
    data->isBattleNet   = BroodwarImpl.isBattleNet();
    data->isReplay      = BroodwarImpl.isReplay();

    // Locally store the map size
    TilePosition mapSize(BroodwarImpl.mapWidth(), BroodwarImpl.mapHeight() );
    WalkPosition mapWalkSize( mapSize );

    // Load walkability
    for ( int x = 0; x < mapWalkSize.x; ++x )
      for ( int y = 0; y < mapWalkSize.y; ++y )
      {
        data->isWalkable[x][y] = BroodwarImpl.isWalkable(x, y);
      }

    // Load buildability, ground height, tile region id
    for ( int x = 0; x < mapSize.x; ++x )
      for ( int y = 0; y < mapSize.y; ++y )
      {
        data->isBuildable[x][y] = BroodwarImpl.isBuildable(x, y);
        data->getGroundHeight[x][y] = BroodwarImpl.getGroundHeight(x, y);
        if (BW::BWDATA::SAIPathing )
          data->mapTileRegionId[x][y] = BW::BWDATA::SAIPathing->mapTileRegionId[y][x];
        else
          data->mapTileRegionId[x][y] = 0;
      }

    // Load pathing info
    if ( BW::BWDATA::SAIPathing )
    {
      data->regionCount = BW::BWDATA::SAIPathing->regionCount;
      for(int i = 0; i < 5000; ++i)
      {
        data->mapSplitTilesMiniTileMask[i] = BW::BWDATA::SAIPathing->splitTiles[i].minitileMask;
        data->mapSplitTilesRegion1[i] = BW::BWDATA::SAIPathing->splitTiles[i].rgn1;
        data->mapSplitTilesRegion2[i] = BW::BWDATA::SAIPathing->splitTiles[i].rgn2;

        BWAPI::Region r = BroodwarImpl.getRegion(i);
        if (r)
        {
          data->regions[i] = static_cast<RegionImpl*>(r)->getData();
        }
        else
        {
          MemZero(data->regions[i]);
        }
      }
    }

    // Store the map size
    data->mapWidth  = mapSize.x;
    data->mapHeight = mapSize.y;

    // Retrieve map strings
    StrCopy(data->mapFileName, BroodwarImpl.mapFileName());
    StrCopy(data->mapPathName, BroodwarImpl.mapPathName());
    StrCopy(data->mapName, BroodwarImpl.mapName());
    StrCopy(data->mapHash, BroodwarImpl.mapHash());

    data->startLocationCount = BroodwarImpl.getStartLocations().size();
    int idx = 0;
    for (TilePosition t : BroodwarImpl.getStartLocations())
    {
      data->startLocations[idx].x = t.x;
      data->startLocations[idx].y = t.y;
      idx++;
    }

    //static force data
    data->forces[0].name[0] = '\0';
    for(Force i : BroodwarImpl.getForces())
    {
      int id = getForceID(i);
      StrCopy(data->forces[id].name, i->getName());
    }

    //static player data
    for(Player i : BroodwarImpl.getPlayers())
    {
      int id = getPlayerID(i);
      PlayerData* p = &(data->players[id]);
      PlayerData &p2 = static_cast<PlayerImpl*>(i)->data;

      StrCopy(p->name, i->getName());
      p->race = i->getRace();
      p->type = i->getType();
      p->force = getForceID(i->getForce());
      p->color = p2.color;

      for(int j = 0; j < 12; ++j)
      {
        p->isAlly[j] = false;
        p->isEnemy[j] = false;
      }
      for(Player j : BroodwarImpl.getPlayers())
      {
        p->isAlly[getPlayerID(j)] = i->isAlly(j);
        p->isEnemy[getPlayerID(j)] = i->isEnemy(j);
      }
      p->isNeutral = i->isNeutral();
      p->startLocationX = i->getStartLocation().x;
      p->startLocationY = i->getStartLocation().y;
    }

    data->forceCount = forceVector.size();
    data->playerCount = playerVector.size();
    data->initialUnitCount = unitVector.size();

    data->botAPM_noselects = 0;
    data->botAPM_selects = 0;
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
    for (Unit u : BroodwarImpl.evadeUnits)
      data->units[getUnitID(u)] = static_cast<UnitImpl*>(u)->data;

    data->frameCount              = BroodwarImpl.getFrameCount();
    data->replayFrameCount        = BroodwarImpl.getReplayFrameCount();
    data->randomSeed              = BroodwarImpl.getRandomSeed();
    data->fps                     = BroodwarImpl.getFPS();
    data->botAPM_noselects        = BroodwarImpl.getAPM(false);
    data->botAPM_selects          = BroodwarImpl.getAPM(true);
    data->latencyFrames           = BroodwarImpl.getLatencyFrames();
    data->latencyTime             = BroodwarImpl.getLatencyTime();
    data->remainingLatencyFrames  = BroodwarImpl.getRemainingLatencyFrames();
    data->remainingLatencyTime    = BroodwarImpl.getRemainingLatencyTime();
    data->elapsedTime             = BroodwarImpl.elapsedTime();
    data->countdownTimer          = BroodwarImpl.countdownTimer();
    data->averageFPS              = BroodwarImpl.getAverageFPS();
    data->mouseX                  = BroodwarImpl.getMousePosition().x;
    data->mouseY                  = BroodwarImpl.getMousePosition().y;
    data->isInGame                = BroodwarImpl.isInGame();
    if (BroodwarImpl.isInGame())
    {
      data->gameType  = BroodwarImpl.getGameType();
      
      // Copy the mouse states
      for(int i = 0; i < M_MAX; ++i)
        data->mouseState[i]  = BroodwarImpl.getMouseState((MouseButton)i);
      
      // Copy the key states
      for(int i = 0; i < K_MAX; ++i)
        data->keyState[i]  = BroodwarImpl.getKeyState((Key)i);

      // Copy the screen position
      data->screenX  = BroodwarImpl.getScreenPosition().x;
      data->screenY  = BroodwarImpl.getScreenPosition().y;

      for ( int i = 0; i < BWAPI::Flag::Max; ++i )
        data->flags[i] = BroodwarImpl.isFlagEnabled(i);

      data->isPaused = BroodwarImpl.isPaused();
      data->selectedUnitCount = BroodwarImpl.getSelectedUnits().size();

      int idx = 0;
      for(Unit t : BroodwarImpl.getSelectedUnits())
        data->selectedUnits[idx++] = getUnitID(t);

      //dynamic map data
      Map::copyToSharedMemory();
      //(no dynamic force data)

      //dynamic player data
      for(Player i : BroodwarImpl.getPlayers())
      {
        int id         = getPlayerID(i);
        if ( id >= 12 )
          continue;
        PlayerData* p  = &(data->players[id]);
        PlayerData &p2 = static_cast<PlayerImpl*>(i)->data;

        p->isVictorious     = i->isVictorious();
        p->isDefeated       = i->isDefeated();
        p->leftGame         = i->leftGame();
        p->minerals         = p2.minerals;
        p->gas              = p2.gas;
        p->gatheredMinerals = p2.gatheredMinerals;
        p->gatheredGas      = p2.gatheredGas;
        p->repairedMinerals = p2.repairedMinerals;
        p->repairedGas      = p2.repairedGas;
        p->refundedMinerals = p2.refundedMinerals;
        p->refundedGas      = p2.refundedGas;
        for(int j = 0; j < 3; ++j)
        {
          p->supplyTotal[j]  = p2.supplyTotal[j];
          p->supplyUsed[j]  = p2.supplyUsed[j];
        }
        for(int j = 0; j < UnitTypes::Enum::MAX; ++j)
        {
          p->allUnitCount[j]        = p2.allUnitCount[j];
          p->visibleUnitCount[j]    = p2.visibleUnitCount[j];
          p->completedUnitCount[j]  = p2.completedUnitCount[j];
          p->deadUnitCount[j]       = p2.deadUnitCount[j];
          p->killedUnitCount[j]     = p2.killedUnitCount[j];
        }
        p->totalUnitScore     = p2.totalUnitScore;
        p->totalKillScore     = p2.totalKillScore;
        p->totalBuildingScore = p2.totalBuildingScore;
        p->totalRazingScore   = p2.totalRazingScore;
        p->customScore        = p2.customScore;

        for(int j = 0; j < 63; ++j)
        {
          p->upgradeLevel[j] = p2.upgradeLevel[j];
          p->isUpgrading[j]  = p2.isUpgrading[j];
        }

        for(int j = 0; j < 47; ++j)
        {
          p->hasResearched[j] = p2.hasResearched[j];
          p->isResearching[j] = p2.isResearching[j];
        }
        memcpy(p->isResearchAvailable, p2.isResearchAvailable, sizeof(p->isResearchAvailable));
        memcpy(p->isUnitAvailable, p2.isUnitAvailable, sizeof(p->isUnitAvailable));
        memcpy(p->maxUpgradeLevel, p2.maxUpgradeLevel, sizeof(p->maxUpgradeLevel));
      }

      //dynamic unit data
      for(Unit i : BroodwarImpl.getAllUnits())
        data->units[getUnitID(i)] = static_cast<UnitImpl*>(i)->data;

      for(int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; ++i)
      {
        Unit u = BroodwarImpl.indexToUnit(i);
        int id = -1;
        if ( u )
          id = getUnitID(u);
        data->unitArray[i] = id;
      }

      unitFinder* xf = data->xUnitSearch;
      unitFinder* yf = data->yUnitSearch;
      const BW::unitFinder* bwxf = BW::BWDATA::UnitOrderingX.data();
      const BW::unitFinder* bwyf = BW::BWDATA::UnitOrderingY.data();
      int bwSearchSize = BW::BWDATA::UnitOrderingCount;

      for ( int i = 0; i < bwSearchSize; ++i, bwxf++, bwyf++ )
      {
        if (bwxf->unitIndex > 0 && bwxf->unitIndex <= BW::UNIT_ARRAY_MAX_LENGTH)
        {
          UnitImpl* u = BroodwarImpl.unitArray[bwxf->unitIndex-1];
          if ( u && u->canAccess() )
          {
            xf->searchValue = bwxf->searchValue;
            xf->unitIndex = getUnitID(u);
            xf++;
          }
        } // x index

        if (bwyf->unitIndex > 0 && bwyf->unitIndex <= BW::UNIT_ARRAY_MAX_LENGTH)
        {
          UnitImpl* u = BroodwarImpl.unitArray[bwyf->unitIndex-1];
          if ( u && u->canAccess() )
          {
            yf->searchValue = bwyf->searchValue;
            yf->unitIndex = getUnitID(u);
            yf++;
          }
        } // x index

      } // loop unit finder
      
      // Set size
      data->unitSearchSize = xf - data->xUnitSearch; // we assume an equal number of y values was put into the array
      

      //dynamic bullet data
      for(int id = 0; id < 100; ++id)
        data->bullets[id] = BroodwarImpl.getBulletFromIndex(id)->data;
      
      //dynamic nuke dot data
      int j = 0;
      data->nukeDotCount = BroodwarImpl.getNukeDots().size();
      for(Position const &nd : BroodwarImpl.getNukeDots())
      {
        data->nukeDots[j].x = nd.x;
        data->nukeDots[j].y = nd.y;
        ++j;
      }
    }

    // iterate events
    for (Event &e : BroodwarImpl.events)
    {
      if (e.getType() == EventType::MatchStart)
      {
        onMatchStart();
      }

      // Add the event to the server queue
      addEvent(e);

      // ignore if tournament AI not loaded
      if (!BroodwarImpl.tournamentAI)
        continue;

      // call the tournament module callbacks for server/client
      BroodwarImpl.isTournamentCall = true;
      GameImpl::SendClientEvent(BroodwarImpl.tournamentAI, e);
      BroodwarImpl.isTournamentCall = false;
    }
    BroodwarImpl.events.clear();
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
    if (forceVector.size() <= static_cast<unsigned>(id))
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
    if (playerVector.size() <= static_cast<unsigned>(id))
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
    if (unitVector.size() <= static_cast<unsigned>(id))
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
        if (BroodwarImpl.isInGame())
          BroodwarImpl.setScreenPosition(v1,v2);
        break;
      case BWAPIC::CommandType::PingMinimap:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.pingMinimap(v1,v2);
        break;
      case BWAPIC::CommandType::EnableFlag:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.enableFlag(v1);
        break;
      case BWAPIC::CommandType::Printf:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.printf("%s", data->strings[v1]);
        break;
      case BWAPIC::CommandType::SendText:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.sendTextEx(v2 != 0, "%s", data->strings[v1]);
        break;
      case BWAPIC::CommandType::PauseGame:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.pauseGame();
        break;
      case BWAPIC::CommandType::ResumeGame:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.resumeGame();
        break;
      case BWAPIC::CommandType::LeaveGame:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.leaveGame();
        break;
      case BWAPIC::CommandType::RestartGame:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.restartGame();
        break;
      case BWAPIC::CommandType::SetLocalSpeed:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.setLocalSpeed(v1);
        break;
      case BWAPIC::CommandType::SetLatCom:
        BroodwarImpl.setLatCom(v1 == 1);
        break;
      case BWAPIC::CommandType::SetGui:
        BroodwarImpl.setGUI(v1 == 1);
        break;
      case BWAPIC::CommandType::SetFrameSkip:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.setFrameSkip(v1);
        break;
      case BWAPIC::CommandType::SetMap:
        BroodwarImpl.setMap(data->strings[v1]);
        break;
      case BWAPIC::CommandType::SetAllies:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.setAlliance(getPlayer(v1), v2 != 0, v2 == 2);
        break;
      case BWAPIC::CommandType::SetVision:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.setVision(getPlayer(v1), v2 != 0);
        break;
      case BWAPIC::CommandType::SetCommandOptimizerLevel:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.setCommandOptimizationLevel(v1);
        break;
      case BWAPIC::CommandType::SetRevealAll:
        if (BroodwarImpl.isInGame())
          BroodwarImpl.setRevealAll(v1 != 0);
        break;
      default:
        break;
      }
    }
    if (BroodwarImpl.isInGame())
    {
      for ( int i = 0; i < data->unitCommandCount; ++i )
      {
        if (data->unitCommands[i].unitIndex < 0 || data->unitCommands[i].unitIndex >= (int)unitVector.size())
          continue;
        Unit unit = unitVector[data->unitCommands[i].unitIndex];
        Unit target = nullptr;
        if (data->unitCommands[i].targetIndex >= 0 && data->unitCommands[i].targetIndex < (int)unitVector.size())
          target = unitVector[data->unitCommands[i].targetIndex];

        unit->issueCommand(UnitCommand(unit, data->unitCommands[i].type, target, data->unitCommands[i].x, data->unitCommands[i].y, data->unitCommands[i].extra));
      }
    } // if isInGame
  }

}