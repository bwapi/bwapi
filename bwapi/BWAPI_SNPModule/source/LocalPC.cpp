#include "LocalPC.h"

char buffer[1024];

BOOL __stdcall fxn1(int a1, int a2, int a3)
{
  MessageBoxA(NULL, "Fxn1", "Fxn1", MB_OK);
  return TRUE;
}

BOOL __stdcall Cleanup()
{
  MessageBoxA(NULL, "Cleanup", "Cleanup", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn3(int a1, int a2, int a3)
{
  MessageBoxA(NULL, "Fxn3", "Fxn3", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn4(int a1, int a2, int a3)
{
  MessageBoxA(NULL, "Fxn4", "Fxn4", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn5(int a1, int a2, int a3, int a4)
{
  MessageBoxA(NULL, "Fxn5", "Fxn5", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn6(int a1, int a2, int a3, int a4)
{
  MessageBoxA(NULL, "Fxn6", "Fxn6", MB_OK);
  return TRUE;
}

struct version
{
  DWORD dwSize;   // 0x3C
  char  *pszGame;
  char  *pszVersion;
  DWORD dwGameId;
  DWORD dwVerByte;
  DWORD dwUnk14;
  DWORD dwMaxPlayers;
  DWORD dwUnk1C;
  DWORD dwUnk20;
  DWORD dwUnk24;
  DWORD dwUnk28;
  WORD  wUnk2C;
  WORD  wUnk2E;
  WORD  wUnk30;
  WORD  wUnk32;
  DWORD dwUnk34;
  LCID  lLangId;
};

struct bncallbacks
{
  DWORD dwSize;     // 0x5C
  DWORD dwUnk4;
  DWORD dwUnk8;
  void  *pUnkCallbackC;
  void  *pUnkCallback10;
  void  *pUnkCallback14;
  void  *pUnkCallback18;
  DWORD  dwUnk1C;
  void  *pUnkCallback20;
  void  *pUnkCallback24;
  DWORD  dwUnk28;
  void  *pUnkCallback2C;
  DWORD  dwUnk30;
  void  *pUnkCallback34;
  DWORD  dwUnk38;
  void  *pUnkCallback3C;
  char  *pszUnkStr40;
  void  *pUnkCallback44;
  DWORD dwUnk48;
  DWORD dwUnk4C;
  void  *pUnkCallback50;
  void  *pUnkCallback54;
  void  *pUnkCallback58;
};

struct module
{
  DWORD dwSize;   // 0x14
  char  *pszVersion;
  char  *pszModulePath;
  char  *pszMPQList;
  char  *pszPatchMpq;
};

BOOL __stdcall fxn7(version *a1, PSTRUCT a2, bncallbacks *a3, module *a4, DWORD a5)
{
  sprintf(buffer, "fxn7(%p, %p, %p, %p, %X)", a1, a2, a3, a4, a5);
  MessageBoxA(NULL, buffer, "Fxn7", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn8(int a1, PSTRUCT a2, PSTRUCT a3, DWORD *a4, module *a5)
{
  sprintf(buffer, "fxn8(0x%p, %p, %p, %p, %p)", a1, a2, a3, a4, a5);
  MessageBoxA(NULL, buffer, "Fxn8", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn9(DWORD *a1)
{
  sprintf(buffer, "fxn9(0x%p)", *a1);
  MessageBoxA(NULL, buffer, "Fxn9", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn10(int a1, int a2, int a3)
{
  //MessageBoxA(NULL, "Fxn10", "Fxn10", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn11(int a1, int a2, int a3)
{
  //MessageBoxA(NULL, "Fxn11", "Fxn11", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn12(int a1, int a2, int a3)
{
  //MessageBoxA(NULL, "Fxn12", "Fxn12", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn13(int a1, int a2, int a3, int a4, int a5, int a6)
{
  MessageBoxA(NULL, "Fxn13", "Fxn13", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn14(int a1, int a2, int a3, int a4)
{
  MessageBoxA(NULL, "Fxn14", "Fxn14", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn15(int a1, int a2, int a3, int a4, int a5)
{
  MessageBoxA(NULL, "Fxn15", "Fxn15", MB_OK);
  return TRUE;
}

/*
  dwGameState
  {
    full    = 0x02
    public  = 0x04
    started = 0x08
  }

*/

struct unka9
{
  DWORD dwMaxStormPlayers;
  DWORD dwHumanPlayers;
  DWORD dwSlots;
};

BOOL __stdcall CreateGame(char *pszGameName, char *pszGamePassword, char *pszGameStats, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, unka9 *a9, int a10)
{
  sprintf(buffer, "CreateGame(\"%s\", \"%s\", "
                "\n\"%s\",\n 0x%X, %d, [type: %d; subtype: %d], "
                "0x%p, 0x%p, \n"
                "[%d, %d, %d],\n"
                "0x%p)", 
                pszGameName, pszGamePassword, 
                pszGameStats, dwGameState, dwElapsedTime, (WORD)dwGameType, dwGameType >> 16, 
                a7, a8, 
                a9->dwMaxStormPlayers, a9->dwHumanPlayers, a9->dwSlots,
                a10);
  MessageBoxA(NULL, buffer, "CreateGame", MB_OK);
  return TRUE;
}

BOOL __stdcall LeaveGame()
{
  MessageBoxA(NULL, "LeaveGame", "LeaveGame", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn18()
{
  MessageBoxA(NULL, "Fxn18", "Fxn18", MB_OK);
  return TRUE;
}

BOOL __stdcall fxn19(int a1, int a2)
{
  //MessageBoxA(NULL, "Fxn19", "Fxn19", MB_OK);
  return TRUE;
}

char wtf[] = "HOOLABALOOZA";
BOOL __stdcall BeginTextMessage(char *pszMessage, DWORD dwMessageSize)
{
  sprintf(buffer, "BeginTextMessage(%s, %d)", pszMessage, dwMessageSize);
  pszMessage = wtf;
  //MessageBoxA(NULL, buffer, "BeginTextMessage", MB_OK);
  return FALSE;
}

