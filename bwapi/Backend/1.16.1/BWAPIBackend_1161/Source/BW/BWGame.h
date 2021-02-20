#pragma once
#include <array>
#include "../Types.h"
#include "Constants.h"
#include "Position.h"

namespace BW
{
  /// <summary>Broodwar super-structure that is used to hold primary game stats.
  /// This data is serialized entirely as-is to save files.</summary>
  struct BWGame
  {
    std::array<s32, PLAYER_COUNT> minerals;
    std::array<s32, PLAYER_COUNT> gas;
    std::array<s32, PLAYER_COUNT> cumulativeGas;
    std::array<s32, PLAYER_COUNT> cumulativeMinerals;

    u32 startingPlayerLocalId;
    std::array<u8, PLAYER_COUNT> playerSlotTypes;
    std::array<u8, PLAYER_COUNT> playerSlotRaces;
    std::array<u8, 4> teamGameMainPlayer;

    TilePosition screenTilePosition; // Used in conjunction with a screen refresh to move the viewport
    TilePosition mapTileSize; // Size of the map in tiles
    u16 messageSendToFilter;
    u16 messagePlayerFilter;
    u16 mapTileset;
    u16 currentMusic;
    u8 __unk_f0;
    u8 activePlayerCount;
    u8 consoleIndex;
    u8 isCustomSinglePlayer;
    u32 __unk_f4;
    u32 __unk_f8;

    std::array<u32, PLAYER_COUNT> playerVision;
    std::array<u32, PLAYABLE_PLAYER_COUNT> playerRandomizerMap; // player id to original id

    u32 frameCount; // frames
    u32 savedElapsedSeconds;
    u16 campaignIndex;
    char nextScenario[32];

    u8 singlePlayerRace;
    std::array<u8, PLAYABLE_PLAYER_COUNT> singlePlayerComputerRaces;
    u8 __unk_17f;

    std::array<Position, 3> savedScreenPositions;
    std::array< std::array<u8, BW::UNIT_TYPE_COUNT>, BW::PLAYER_COUNT> unitAvailability;
    std::array<u8, BW::PLAYABLE_PLAYER_COUNT> __unk_c3c;
    point lastEventPosition;

    char mapFileName[260];
    char mapTitle[32];

    struct Selection
    {
      struct UnitGroup
      {
        std::array<s32, MAX_SELECTION_COUNT> unitIds;
      };
      std::array<UnitGroup, 18> hotkeys;
    };

    std::array<Selection, PLAYABLE_PLAYER_COUNT> savedPlayerSelections;
    std::array<u8, 1024> __unk_2870;

    u8 defaultMessageFilter;
    u8 playerLoseType;
    std::array<bool, PLAYABLE_PLAYER_COUNT> playerLeft;

    std::array<u8, PLAYER_COUNT> selectionCircleColor;

    typedef struct {
      BYTE peRed;
      BYTE peGreen;
      BYTE peBlue;
      BYTE peFlags;
    } PaletteEntry;
    std::array<PaletteEntry, 24> tunitImagePalette;

    std::array<u8, PLAYER_COUNT> playerColorIndex;
    std::array<u8, 2> __unk_2cf2;
    
    std::array<s32, PLAYER_COUNT> allUnitsTotal;
    std::array<s32, PLAYER_COUNT> allUnitsProduced;
    std::array<s32, PLAYER_COUNT> allUnitsOwned;
    std::array<s32, PLAYER_COUNT> allUnitsLost;
    std::array<s32, PLAYER_COUNT> allUnitsKilled;
    std::array<s32, PLAYER_COUNT> allUnitScore;
    std::array<s32, PLAYER_COUNT> allKillScore;
    std::array<s32, PLAYER_COUNT> allBuildingsTotal;
    std::array<s32, PLAYER_COUNT> allBuildingsConstructed;
    std::array<s32, PLAYER_COUNT> allBuildingsOwned;
    std::array<s32, PLAYER_COUNT> allBuildingsLost;
    std::array<s32, PLAYER_COUNT> allBuildingsRazed;
    std::array<s32, PLAYER_COUNT> allBuildingScore;
    std::array<s32, PLAYER_COUNT> allRazingScore;
    std::array<s32, PLAYER_COUNT> allFactoriesConstructed;
    std::array<s32, PLAYER_COUNT> allFactoriesOwned;
    std::array<s32, PLAYER_COUNT> allFactoriesLost;
    std::array<s32, PLAYER_COUNT> allFactoriesRazed;

