#pragma once

namespace ReplayTool
{
  #pragma pack(1)
  struct replay_resource
  {
    bool  isBroodwar;
    DWORD dwFrameCount;
    WORD  wCampaignID;
    struct _gameSeed
    {
      BYTE  bCommandID;
      DWORD dwRandSeed;
      BYTE  bPlayerbytes[8];
    } gameSeed;
    struct _netGame
    {
      DWORD dwUnknown;
      char  szCurrentPlayerName[24];
      DWORD dwGameFlags;
      WORD  wMapWidth;
      WORD  wMapHeight;
      BYTE  bActivePlayerCount;
      BYTE  bAvailableSlots;
      BYTE  bGameSpeed;
      BYTE  bGameState;
      BYTE  bGameType;
      BYTE  bUnknown;
      WORD  wGameSubtype;
      DWORD dwSeed; // unused
      WORD  wTileset;
      BYTE  bAutosaved;
      BYTE  bComputerPlayerCount;
      char  szGameName[25];
      char  szMapName[32];
      struct _gameTemplate
      {
        BYTE  bGameType;
        BYTE  bUnknown;
        WORD  wSubType;
        WORD  wSubTypeDisplay;
        WORD  wSubTypeLabel;
        BYTE  bVictoryCondition;
        BYTE  bResourceType;
        bool  bUseStandardUnitStats;
        BYTE  bFogOfWar; // unused
        BYTE  bStartingUnits;
        bool  bUseFixedPosition;
        BYTE  bUsageRestrictionFlags;
        bool  bAlliesAllowed;
        BYTE  bTeams;
        bool  bCheats;
        BYTE  bTournamentMode;
        DWORD dwVictoryConditionVal;
        DWORD dwResourcesVal;
        DWORD dwUnusedVal;
        BYTE  bExtra;
      } gameTemplateInfo;
    } networkGameHeader;
    struct _playerEntry
    {
      DWORD dwSlot;
      DWORD dwStormId;
      BYTE  nType;
      BYTE  nRace;
      BYTE  nTeam;
      char  szPlayerName[25];
    } players[12];
    DWORD dwPlayerColors[8];
    BYTE  bPlayerForceData[8];
  };
  #pragma pack()
}