#include <windows.h>
#include <stdio.h>

struct unka9;
struct version;
struct bncallbacks;
struct module;

typedef void* PSTRUCT;

BOOL __stdcall fxn1(int a1, int a2, int a3);
BOOL __stdcall Cleanup();
BOOL __stdcall fxn3(int a1, int a2, int a3);
BOOL __stdcall fxn4(int a1, int a2, int a3);
BOOL __stdcall fxn5(int a1, int a2, int a3, int a4);
BOOL __stdcall fxn6(int a1, int a2, int a3, int a4);
BOOL __stdcall fxn7(version *a1, PSTRUCT a2, bncallbacks *a3, module *a4, DWORD a5);
BOOL __stdcall fxn8(int a1, PSTRUCT a2, PSTRUCT a3, DWORD *a4, module *a5);
BOOL __stdcall fxn9(DWORD *a1);
BOOL __stdcall fxn10(int a1, int a2, int a3);
BOOL __stdcall fxn11(int a1, int a2, int a3);
BOOL __stdcall fxn12(int a1, int a2, int a3);
BOOL __stdcall fxn13(int a1, int a2, int a3, int a4, int a5, int a6);
BOOL __stdcall fxn14(int a1, int a2, int a3, int a4);
BOOL __stdcall fxn15(int a1, int a2, int a3, int a4, int a5);
BOOL __stdcall CreateGame(char *pszGameName, char *pszGamePassword, char *pszGameStats, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, unka9 *a9, int a10);
BOOL __stdcall LeaveGame();
BOOL __stdcall fxn18();
BOOL __stdcall fxn19(int a1, int a2);
BOOL __stdcall BeginTextMessage(char *pszMessage, DWORD dwMessageSize);