    // Supply available, used, and maximum for all players and every race
    struct SuppliesPerRace
    {
      std::array<s32, PLAYER_COUNT> available;
      std::array<s32, PLAYER_COUNT> used;
      std::array<s32, PLAYER_COUNT> max;
    };
    std::array<SuppliesPerRace, RACE_COUNT> supplies;
    std::array<s32, PLAYER_COUNT> customScore;

    // Unit counts: all, completed, killed, dead
    struct Counts
    {
      std::array<std::array<s32, PLAYER_COUNT>, UNIT_TYPE_COUNT> all;
      std::array<std::array<s32, PLAYER_COUNT>, UNIT_TYPE_COUNT> completed;
      std::array<std::array<s32, PLAYER_COUNT>, UNIT_TYPE_COUNT> killed;
      std::array<std::array<s32, PLAYER_COUNT>, UNIT_TYPE_COUNT> dead;
    };
    Counts unitCounts;

    std::array<std::array<u8, 24>, PLAYER_COUNT> techAvailableSC;
    std::array<std::array<u8, 24>, PLAYER_COUNT> techResearchedSC;
    std::array<u8, 36> techResearchInProgressSC;

    std::array<std::array<u8, 46>, PLAYER_COUNT> maxUpgradeLevelSC;
    std::array<std::array<u8, 46>, PLAYER_COUNT> currentUpgradeLevelSC;
    std::array<u8, 72> upgradeInProgressLvl1SC; // unused?
    std::array<u8, 72> upgradeInProgressLvl2SC; // unused?
    std::array<u8, 72> upgradeInProgressLvl3SC; // unused?

    std::array<u8, PLAYABLE_PLAYER_COUNT> playerForceId;
    std::array<u8, 4> forceFlags;
    std::array<std::array<char, 30>, 4> forceNames;

    std::array<std::array<u8, PLAYER_COUNT>, PLAYER_COUNT> playerAlliances;
    std::array<u32, PLAYER_COUNT> missionObjectiveStringIndices;

    u32 countdownTimer;   // seconds
    u32 elapsedTime;      // seconds

    u32 switchStatesSC;   // unused? 32 switches

    std::array<u8, PLAYABLE_PLAYER_COUNT> playerVictoryStatus;

    u32 leaderboardHasComputerPlayers;
    u8 leaderboardType;
    u8 leaderboardCondition;
    u16 leaderboardSubtype;
    u32 leaderboardGoal;
    u32 leaderboardStringIndex;

    std::array<u8, PLAYABLE_PLAYER_COUNT> remainingGamePauses;
    std::array<BW::Position, PLAYABLE_PLAYER_COUNT> startPositions;

    struct TriggerLocation
    {
      s32 left, top, right, bottom;
      u16 stringIndex;
      u16 flags;
    };

    std::array<TriggerLocation, 64> triggerLocationsSC;

    std::array<u32, 8> switchStatesBW;
    std::array<TriggerLocation, 255> triggerLocationsBW;

    u32 timerPauseState;

    std::array< std::array<u8, 20>, PLAYER_COUNT> techAvailableBW;
    std::array< std::array<u8, 20>, PLAYER_COUNT> techResearchedBW;
    std::array<u8, 72> techResearchInProgressBW;

    std::array< std::array<u8, 15>, PLAYER_COUNT> maxUpgradeLevelBW;
    std::array< std::array<u8, 15>, PLAYER_COUNT> currentUpgradeLevelBW;
    std::array<u8, 96> upgradeInProgressBW;

