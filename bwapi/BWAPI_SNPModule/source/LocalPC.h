#include <windows.h>
#include <stdio.h>

struct unka9;
struct version;
struct bncallbacks;
struct module;

typedef void* PSTRUCT;

bool __stdcall fxn0(int a1, int a2, int a3);
bool __stdcall Cleanup();
bool __stdcall fxn2(int a1, int a2, int a3);
bool __stdcall fxn3(int a1, int a2, int a3);
bool __stdcall fxn4(int a1, int a2, int a3, int a4);
bool __stdcall fxn5(int a1, int a2, int a3, int a4);
bool __stdcall InitializeModule(version *a1, PSTRUCT a2, bncallbacks *a3, module *a4, DWORD a5);
bool __stdcall fxn7(int a1, PSTRUCT a2, PSTRUCT a3, DWORD *a4, module *a5);
bool __stdcall fxn8(DWORD *a1);
bool __stdcall fxn9(int a1, int a2, int a3);
bool __stdcall fxn10(int a1, int a2, int a3);
bool __stdcall fxn11(int a1, int a2, int a3);
bool __stdcall fxn12(int a1, int a2, int a3, int a4, int a5, int a6);
bool __stdcall fxn13(int a1, int a2, int a3, int a4);
bool __stdcall fxn14(int a1, int a2, int a3, int a4, int a5);
bool __stdcall CreateGame(char *pszGameName, char *pszGamePassword, char *pszGameStats, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, unka9 *a9, int a10);
bool __stdcall LeaveGame();
bool __stdcall InitializeUser();
bool __stdcall fxn18(int a1, int a2);
bool __stdcall fxn19(char *pszName, DWORD dwNameSize, char *pszStats, DWORD dwStatSize);
bool __stdcall fxn20(int a1, int a2, int a3, int a4, int a5, int a6);
bool __stdcall fxn21(int a1, int a2, int a3, int a4);
bool __stdcall fxn22(char *pszMessage, DWORD dwSize);
bool __stdcall fxn23(int a1, int a2, int a3);
bool __stdcall fxn24(int a1);
bool __stdcall LeagueLogout(char *pszName);
bool __stdcall GetLeagueName(char *pszDest, DWORD dwSize);
