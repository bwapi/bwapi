#include "storm.h"

#define rBool { return TRUE; }
#define rPVoid { return NULL; }
#define rVoid { return; }
#define rInt { return 0; }

BOOL __stdcall SNetCreateGame(char *Source, char *a2, char *a3, int a4, int a5, int a6, char *a7, char *a8, int *playerid) rBool;
BOOL __stdcall SNetDestroy() rBool;
BOOL __stdcall SNetEnumProviders(int (__stdcall *callback)(DWORD, DWORD, DWORD, DWORD), int mincaps) rBool;

BOOL __stdcall SNetEnumGames(int (__stdcall *callback)(DWORD, DWORD, DWORD), int *hintnextcall) rBool;
BOOL __stdcall SNetDropPlayer(int playerid, DWORD flags) rBool;
BOOL __stdcall SNetGetGameInfo(int type, char *src, unsigned int length, int *byteswritten) rBool;

BOOL __stdcall SNetGetNumPlayers(int *firstplayerid, int *lastplayerid, int *activeplayers) rBool;

BOOL __stdcall SNetGetPlayerCaps(char playerid, int *caps) rBool;
BOOL __stdcall SNetGetPlayerName(int playerid, char *buffer, size_t buffersize) rBool;
BOOL __stdcall SNetGetProviderCaps(DWORD *caps) rBool;
BOOL __stdcall SNetGetTurnsInTransit(int *turns) rBool;
BOOL __stdcall SNetInitializeDevice(int a1, int a2, int a3, int a4, int *a5) rBool;
BOOL __stdcall SNetInitializeProvider(DWORD providerName, int *providerData, int *userData, int *a4, int *a5) rBool;
BOOL __stdcall SNetJoinGame(unsigned int a1, char *gameName, char *gamePassword, char *playerName, char *userStats, int *playerid) rBool;
BOOL __stdcall SNetLeaveGame(int type) rBool;
BOOL __stdcall SNetPerformUpgrade(DWORD *upgradestatus) rBool;
BOOL __stdcall SNetReceiveMessage(int *senderplayerid, char **data, int *databytes) rBool;
BOOL __stdcall SNetReceiveTurns(int a1, int arraysize, char **arraydata, unsigned int *arraydatabytes, DWORD *arrayplayerstatus) rBool;
HANDLE __stdcall SNetRegisterEventHandler(int type, int sevent) rPVoid;

int __stdcall SNetSelectGame(int a1, int a2, int a3, int a4, int a5, int *playerid) rInt;

BOOL __stdcall SNetSendMessage(unsigned int playerID, char *data, unsigned int databytes) rBool;
BOOL __stdcall SNetSendTurn(char *data, unsigned int databytes) rBool;

BOOL __stdcall SNetSetGameMode(DWORD modeFlags, char a2) rBool;

BOOL __stdcall SNetEnumGamesEx(int a1, int a2, int (__fastcall *callback)(DWORD, DWORD, DWORD), int *hintnextcall) rBool;
int __stdcall SNetSendServerChatCommand(const char *command) rInt;

BOOL __stdcall SNetGetPlayerNames(DWORD flags) rBool;
BOOL __stdcall SNetCreateLadderGame(char *gameName, char *gamePassword, char *gameDataString, DWORD gameType, int a5, int a6, char *GameTemplateData, int GameTemplateSize, int playerCount, char *playerName, char *a11, int *playerID) rBool;
BOOL __stdcall SNetReportGameResult(unsigned int a1, int size, int a3, int a4, int a5) rBool;

int __stdcall SNetSendLeagueCommand(char *cmd, char *callback) rInt;
int __stdcall SNetSendReplayPath(int a1, int a2, char *replayPath) rInt;
int __stdcall SNetGetLeagueName(int leagueID) rInt;
BOOL __stdcall SNet144(char *buffer) rBool;
int __stdcall SNetLeagueLogout(char *bnetName) rInt;
int __stdcall SNetGetLeaguePlayerName(int *curPlayerID, size_t nameSize) rInt;

HGDIOBJ __stdcall SDlgDefDialogProc(HWND hDlg, signed int DlgType, HDC textLabel, HWND hWnd) rPVoid;

HANDLE __stdcall SDlgDialogBoxIndirectParam(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam, LPARAM lParam) rPVoid;

BOOL __stdcall SDlgEndDialog(HWND hDlg, HANDLE nResult) rBool;

BOOL __stdcall SDlgSetControlBitmaps(HWND parentwindow, int *id, int a3, char *buffer2, char *buffer, int flags, int mask) rBool;

BOOL __stdcall SDlgBltToWindowE(HWND hWnd, HRGN a2, char *a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10) rBool;
BOOL __stdcall SDlgSetBitmapE(HWND hWnd, int a2, char *src, int mask1, int flags, int a6, int a7, int width, int a9, int mask2) rBool;