    bool isExpansion;

    u8 __unk_10351;

    std::array<u8, PLAYABLE_PLAYER_COUNT> unkPlayerColorSomething;

    std::array<u8, 29604> __unk_1035a; // literally unused?
  };

  static_assert(sizeof(BWGame) == 96000, "BWGame is expected to be 96000 bytes");

#define OFFSET_ASSERT(offset, member) static_assert(offset == offsetof(BWGame, member), "BWGame member not at correct offset")
  OFFSET_ASSERT(0x00, minerals);
  OFFSET_ASSERT(0x30, gas);
  OFFSET_ASSERT(0x60, cumulativeGas);
  OFFSET_ASSERT(0x90, cumulativeMinerals);
  OFFSET_ASSERT(0xc0, startingPlayerLocalId);
  OFFSET_ASSERT(0xc4, playerSlotTypes);
  OFFSET_ASSERT(0xd0, playerSlotRaces);
  OFFSET_ASSERT(0xdc, teamGameMainPlayer);
  OFFSET_ASSERT(0xe0, screenTilePosition);
  OFFSET_ASSERT(0xe4, mapTileSize);
  OFFSET_ASSERT(0xe8, messageSendToFilter);
  OFFSET_ASSERT(0xea, messagePlayerFilter);
  OFFSET_ASSERT(0xec, mapTileset);
  OFFSET_ASSERT(0xee, currentMusic);
  OFFSET_ASSERT(0xf0, __unk_f0);
  OFFSET_ASSERT(0xf1, activePlayerCount);
  OFFSET_ASSERT(0xf2, consoleIndex);
  OFFSET_ASSERT(0xf3, isCustomSinglePlayer);
  OFFSET_ASSERT(0xf4, __unk_f4);
  OFFSET_ASSERT(0xf8, __unk_f8);
  OFFSET_ASSERT(0xfc, playerVision);
  OFFSET_ASSERT(0x12c, playerRandomizerMap);
  OFFSET_ASSERT(0x14c, frameCount);
  OFFSET_ASSERT(0x150, savedElapsedSeconds);
  OFFSET_ASSERT(0x154, campaignIndex);
  OFFSET_ASSERT(0x156, nextScenario);
  OFFSET_ASSERT(0x176, singlePlayerRace);
  OFFSET_ASSERT(0x177, singlePlayerComputerRaces);
  OFFSET_ASSERT(0x17f, __unk_17f);
  OFFSET_ASSERT(0x180, savedScreenPositions);
  OFFSET_ASSERT(0x18c, unitAvailability);
  OFFSET_ASSERT(0xc3c, __unk_c3c);
  OFFSET_ASSERT(0xc44, lastEventPosition);
  OFFSET_ASSERT(0xc4c, mapFileName);
  OFFSET_ASSERT(0xd50, mapTitle);
  OFFSET_ASSERT(0xd70, savedPlayerSelections);
  OFFSET_ASSERT(0x2870, __unk_2870);
  OFFSET_ASSERT(0x2c70, defaultMessageFilter);
  OFFSET_ASSERT(0x2c71, playerLoseType);
  OFFSET_ASSERT(0x2c72, playerLeft);
  OFFSET_ASSERT(0x2c7a, selectionCircleColor);
  OFFSET_ASSERT(0x2c86, tunitImagePalette);
  OFFSET_ASSERT(0x2ce6, playerColorIndex);
  OFFSET_ASSERT(0x2cf2, __unk_2cf2);
  OFFSET_ASSERT(0x2cf4, allUnitsTotal);
  OFFSET_ASSERT(0x2d24, allUnitsProduced);
  OFFSET_ASSERT(0x2d54, allUnitsOwned);
  OFFSET_ASSERT(0x2d84, allUnitsLost);
  OFFSET_ASSERT(0x2db4, allUnitsKilled);
  OFFSET_ASSERT(0x2de4, allUnitScore);
  OFFSET_ASSERT(0x2e14, allKillScore);
  OFFSET_ASSERT(0x2e44, allBuildingsTotal);
  OFFSET_ASSERT(0x2e74, allBuildingsConstructed);
  OFFSET_ASSERT(0x2ea4, allBuildingsOwned);
  OFFSET_ASSERT(0x2ed4, allBuildingsLost);
  OFFSET_ASSERT(0x2f04, allBuildingsRazed);
  OFFSET_ASSERT(0x2f34, allBuildingScore);
  OFFSET_ASSERT(0x2f64, allRazingScore);
  OFFSET_ASSERT(0x2f94, allFactoriesConstructed);
  OFFSET_ASSERT(0x2fc4, allFactoriesOwned);
  OFFSET_ASSERT(0x2ff4, allFactoriesLost);
  OFFSET_ASSERT(0x3024, allFactoriesRazed);
  OFFSET_ASSERT(0x3054, supplies);
  OFFSET_ASSERT(0x3204, customScore);
  OFFSET_ASSERT(0x3234, unitCounts);
  OFFSET_ASSERT(0xdd34, techAvailableSC);
  OFFSET_ASSERT(0xde54, techResearchedSC);
  OFFSET_ASSERT(0xdf74, techResearchInProgressSC);
  OFFSET_ASSERT(0xdf98, maxUpgradeLevelSC);
  OFFSET_ASSERT(0xe1c0, currentUpgradeLevelSC);
  OFFSET_ASSERT(0xe3e8, upgradeInProgressLvl1SC);
  OFFSET_ASSERT(0xe430, upgradeInProgressLvl2SC);
  OFFSET_ASSERT(0xe478, upgradeInProgressLvl3SC);
  OFFSET_ASSERT(0xe4c0, playerForceId);
  OFFSET_ASSERT(0xe4c8, forceFlags);
  OFFSET_ASSERT(0xe4cc, forceNames);
  OFFSET_ASSERT(0xe544, playerAlliances);
  OFFSET_ASSERT(0xe5d4, missionObjectiveStringIndices);
  OFFSET_ASSERT(0xe604, countdownTimer);
  OFFSET_ASSERT(0xe608, elapsedTime);
  OFFSET_ASSERT(0xe60c, switchStatesSC);
  OFFSET_ASSERT(0xe610, playerVictoryStatus);
  OFFSET_ASSERT(0xe618, leaderboardHasComputerPlayers);
  OFFSET_ASSERT(0xe61c, leaderboardType);
  OFFSET_ASSERT(0xe61d, leaderboardCondition);
  OFFSET_ASSERT(0xe61e, leaderboardSubtype);
  OFFSET_ASSERT(0xe620, leaderboardGoal);
  OFFSET_ASSERT(0xe624, leaderboardStringIndex);
  OFFSET_ASSERT(0xe628, remainingGamePauses);
  OFFSET_ASSERT(0xe630, startPositions);
  OFFSET_ASSERT(0xe650, triggerLocationsSC);
  OFFSET_ASSERT(0xeb50, switchStatesBW);
  OFFSET_ASSERT(0xeb70, triggerLocationsBW);
  OFFSET_ASSERT(0xff5c, timerPauseState);
  OFFSET_ASSERT(0xff60, techAvailableBW);
  OFFSET_ASSERT(0x10050, techResearchedBW);
  OFFSET_ASSERT(0x10140, techResearchInProgressBW);
  OFFSET_ASSERT(0x10188, maxUpgradeLevelBW);
  OFFSET_ASSERT(0x1023c, currentUpgradeLevelBW);
  OFFSET_ASSERT(0x102f0, upgradeInProgressBW);
  OFFSET_ASSERT(0x10350, isExpansion);
  OFFSET_ASSERT(0x10351, __unk_10351);
  OFFSET_ASSERT(0x10352, unkPlayerColorSomething);
  OFFSET_ASSERT(0x1035a, __unk_1035a);

  
#undef OFFSET_ASSERT
}
