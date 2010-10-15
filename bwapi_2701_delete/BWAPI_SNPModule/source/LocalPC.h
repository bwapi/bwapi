#include <windows.h>
#include <stdio.h>

struct unka9;
struct version;
struct bncallbacks;
struct module;

typedef void* PSTRUCT;

bool __stdcall fxn0(int a1, int a2, int a3);
bool __stdcall _spiDestroy();
bool __stdcall _spiFree(int a1, int a2, int a3);
bool __stdcall _spiError(int a1, int a2, int a3);
bool __stdcall fxn4(int a1, int a2, int a3, int a4);
bool __stdcall fxn5(int a1, int a2, int a3, int a4);
bool __stdcall _spiInitialize(version *a1, PSTRUCT a2, bncallbacks *a3, module *a4, DWORD a5);
bool __stdcall fxn7(int a1, PSTRUCT a2, PSTRUCT a3, DWORD *a4, module *a5);
bool __stdcall fxn8(DWORD *a1);
bool __stdcall _spiLockGameList(int a1, int a2, DWORD *a3);
bool __stdcall fxn10(int *a1, int *a2, int *a3);
bool __stdcall _spiReceive(DWORD *a1, DWORD *a2, DWORD *a3);
bool __stdcall fxn12(int a1, int a2, int a3, int a4, int a5, int a6);
bool __stdcall _spiSend(DWORD addrCount, sockaddr **addrList, void *buf, DWORD bufLen);
bool __stdcall fxn14(int a1, int a2, int a3, int a4, int a5);
bool __stdcall _spiStartAdvertisingLadderGame(char *pszGameName, char *pszGamePassword, char *pszGameStats, DWORD dwGameState, DWORD dwElapsedTime, DWORD dwGameType, int a7, int a8, unka9 *a9, int a10);
bool __stdcall _spiStopAdvertisingGame();
bool __stdcall InitializeUser();
bool __stdcall _spiUnlockGameList(int a1, DWORD *a2);
/*bool __stdcall _spiStartAdvertisingGame(char *pszName, DWORD dwNameSize, char *pszStats, DWORD dwStatSize);
bool __stdcall fxn20(int a1, int a2, int a3, int a4, int a5, int a6);
bool __stdcall fxn21(int a1, int a2, int a3, int a4);
bool __stdcall fxn22(char *pszMessage, DWORD dwSize);
bool __stdcall fxn23(int a1, int a2, int a3);
bool __stdcall fxn24(int a1);
bool __stdcall LeagueLogout(char *pszName);
bool __stdcall GetLeagueName(char *pszDest, DWORD dwSize);
*/