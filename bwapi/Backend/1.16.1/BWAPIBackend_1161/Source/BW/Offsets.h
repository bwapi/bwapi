#pragma once
#include "../Types.h"
#include <storm.h>
#include <array>
#include <type_traits>

#include "Constants.h"
#include "Structures.h"
#include "Replay.h"

#include "TriggerEngine.h"
#include "Position.h"

#include "Dialog.h"
#include "BWGame.h"

#pragma pack(1)

/**
 * Broodwar content access tools. The namespace contains:
 * -# Data structers corresponding to bw data structures
 * -# Offests to bw data/functions
 * -# Functions that communicate directly with broodwar.
 */

namespace BW
{
  class  CUnit;
  class  CSprite;
  class  CImage;
  class  CBullet;
  class  TileType;
  class  dialog;
  class  Bitmap;
  struct dlgEvent;
  class  Font;
  struct SAI_Paths;
  struct grpHead;

#define IS_REF(name,addr) (& name) = *reinterpret_cast<std::remove_reference<decltype(name)>::type*>(addr);

  namespace BWDATA
  {
    namespace
    {
      BWGame IS_REF(Game, 0x0057F0F0); // up to 0x005967EE

      // Player Information
      std::array<u8, PLAYABLE_PLAYER_COUNT> IS_REF(PlayerVictory, 0x00650974);

      std::array<PlayerInfo, PLAYER_COUNT> IS_REF(Players, 0x0057EEE0);
      std::array<u32, PLAYABLE_PLAYER_COUNT> IS_REF(PlayerDownloadStatus, 0x0068F4FC);
      std::array<u32, PLAYABLE_PLAYER_COUNT> IS_REF(playerStatusArray, 0x0057F0B8);

      // Alliance and vision
      u32 IS_REF(ReplayVision, 0x006D0F18);
      u32 IS_REF(ReplayRevealAll, 0x006D0F1C);

      // Code Patches
      const u32 ServerMenuIn = 0x004DD5A2;
      const u32 ServerMenuOut = 0x004DD5C9;
      const u32 OpponentStartHack = 0x004B995D;
      const u32 SingleSpeedHack = 0x004D99FB;

      std::array<swishTimer, 43> IS_REF(commonSwishControllers, 0x005129EC);
      std::array<swishTimer, 5> IS_REF(gluCustmSwishController, 0x0051A9F0);
      std::array<swishTimer, 2> IS_REF(gluCmpgnSwishController, 0x00512B10);
      std::array<swishTimer, 1> IS_REF(gluScoreSwishController, 0x0051A844);
      std::array<swishTimer, 5> IS_REF(gluChatSwishController, 0x0051A490);

      // Command queue data
      u32 IS_REF(sgdwBytesInCmdQueue, 0x00654AA0);
      std::array<u8, TURN_BUFFER_SIZE> IS_REF(TurnBuffer, 0x00654880);

      static void(*const BWFXN_sendTurn)() = (void(*)()) 0x00485A40;
      const u32 BWFXN_QueueCommand = 0x00485BD0;

      // Speed and Latency
      struct GameSpeeds
      {
        std::array<u32, NUM_SPEEDS> gameSpeedModifiers;
        std::array<u32, NUM_SPEEDS> altSpeedModifiers;
      };
      GameSpeeds IS_REF(GameSpeedModifiers, 0x005124D8);   // mac: 0x0014BF3C
      u32 IS_REF(GameSpeed, 0x006CDFD4);  // mac: 0x00228458

      std::array<u32, NUM_SPEEDS> IS_REF(LatencyFrames, 0x0051CE70);
      s32 IS_REF(FrameSkip, 0x005124D4);
      u8 IS_REF(Latency, 0x006556e4);   // A value between 0 and 2 indicating latency setting

      // Dialog and drawing
      dialog* IS_REF(DialogList, 0x006D5E34);

      std::array<Font*, 4> IS_REF(FontBase, 0x006CE0F4);    // Can be worked around
      Bitmap IS_REF(GameScreenBuffer, 0x006CEFF0);

      std::array<layer, 8> IS_REF(ScreenLayers, 0x006CEF50);