int __stdcall Ordinal224(int a1) rInt;

BOOL __stdcall SFileCloseArchive(HANDLE hArchive) rBool;
BOOL __stdcall SFileCloseFile(HANDLE hFile) rBool;

BOOL __stdcall SFileDdaBeginEx(HANDLE directsound, DWORD flags, DWORD mask, unsigned __int32 lDistanceToMove, signed __int32 volume, signed int a6, int a7) rBool;
BOOL __stdcall SFileDdaDestroy() rBool;
BOOL __stdcall SFileDdaEnd(HANDLE directsound) rBool;
BOOL __stdcall SFileDdaGetPos(HANDLE directsound, int a2, int a3) rBool;

BOOL __stdcall SFileDdaInitialize(HANDLE directsound) rBool;
BOOL __stdcall SFileDdaSetVolume(HANDLE directsound, signed int bigvolume, signed int volume) rBool;

BOOL __stdcall SFileGetFileArchive(HANDLE hFile, HANDLE archive) rBool;
LONG __stdcall SFileGetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh) rInt;
BOOL __stdcall SFileOpenArchive(const char *szMpqName, DWORD dwPriority, DWORD dwFlags, HANDLE *phMpq) rBool;
BOOL __stdcall SFileOpenFile(char *filename, HANDLE *phFile) rBool;
BOOL __stdcall SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile) rBool;
BOOL __stdcall SFileReadFile(HANDLE hFile, void *buffer, DWORD nNumberOfBytesToRead, DWORD *read, LONG lpDistanceToMoveHigh) rBool;

void __stdcall SFileSetLocale(LCID lcLocale) rVoid;

BOOL __stdcall SFileSetIoErrorMode(int mode, BOOL (__stdcall *callback)(char*,int,int) ) rBool;
BOOL __stdcall SFileGetArchiveName(HANDLE hArchive, char *name, int length) rBool;
BOOL __stdcall SFileGetFileName(HANDLE hFile, char *buffer, int length) rBool;

BOOL __stdcall SFileLoadFile(char *filename, void *buffer, int buffersize, int a4, int a5) rBool;
BOOL __stdcall SFileUnloadFile(HANDLE hFile) rBool;
BOOL __stdcall SFileLoadFileEx(void *hArchive, char *filename, int a3, int a4, int a5, DWORD searchScope, struct _OVERLAPPED *lpOverlapped) rBool;

BOOL __stdcall SBltROP3(int maxiterations, int lpSurface, int width, int height, int width2, int pitch, int a7, DWORD rop) rBool;
BOOL __stdcall SBltROP3Clipped(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10) rBool;

BOOL __stdcall SBmpDecodeImage(int type, signed int *srcbuffer, unsigned int a3, int a4, void *dstbuffer, int size, int a7, int a8, int a9) rBool;

BOOL __stdcall SBmpLoadImage(const char *fileName, int size, void *buffer, int buffersize, int width, int height, int depth) rBool;

HANDLE __stdcall SBmpAllocLoadImage(const char *fileName, int *palette, void **buffer, int *width, int *height, int unused6, int unused7, int (__stdcall *allocFunction)(DWORD)) rPVoid;

BOOL __stdcall SCodeCompile(char *directives1, char *directives2, char *loopstring, unsigned int maxiterations, unsigned int flags, HANDLE handle) rBool;
BOOL __stdcall SCodeDelete(HANDLE handle) rBool;

int __stdcall SCodeExecute(HANDLE handle, int a2) rInt;

BOOL __stdcall SDrawCaptureScreen(char *source) rBool;

HWND __stdcall SDrawGetFrameWindow(HWND sdraw_framewindow) rPVoid;

BOOL __stdcall SDrawLockSurface(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused) rBool;
BOOL __stdcall SDrawManualInitialize(HWND hWnd, IDirectDraw *ddInterface, IDirectDrawSurface *primarySurface, int a4, int a5, IDirectDrawSurface *backSurface, IDirectDrawPalette *palette, int a8) rBool;

BOOL __stdcall SDrawRealizePalette() rBool;

BOOL __stdcall SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect) rBool;
BOOL __stdcall SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4) rBool;

BOOL __stdcall SGdiDeleteObject(HANDLE handle) rBool;

BOOL __stdcall SGdiExtTextOut(int a1, int a2, int a3, int a4, unsigned int a8, signed int a6, signed int a7, const char *string, unsigned int arg20) rBool;
BOOL __stdcall SGdiImportFont(HGDIOBJ handle, int windowsfont) rBool;

BOOL __stdcall SGdiSelectObject(int handle) rBool;
BOOL __stdcall SGdiSetPitch(int pitch) rBool;

BOOL __stdcall Ordinal393(char *string, int, int) rBool;

void* __stdcall SMemAlloc(int amount, char *logfilename, int logline, int defaultValue) rPVoid;

