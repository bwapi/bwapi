#include "Hook.h"
#include "Offsets.h"
#include "Broodwar.h"

// quick fix of callbacks with a this circular dependency
// Hook and Engine are singleton, so this actually works fine
#include "BWAPI\Engine.h"

#include <Util\Types.h>
#include <Util/FileLogger.h>

// for installing the hooks
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace BW
{
  /* Helper function for displaying thread id. uncomment when thread info needed
  char threadIdBuffer[1000];
  void printThreadId(const char *text)
  {
    sprintf(threadIdBuffer, "%s thread id: %d\n", text, GetCurrentThreadId());
    OutputDebugStringA(threadIdBuffer);
  }
  */

  DWORD onCancelTrain_edx;
  DWORD onCancelTrain_ecx;
  struct SavedHookRegisters
  {
    DWORD eax;
    DWORD ebx;
    DWORD ecx;
    DWORD edx;
    DWORD esi;
    DWORD edi;
    DWORD esp;
    DWORD ebp;
  };

  //--------------------------------------------- ON COMMAND ORDER ---------------------------------------------
  void __declspec(naked) UnitDeathHook()
  {
    static DWORD removedUnit;
    __asm
    {
      mov removedUnit, esi
      call [BW::BWFXN_KillUnitTarget]
    }
  //  printThreadId("onUnitDeath");
    BWAPI::Engine::onUnitDeath((BW::Unit*) removedUnit);
    __asm jmp [BW::BWFXN_KillUnitBack]
  }

  //----------------------------------------------- ON MATCH END -----------------------------------------------
  void __declspec(naked) MatchEndHook()
  {
    BWAPI::Engine::onMatchEnd();
    __asm
    {
      call [BW::BWFXN_GameEndTarget]
      jmp [BW::BWFXN_GameEndBack]
    }
  }
  DWORD frameHookEax;
  //--------------------------------------------- NEXT FRAME HOOK ----------------------------------------------
  void __declspec(naked)  NextFrameHook()
  {
    __asm
    {
      call [BW::BWFXN_NextLogicFrameTarget]
      mov frameHookEax, eax
    }
  //  printThreadId("nextFrameHook");
    BWAPI::Engine::onMatchFrame();
    __asm
    {
      mov eax, frameHookEax
      jmp [BW::BWFXN_NextLogicFrameBack]
    }
  }

  //--------------------------------------------- MENU FRAME HOOK ----------------------------------------------
  DWORD menu_eaxSave, menu_ebxSave, menu_ecxSave, menu_edxSave, menu_esiSave, menu_ediSave, menu_espSave, menu_ebpSave;
  void __declspec(naked)  MenuFrameHook()
  {
    //not sure if all these registers need to be saved, but just to be safe.
    static SavedHookRegisters registers;
    __asm
    {
      call [BW::BWFXN_NextMenuFrameTarget]
      mov registers.eax, eax
      mov registers.ebx, ebx
      mov registers.ecx, ecx
      mov registers.edx, edx
      mov registers.esi, esi
      mov registers.edi, edi
      mov registers.esp, esp
      mov registers.ebp, ebp
    }
  //  printThreadId("menuFrameHook");
    BWAPI::Engine::onMenuFrame();
    __asm
    {
      mov eax, registers.eax
      mov ebx, registers.ebx
      mov ecx, registers.ecx
      mov edx, registers.edx
      mov esi, registers.esi
      mov edi, registers.edi
      mov esp, registers.esp
      mov ebp, registers.ebp
      jmp [BW::BWFXN_NextMenuFrameBack]
    }
  }

  //---------------------------------------------- SEND TEXT HOOKS ---------------------------------------------
  void __declspec(naked) SendTextHook()
  {
    static char* text;
    __asm
    {
      mov text, esi
    }
  //  printThreadId("onSendText");
    if (!BW::isSingleplayer() && text[0] != 0)
    {
      BWAPI::Engine::onMessageIntercepted(text);
    }
    text[0] = 0;
    __asm jmp [BW::BWFXN_SendPublicCallBack]
  }
  void __declspec(naked) SendSingleHook()
  {
    static char* text;
    __asm
    {
      mov text, edx
    }
  //  printThreadId("onSendSingle");
    if (BW::isSingleplayer() && text[0] != 0)
    {
      BWAPI::Engine::onMessageIntercepted(text);
    }

    __asm jmp BWFXN_SendTextCallBack
  }
  void __declspec(naked) SendLobbyHook()
  {
    static char* text;
    static SavedHookRegisters registers;
    __asm
    {
      mov registers.eax, eax
      mov registers.ebx, ebx
      mov registers.ecx, ecx
      mov registers.edx, edx
      mov registers.esi, esi
      mov registers.edi, edi
      mov registers.esp, esp
      mov registers.ebp, ebp
      mov text, edi
    }
    __asm
    {
      call [BW::BWFXN_SendLobbyCallTarget]
    }
    __asm
    {
      mov eax, registers.eax
      mov ebx, registers.ebx
      mov ecx, registers.ecx
      mov edx, registers.edx
      mov esi, registers.esi
      mov edi, registers.edi
      mov esp, registers.esp
      mov ebp, registers.ebp
      jmp [BW::BWFXN_SendLobbyCallBack]
    }
  }

  //---------------------------------------------- DRAW HOOKS --------------------------------------------------

  void __declspec(naked) RefreshHook()
  {
    // Fix name. When exactly does this hook get called?
    static SavedHookRegisters registers;
    __asm
    {
      mov registers.eax, eax
      mov registers.ebx, ebx
      mov registers.ecx, ecx
      mov registers.edx, edx
      mov registers.esi, esi
      mov registers.edi, edi
      mov registers.esp, esp
      mov registers.ebp, ebp
      push 640
      xor eax, eax
      mov edx, 480
      xor ecx, ecx
      call [BW::BWFXN_RefreshTarget]
      mov eax, registers.eax
      mov ebx, registers.ebx
      mov ecx, registers.ecx
      mov edx, registers.edx
      mov esi, registers.esi
      mov edi, registers.edi
      mov esp, registers.esp
      mov ebp, registers.ebp
      call [BW::BWFXN_RefreshTarget]
      jmp [BW::BWFXN_RefreshBack]
    }
  }

  static SavedHookRegisters drawRegisters;
  void __declspec(naked) MatchDrawHighHook()
  {
    static unsigned int shape_i;
    static SavedHookRegisters registers;
    __asm
    {
      mov drawRegisters.eax, eax
      mov drawRegisters.ebx, ebx
      mov drawRegisters.ecx, ecx
      mov drawRegisters.edx, edx
      mov drawRegisters.esi, esi
      mov drawRegisters.edi, edi
      mov drawRegisters.esp, esp
    }
  //  printThreadId("onDrawHigh");
    BWAPI::Engine::onMatchDrawHigh();
    __asm
    {
      mov eax, drawRegisters.eax
      mov ebx, drawRegisters.ebx
      mov ecx, drawRegisters.ecx
      mov edx, drawRegisters.edx
      mov esi, drawRegisters.esi
      mov edi, drawRegisters.edi
      mov esp, drawRegisters.esp
      call [BW::BWFXN_DrawHighTarget]
      jmp [BW::BWFXN_DrawHighBack]
    }
  }



  //-------------------------------------------- ISSUE NEW COMMAND ---------------------------------------------
  void __declspec(naked) IssueNewCommand()
  {
    //execute the part of the function that we overwrote:
    __asm
    {
      push ebp
      mov ebp, esp
      push ecx
      mov eax, dword ptr ds: [0x654AA0]
      jmp [BW::BWFXN_NewIssueCommand]
    }
  }
  //--------------------------------------------- ON ISSUE COMMAND ---------------------------------------------
  void __declspec(naked) IssueCommandHook()
  {
    static u32 commandIDptr;
    static u8 commandID;
    static SavedHookRegisters registers;
    __asm
    {
      mov registers.eax, eax
      mov registers.ebx, ebx
      mov registers.ecx, ecx
      mov registers.edx, edx
      mov registers.esi, esi
      mov registers.edi, edi
      mov registers.esp, esp
      mov registers.ebp, ebp
      mov commandIDptr, ecx;
    }
    commandID = *(u8*)commandIDptr;

    /* TODO: copy, redirect into Engine, process there
    //decide if we should let the command go through
    if ( BWAPI::Engine::isFlagEnabled(BWAPI::Flag::UserInput)
         || !BWAPI::Engine::isOnStartCalled()
         //If user input is disabled, only allow the following commands to go through:
         // TODO: make enum
         || commandID == 0x00 // Game Chat
         || commandID == 0x05 // Keep Alive
         || commandID == 0x06 // Save Game
         || commandID == 0x07 // Load Game
         || commandID == 0x08 // Restart Game
         || commandID == 0x09 // Select
         || commandID == 0x0A // Shift Select
         || commandID == 0x10 // Pause Game
         || commandID == 0x11 // Resume Game
         || commandID == 0x37 // Game Hash
         || commandID == 0x3C // Start Game
         || commandID == 0x3D // Map Download %
         || commandID == 0x3E // Game Slot Modification
         || commandID == 0x3F // Unknown
         || commandID == 0x40 // Join Game
         || commandID == 0x41 // Race Change
         || commandID == 0x42 // Melee Force Change
         || commandID == 0x43 // UMS   Force Change
         || commandID == 0x44 // Slot Change
         || commandID == 0x45 // Swap Players
         || commandID == 0x48 // Game Init (Random Seed)
         || commandID == 0x49 // Info Request
         || commandID == 0x4A // Force Data Transfer
         || commandID == 0x4B // Force Name Transfer
         || commandID == 0x4C // Lobby Chat
         || commandID == 0x4E // Boot Player
         || commandID == 0x4F // Map Transfer
         || commandID == 0x54 // Mission Briefing Start
         || commandID == 0x55 // Set Latency
         || commandID == 0x56 // Change Replay Speed
         || commandID == 0x57 // Leave Game
         || commandID == 0x58 // Minimap Ping
         || commandID == 0x5B // Make Game Public
         || commandID == 0x5C // Replay Game Chat
       )
     */
    if(true)
    {
      __asm
      {
        mov eax, registers.eax
        mov ebx, registers.ebx
        mov ecx, registers.ecx
        mov edx, registers.edx
        mov esi, registers.esi
        mov edi, registers.edi
        mov esp, registers.esp
        mov ebp, registers.ebp
      }
      IssueNewCommand();
      __asm retn
    }
    else
      __asm
      {
        mov eax, registers.eax
        mov ebx, registers.ebx
        mov ecx, registers.ecx
        mov edx, registers.edx
        mov esi, registers.esi
        mov edi, registers.edi
        mov esp, registers.esp
        mov ebp, registers.ebp
        retn
      }
  }

  //----------------------------------- DRAW FUNCTIONS -------------------------------------
  void drawBox(int _x, int _y, int _w, int _h, int color)
  {
    static int x;
    static int y;
    static int w;
    static int h;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    if (x + w <= 0 || y + h <= 0 || x >= 639 || y >= 479)
      return;
    if (x + w > 639) w = 639 - x;
    if (y + h > 479) h = 479 - y;
    if (x < 0) {w += x; x = 0;}
    if (y < 0) {h += y; y = 0;}

    *BW::BWDATA_DrawColor = (u8)color;

    __asm
    {
      mov eax, drawRegisters.eax
      mov ebx, drawRegisters.ebx
      mov ecx, drawRegisters.ecx
      mov edx, drawRegisters.edx
      push h
      push w
      push y
      push x
      call [BW::BWFXN_DrawBox]
    }
  }

  void drawText(int _x, int _y, const char* ptext)
  {
    if (_x<0 || _y<0 || _x>640 || _y>400) return;
    int temp = 0;
    /* @todo: this looks retarded, and probably not all of it is necessary */
    DWORD temp_ptr = (DWORD)&temp;
    *BW::BWDATA_PrintXY_Unknown1 = 0x21;
    *BW::BWDATA_PrintXY_Unknown2 = 0x00D8;
    *BW::BWDATA_PrintXY_Unknown3 = 0x0013;
    *BW::BWDATA_PrintXY_PositionX2 = 640;
    *BW::BWDATA_PrintXY_PositionX = _x;
    *BW::BWDATA_PrintXY_PositionY = _y;
    *BW::BWDATA_PrintXY_Current_Font = *BW::BWDATA_Font10_Handle;
    BW::BWDATA_PrintXY_Font->tFontData = BW::BWDATA_FontData;
    BW::BWDATA_PrintXY_Font->tFontUnknown = 0x00000001;
    BW::BWDATA_PrintXY_Font->x1 = 0x00D8;
    BW::BWDATA_PrintXY_Font->y1 = 0x0000;
    BW::BWDATA_PrintXY_Font->x2 = 0x0280;
    BW::BWDATA_PrintXY_Font->y2 = 0x0000;

    __asm
    {
      mov eax, ptext
      mov ebx, 0x00000000
      mov ecx, 0x0000000D
      mov esi, 0x000000e8
      mov edi, ptext
      push temp_ptr
      call [BW::BWFXN_PrintXY]
    }
  }
  //------------------------------------ ISSUE COMMAND -------------------------------------
  void issueCommandFromMemory(void *pbBuffer, int iSize)
  {
    __asm
    {
      mov ecx, pbBuffer
      mov edx, iSize
    }
    IssueNewCommand();
  }
  //------------------------------------------------ JMP PATCH -------------------------------------------------
  void JmpCallPatch(void* pDest, int pSrc, int nNops = 0)
  {
    DWORD OldProt = 0;
    VirtualProtect((LPVOID)pSrc, 5 + nNops, PAGE_EXECUTE_READWRITE, &OldProt);
    unsigned char jmp = 0xE9;
    memcpy((LPVOID)pSrc, &jmp, 1);
    DWORD address = (DWORD)pDest - (DWORD)pSrc - 5;
    memcpy((LPVOID)(pSrc + 1), &address, 4);
    for (int i = 0; i < nNops; ++i)
      *(BYTE*)((DWORD)pSrc + 5 + i) = 0x90;
    VirtualProtect((LPVOID)pSrc, 5 + nNops, OldProt, &OldProt);
  }
  //------------------------------------------------ WRITE NOPS ------------------------------------------------
  void WriteNops(void* pDest, int nNops)
  {
    DWORD OldProt = 0;
    VirtualProtect(pDest, nNops, PAGE_EXECUTE_READWRITE, &OldProt);
    memset(pDest, 0x90, nNops);
    VirtualProtect(pDest, nNops, OldProt, &OldProt);
  }
  //------------------------------------------------ WRITE MEM -------------------------------------------------
  void WriteMem(void* pDest, void* pSource, int nSize)
  {
    DWORD OldProt = 0;
    VirtualProtect(pDest, nSize, PAGE_EXECUTE_READWRITE, &OldProt);
    memcpy(pDest, pSource, nSize);
    VirtualProtect(pDest, nSize, OldProt, &OldProt);
  }
  //--------------------------------------------- CTRT THREAD MAIN ---------------------------------------------
  DWORD installHooks()
  {
    static char logPath[MAX_PATH];
    delete Util::Logger::globalLog;
    GetPrivateProfileStringA("paths", "log_path", "NULL", logPath, MAX_PATH, "bwapi-data\\bwapi.ini");
    if (_strcmpi(logPath, "NULL") == 0)
    {
      FILE* f = fopen("bwapi-error.txt", "a+");
      fprintf(f, "Could not find log_path under paths in \"bwapi-data\\bwapi.ini\".\n");
      fclose(f);
    }

    Util::Logger::globalLog = new Util::FileLogger(std::string(logPath) + "\\global", Util::LogLevel::MicroDetailed);
    Util::Logger::globalLog->log("BWAPI initialisation started");

    JmpCallPatch((void*)&NextFrameHook,     BW::BWFXN_NextLogicFrame,  0);
    JmpCallPatch((void*)&MenuFrameHook,     BW::BWFXN_NextMenuFrame,   0);
    JmpCallPatch((void*)&MatchEndHook,      BW::BWFXN_GameEnd,         0);
    JmpCallPatch((void*)&UnitDeathHook,     BW::BWFXN_KillUnit,        0);
    JmpCallPatch((void*)&SendTextHook,      BW::BWFXN_SendPublicCall,  0);
    JmpCallPatch((void*)&SendSingleHook,    BW::BWFXN_SendTextCall,    0);
    JmpCallPatch((void*)&SendLobbyHook,     BW::BWFXN_SendLobbyCall,   0);
    JmpCallPatch((void*)&MatchDrawHighHook, BW::BWFXN_DrawHigh,        0);
    JmpCallPatch((void*)&RefreshHook,       BW::BWFXN_Refresh,         0);
    JmpCallPatch((void*)&IssueCommandHook,  BW::BWFXN_OldIssueCommand, 4);

    static const char zero = 0;
    WriteNops((void*)BW::BWDATA_MenuLoadHack, 11); // menu load
    WriteMem( (void*)BW::BWDATA_MenuInHack,        (void*)&zero, 1); // menu in
    WriteMem( (void*)BW::BWDATA_MenuOutHack,       (void*)&zero, 1); // menu out
    WriteMem( (void*)BW::BWDATA_MultiplayerHack,   (void*)&zero, 1); // Battle.net Server Select
    WriteMem( (void*)BW::BWDATA_MultiplayerHack2,  (void*)&zero, 1); // Battle.net Server Select
    WriteMem( (void*)BW::BWDATA_OpponentStartHack, (void*)&zero, 1); // Start without an opponent
    return 0;
  }
}
