#include "LocalPC.h"

char buffer[1024];

bool __stdcall fxn0(int a1, int a2, int a3)
{
  MessageBoxA(NULL, "Fxn0", "Fxn0", MB_OK);
  return true;
}

bool __stdcall _spiDestroy()
{
  return true;
}

bool __stdcall _spiFree(int a1, int a2, int a3)
{
  return true;
}

bool __stdcall _spiError(int a1, int a2, int a3)
{
  SetLastError(ERROR_INVALID_PARAMETER);
  return false;
}

bool __stdcall fxn4(int a1, int a2, int a3, int a4)
{
  MessageBoxA(NULL, "Fxn4", "Fxn4", MB_OK);
  return true;
}

bool __stdcall fxn5(int a1, int a2, int a3, int a4)
{
  MessageBoxA(NULL, "Fxn5", "Fxn5", MB_OK);
  return true;
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

// spiInitialize(0x%08x)
bool __stdcall _spiInitialize(version *a1, PSTRUCT a2, bncallbacks *a3, module *a4, DWORD a5)
{
  /*
  sprintf(buffer, "InitializeModule(%p, %p, %p, %p, %X)", a1, a2, a3, a4, a5);
  MessageBoxA(NULL, buffer, "InitializeModule", MB_OK);*/
  return true;
}

bool __stdcall fxn7(int a1, PSTRUCT a2, PSTRUCT a3, DWORD *a4, module *a5)
{
  return false;
}

bool __stdcall fxn8(DWORD *a1)
{
  *a1 = 0;
  return true;
}

// spiLockGameList(0x%08x,0x%08x,*gamelist)
bool __stdcall _spiLockGameList(int a1, int a2, DWORD *a3)
{
  //sprintf(buffer, "fxn9(%p, %p, %p)", a1, a2, *a3);
  //MessageBoxA(NULL, buffer, "Fxn9", MB_OK);
  return true;
}

bool __stdcall fxn10(int *a1, int *a2, int *a3)
{
  if ( a1 )
    *a1 = 0;
  if ( a2 )
    *a2 = 0;
  if ( a3 )
    *a3 = 0;
  // stuff
  return true;
}

// spiReceive(*addr,*data,*databytes) (int *, int *, char **)
bool __stdcall _spiReceive(DWORD *a1, DWORD *a2, DWORD  *a3)
{
  if ( a1 )
    *a1 = 0;
  if ( a2 )
    *a2 = 0;
  if ( a3 )
    *a3 = 0;
  SetLastError(0x8510006B);
  return true;
}

bool __stdcall fxn12(int a1, int a2, int a3, int a4, int a5, int a6)
{
  MessageBoxA(NULL, "Fxn12", "Fxn12", MB_OK);
  return true;
}

bool __stdcall _spiSend(DWORD addrCount, sockaddr **addrList, void *buf, DWORD bufLen)
{
  return true;
}

bool __stdcall fxn14(int a1, int a2, int a3, int a4, int a5)
{
  MessageBoxA(NULL, "Fxn14", "Fxn14", MB_OK);
  return false;
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

bool __stdcall _spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStats, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, unka9 *a9, int a10)
{
  /*sprintf(buffer, "CreateGame(\"%s\", \"%s\", "
                "\n\"%s\",\n 0x%X, %d, [type: %d; subtype: %d], "
                "0x%p, 0x%p, \n"
                "[%d, %d, %d],\n"
                "0x%p)", 
                pszGameName, pszGamePassword, 
                pszGameStats, dwGameState, dwElapsedTime, (WORD)dwGameType, dwGameType >> 16, 
                a7, a8, 
                a9->dwMaxStormPlayers, a9->dwHumanPlayers, a9->dwSlots,
                a10);
  MessageBoxA(NULL, buffer, "CreateGame", MB_OK);*/
  return true;
}

bool __stdcall _spiStopAdvertisingGame()
{
  return true;
}

bool __stdcall InitializeUser()
{
  return true;
}

// spiUnlockGameList(0x%08x,*hintnextcall)
bool __stdcall _spiUnlockGameList(int a1, DWORD *a2)
{
  //sprintf(buffer, "fxn18(%p, %p)", a1, *a2);
  //MessageBoxA(NULL, buffer, "Fxn18", MB_OK);
  if ( a2 )
  {
    *a2 = 500;
  }
  return true;
}
/*
bool __stdcall _spiStartAdvertisingGame(char *pszName, DWORD dwNameSize, char *pszStats, DWORD dwStatSize)
{
  MessageBoxA(NULL, "fxn19", "fxn19", MB_OK);
  return true;
}

bool __stdcall fxn20(int a1, int a2, int a3, int a4, int a5, int a6)
{
  MessageBoxA(NULL, "fxn20", "fxn20", MB_OK);
  return true;
}

bool __stdcall fxn21(int a1, int a2, int a3, int a4)
{
  MessageBoxA(NULL, "fxn21", "fxn21", MB_OK);
  return true;
}

bool __stdcall fxn22(char *pszMessage, DWORD dwSize)
{
  MessageBoxA(NULL, "fxn22", "fxn22", MB_OK);
  return true;
}

bool __stdcall fxn23(int a1, int a2, int a3)
{
  MessageBoxA(NULL, "fxn23", "fxn23", MB_OK);
  return true;
}

bool __stdcall fxn24(int a1)
{
  MessageBoxA(NULL, "fxn24", "fxn24", MB_OK);
  return true;
}

bool __stdcall LeagueLogout(char *pszName)
{
  return true;
}

bool __stdcall GetLeagueName(char *pszDest, DWORD dwSize)
{
  return true;
}

*/