BOOL __stdcall SMemFree(void *location, char *logfilename, int logline, char defaultValue) rBool;

BOOL __stdcall SRegLoadData(char *keyname, char *valuename, int size, LPBYTE lpData, BYTE flags, LPDWORD lpcbData) rBool;
BOOL __stdcall SRegLoadString(char *keyname, char *valuename, BYTE flags, char *buffer, size_t buffersize) rBool;
BOOL __stdcall SRegLoadValue(char *keyname, char *valuename, BYTE flags, int *value) rBool;
BOOL __stdcall SRegSaveData(char *keyname, char *valuename, int size, BYTE *lpData, DWORD cbData) rBool;
BOOL __stdcall SRegSaveString(char *keyname, char *valuename, BYTE flags, char *string) rBool;
BOOL __stdcall SRegSaveValue(char *keyname, char *valuename, BYTE flags, DWORD result) rBool;

BOOL __stdcall SRegDeleteValue(char *keyname, char *valuename, BYTE flags) rBool;

BOOL __stdcall STransBlt(int handle, int a2, int a3, int a4, int a5) rBool;
BOOL __stdcall STransBltUsingMask(int lpSurface, int a2, int pitch, int width, int handle) rBool;

BOOL __stdcall STransDelete(int handle) rBool;

BOOL __stdcall STransDuplicate(int handle, int source) rBool;
BOOL __stdcall STransIntersectDirtyArray(int handle, int dirtyarraymask, unsigned __int8 dirtyarray, int sourcemask) rBool;
BOOL __stdcall STransInvertMask(int handle, int sourcemask) rBool;

BOOL __stdcall STransSetDirtyArrayInfo(int width, int height, int depth, int bits) rBool;

BOOL __stdcall STrans442(int a1, int a2, int a3, char *a4, size_t a5) rBool;
BOOL __stdcall STransCombineMasks(int handle, int a2, int a3, int a4, int depth, int a6) rBool;

BOOL __stdcall STransCreateE(int a1, int a2, int a3, int bpp, int a5, int bufferSize, int a7) rBool;

BOOL __stdcall SVidDestroy() rBool;
BOOL __stdcall SVidGetSize(HANDLE video, int width, int height, int zero) rBool;
BOOL __stdcall SVidInitialize(HANDLE video) rBool;
BOOL __stdcall SVidPlayBegin(char *filename, int arg4, int a3, int a4, int a5, int a6, HANDLE video) rBool;

BOOL __stdcall SVidPlayContinueSingle(HANDLE video, int a2, int a3) rBool;
BOOL __stdcall SVidPlayEnd(HANDLE video) rBool;

BOOL __stdcall SErrGetErrorStr(DWORD dwErrCode, char *buffer, size_t bufferchars) rBool;
DWORD __stdcall SErrGetLastError() rInt;

void __stdcall SErrSetLastError(DWORD dwErrCode) rVoid;

void __stdcall SErrSuppressErrors(BOOL suppressErrors) rVoid;

void __stdcall SMemCopy(void *dest, const void *source, size_t size) rVoid;
int __stdcall SMemFill(void *location, size_t length, char fillWith) rInt;

int __stdcall SMemZero(void *location, DWORD length) rInt;
int __stdcall SMemCmp(void *location1, void *location2, DWORD size) rInt;

int __stdcall SStrCopy(char *dest, const char *src, DWORD max_length) rInt;
DWORD __stdcall SStrHash(const char *string, DWORD flags, DWORD Seed) rInt;
int __stdcall SStrNCat(char *dest, const char *src, DWORD max_length) rInt;

int __stdcall SStrLen(const char* string) rInt;

int __stdcall SStrCmp(const char *string1, const char *string2, size_t size) rInt;
int __stdcall SStrCmpI(const char *string1, const char *string2, size_t size) rInt;
char* __stdcall SStrUpper(char* string) rPVoid;

void __stdcall SRgn523(int handle, RECT *rect, int a3, int a4) rVoid;
void __stdcall SRgn524(int handle, int a2) rVoid;
void __stdcall SRgn525(int handle) rVoid;

void __stdcall SRgn529i(int handle, int a2, int a3) rVoid;

char* __stdcall SStrChr(const char *string, char c) rPVoid;
char* __stdcall SStrChrR(const char *string, char c) rPVoid;

size_t __cdecl SStrVPrintf(char *dest, size_t size, const char *format, ...) rInt;

int __stdcall SBigDel(void *buffer) rInt;

int __stdcall SBigFromBinary(void *buffer, const void *str, size_t size) rInt;

int __stdcall SBigNew(void **buffer) rInt;

int __stdcall SBigPowMod(void *buffer1, void *buffer2, int a3, int a4) rInt;

int __stdcall SBigToBinaryBuffer(void *buffer, int length, int a3, int a4) rInt;