      // Unit and CList offsets
      CUnit* IS_REF(UnitNodeList_VisibleUnit_First, 0x00628430);
      CUnit* IS_REF(UnitNodeList_HiddenUnit_First, 0x006283EC);
      CUnit* IS_REF(UnitNodeList_ScannerSweep_First, 0x006283F4);

      std::array<CUnit, UNIT_ARRAY_MAX_LENGTH> IS_REF(UnitNodeTable, 0x0059CCA8);

      CBullet* IS_REF(BulletNodeTable_FirstElement, 0x0064DEC4);
      std::array<CBullet, BULLET_ARRAY_MAX_LENGTH> IS_REF(BulletNodeTable, 0x0064B2E8);

      // Unit ordering system (fast units in region calculations)
      std::array<unitFinder, UNIT_ARRAY_MAX_LENGTH * 2> IS_REF(UnitOrderingX, 0x0066FF78);
      std::array<unitFinder, UNIT_ARRAY_MAX_LENGTH * 2> IS_REF(UnitOrderingY, 0x006769B8);
      int IS_REF(UnitOrderingCount, 0x0066FF74);

      // Mode stuff
      u8 IS_REF(gameType, 0x00596820);
      u32 IS_REF(InReplay, 0x006D0F14);
      int IS_REF(NetMode, 0x0059688C);

      ReplayHeader IS_REF(ReplayHead, 0x006D0F30);

      u8 IS_REF(GameState, 0x006D11EC);
      u16 IS_REF(gwNextGameMode, 0x0051CE90);
      u16 IS_REF(gwGameMode, 0x00596904);
      u32 IS_REF(glGluesMode, 0x006D11BC);

      int IS_REF(g_LocalNationID, 0x00512684);
      int IS_REF(g_LocalHumanID, 0x00512688);

      std::array<int, PLAYABLE_PLAYER_COUNT> IS_REF(playerToNationIdMap, 0x0057EEC0);

      // For fixing BW-level memory leak
      std::array<void*,2> IS_REF(customList_UIDlgData, 0x0051A350);

      // Selected units
      std::array<CUnit*, MAX_SELECTION_COUNT> IS_REF(ClientSelectionGroup, 0x00597208);
      u8 IS_REF(ClientSelectionCount, 0x0059723D);

      //
      u32 IS_REF(isGamePaused, 0x006509C4);

      // Positions (mouse/screen)
      u32 IS_REF(MoveToX, 0x0062848C);
      u32 IS_REF(MoveToY, 0x006284A8);

      POINT IS_REF(Mouse, 0x006CDDC4);
      u32 IS_REF(ScreenX, 0x00628448);
      u32 IS_REF(ScreenY, 0x00628470);

      // Strings
      std::array<char, MAX_PATH> IS_REF(CurrentMapFolder, 0x0059BB70);
      std::array<char, 28> IS_REF(SaveGameFile, 0x00685148);
    }
  }



  //----------------------------------------- VIDEO & DRAWING ------------------------------------------------
  extern void (__stdcall *pOldDrawGameProc)(BW::Bitmap *pSurface, BW::bounds *pBounds);
  extern void (__stdcall *pOldDrawDialogProc)(BW::Bitmap *pSurface, BW::bounds *pBounds);

  //----------------------------------------- FUNCTION LEVEL -------------------------------------------------
  static const u32 BWFXN_P_IsGamePaused           = 0x004D974E;

  // These 2 can be merged;
  // void __usercall refundBuildingCost(unsigned __int16 unitType<ax>, unsigned __int8 playerID<cl>)
  // Function takes cost of unit type, multiplies it by 3/4, and adds it to the player's resources.
  static const u32 BWFXN_RefundMinerals                 = 0x0042ce97;//building cancel
  static const u32 BWFXN_RefundMineralsReturnAddress    = 0x0042ce9e;//building cancel
  static const u32 BWFXN_RefundGas                      = 0x0042ceb8;//building cancel
  static const u32 BWFXN_RefundGasReturnAddress         = 0x0042cebe;//building cancel

  static const u32 BWFXN_RefundMinAndGas2               = 0x0042ced9;//morphing
  static const u32 BWFXN_RefundMinAndGas2ReturnAddress  = 0x0042cedf;//morphing

  static const u32 BWFXN_RefundMinAndGas5               = 0x00453E62;//research cancel
  static const u32 BWFXN_RefundMinAndGas5ReturnAddress  = 0x00453E68;//research cancel

  // These 2 can be merged;
  // void __userpurge refundUpgrade(unsigned __int8 upgradeType<al>, unsigned __int8 playerID)
  // Function calculates the cost of the upgrade by taking the upgrade level, multiplying 
  // with cost factor, and adding base cost, then adding that to player's resources.
  static const u32 BWFXN_RefundMin4                     = 0x004541C2;//upgrade cancel
  static const u32 BWFXN_RefundMin4ReturnAddress        = 0x004541CD;//upgrade cancel
  static const u32 BWFXN_RefundGas4                     = 0x004541FD;//upgrade cancel
  static const u32 BWFXN_RefundGas4ReturnAddress        = 0x00454204;//upgrade cancel

  // A little trickier, but can also be merged
  static const u32 BWFXN_RefundMin3                     = 0x0045D44D;//morphing
  static const u32 BWFXN_RefundMin3ReturnAddress        = 0x0045D454;//morphing
  static const u32 BWFXN_RefundGas3                     = 0x0045D46e;//morphing
  static const u32 BWFXN_RefundGas3ReturnAddress        = 0x0045D474;//morphing

  static const u32 BWFXN_SpendRepair                    = 0x00467006;
  static const u32 BWFXN_SpendRepairReturnAddress       = 0x0046700D;

  static void (* const BWFXN_RandomizePlayerRaces)() = (void (*)()) 0x004A9A30;
  static const u32 BWFXN_RandomizeRacePatch = 0x004EF20E;

  static void (* const BWFXN_InitializePlayerConsole)() = (void (*)()) 0x004EE180;
  static const u32 BWFXN_InitPlayerConsolePatch = 0x004EF244;

  //------------------------------------ POSITIONS (MOUSE/SCREEN) --------------------------------------------
  static void (__cdecl * const BWFXN_UpdateScreenPosition)()    = (void(__cdecl*)()) 0x0049BFD0;

  //-------------------------------------------- TRIGGERS -----------------------------------------------------

  namespace BWDATA
  {
    namespace
    {
      std::array<VCList<Triggers::Trigger>, PLAYABLE_PLAYER_COUNT> IS_REF(TriggerVectors, 0x0051A280);

      // Create/Kill/Remove Unit
      bool IS_REF(TrigRemoveInsteadOfKill, 0x005971DC); // false = kill, true = remove
      std::array<TrigLocation, 255> IS_REF(TrigLocations, 0x0058DC60);
      std::array<TrigUnitPropertySlot, 64> IS_REF(TrigUnitProperties, 0x00596CD8);
    }
  }

  static void (__stdcall * const BWFXN_ExecuteGameTriggers)(DWORD dwMillisecondsPerFrame) = (void(__stdcall*)(DWORD))0x0048A460;
  static const u32 BWFXN_ExecuteGameTrigsCallPatch = 0x004D9798;

  // Create/Kill/Remove Unit
  static bool(__fastcall * const BWFXN_KillRemoveUnit)(CUnit *pUnit, TrigKillRemoveProperties *properties) = (bool(__fastcall*)(CUnit*,TrigKillRemoveProperties*))0x004C7E20;
  static void(__fastcall * const BWFXN_CreateUnitWithProperties)(int playerGroup, u16 unitType, TrigCreateUnitProperties *properties) = (void(__fastcall*)(int group,u16 unitType,TrigCreateUnitProperties*))0x004C8C20;

  namespace BWDATA
  {
    namespace
    {
      //------------------------------------------------ MAPPING -------------------------------------------------
      TileID* IS_REF(MapTileArray, 0x005993C4); // MTXM (Matrix Map) -- gfpMIMap
      TileType* IS_REF(TileSetMap, 0x006D5EC8);  // cv5
      MiniTileMaps_type* IS_REF(MiniTileFlags, 0x005993D0);   // vf4
      activeTile* IS_REF(ActiveTileArray, 0x006D1260);
      SAI_Paths* IS_REF(SAIPathing, 0x006D5BFC);
    }
  }

#undef IS_REF
};

#pragma pack()
