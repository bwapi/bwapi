#include "storm.h"

#define rBool { return TRUE; }
#define rPVoid { return NULL; }
#define rVoid { return; }
#define rInt { return 0; }

BOOL STORMAPI SNetCreateGame(const char *pszGameName, const char *pszGamePassword, const char *pszGameStatString, DWORD dwGameType, char *GameTemplateData, int GameTemplateSize, int playerCount, char *creatorName, char *a11, int *playerID) rBool;
BOOL STORMAPI SNetDestroy() rBool;
BOOL STORMAPI SNetEnumProviders(int (STORMAPI *callback)(DWORD, DWORD, DWORD, DWORD), int mincaps) rBool;

BOOL STORMAPI SNetEnumGames(int (STORMAPI *callback)(DWORD, DWORD, DWORD), int *hintnextcall) rBool;
BOOL STORMAPI SNetDropPlayer(int playerid, DWORD flags) rBool;
BOOL STORMAPI SNetGetGameInfo(DWORD index, LPVOID buffer, DWORD buffersize, DWORD* byteswritten) rBool;

BOOL STORMAPI SNetGetNumPlayers(DWORD* firstplayerid, DWORD* lastplayerid, DWORD* activeplayers) rBool;

BOOL STORMAPI SNetGetPlayerCaps(DWORD playerid, SNETCAPSPTR caps) rBool;
BOOL STORMAPI SNetGetPlayerName(int playerid, char *buffer, size_t buffersize) rBool;
BOOL STORMAPI SNetGetProviderCaps(SNETCAPSPTR providerCaps) rBool;
BOOL STORMAPI SNetGetTurnsInTransit(DWORD *turns) rBool;
BOOL STORMAPI SNetInitializeDevice(DWORD deviceId, SNETPROGRAMDATAPTR programData, SNETPLAYERDATAPTR playerData, SNETUIDATAPTR interfaceData, SNETVERSIONDATAPTR versionData) rBool;
BOOL STORMAPI SNetInitializeProvider(DWORD providerId, SNETPROGRAMDATAPTR programData, SNETPLAYERDATAPTR playerData, SNETUIDATAPTR interfaceData, SNETVERSIONDATAPTR versionData) rBool;
BOOL STORMAPI SNetJoinGame(DWORD gameid, LPCSTR gameName, LPCSTR gamePassword, LPCSTR playerName, LPCSTR playerDescription, DWORD* playerid) rBool;
BOOL STORMAPI SNetLeaveGame(DWORD exitcode) rBool;
BOOL STORMAPI SNetPerformUpgrade(DWORD *upgradestatus) rBool;
BOOL STORMAPI SNetReceiveMessage(DWORD* senderplayerid, LPVOID* data, DWORD* databytes) rBool;
BOOL STORMAPI SNetReceiveTurns(DWORD firstplayerid, DWORD arraysize, LPVOID* arraydata, LPDWORD arraydatabytes, LPDWORD arrayplayerstatus) rBool;
BOOL STORMAPI SNetRegisterEventHandler(DWORD eventid, SNETEVENTPROC callback) rBool;

BOOL STORMAPI SNetSelectGame(DWORD flags, SNETPROGRAMDATAPTR programdata, SNETPLAYERDATAPTR playerdata, SNETUIDATAPTR interfacedata, SNETVERSIONDATAPTR versiondata, DWORD* playerid) rBool;

BOOL STORMAPI SNetSendMessage(DWORD targetplayerid, LPVOID data, DWORD databytes) rBool;
BOOL STORMAPI SNetSendTurn(LPVOID data, DWORD databytes) rBool;

BOOL STORMAPI SNetSetGameMode(DWORD modeFlags, bool makePublic) rBool;

BOOL STORMAPI SNetEnumGamesEx(DWORD categorybits, DWORD categorymask, int(__fastcall* callback)(DWORD, DWORD, DWORD), DWORD* hintnextcall) rBool;
BOOL STORMAPI SNetSendServerChatCommand(LPCSTR command) rBool;

BOOL STORMAPI SNetDisconnectAll(DWORD flags) rBool;
BOOL STORMAPI SNetCreateLadderGame(const char *pszGameName, const char *pszGamePassword, const char *pszGameStatString, DWORD dwGameType, DWORD dwGameLadderType, DWORD dwGameModeFlags, char *GameTemplateData, int GameTemplateSize, int playerCount, char *creatorName, char *a11, int *playerID) rBool;
BOOL STORMAPI SNetReportGameResult(unsigned a1, int size, int *results, const char* headerInfo, const char* detailInfo) rBool;

int STORMAPI SNetSendLeagueCommand(char *cmd, char *callback) rInt;
int STORMAPI SNetSendReplayPath(int a1, int a2, char *replayPath) rInt;
int STORMAPI SNetGetLeagueName(int leagueID) rInt;
BOOL STORMAPI SNetGetPlayerNames(char **names) rBool;
int STORMAPI SNetLeagueLogout(char *bnetName) rInt;
int STORMAPI SNetGetLeaguePlayerName(char *curPlayerLeageName, size_t nameSize) rInt;

HDC  STORMAPI SDlgBeginPaint(HWND window, LPPAINTSTRUCT ps) rPVoid;
BOOL STORMAPI SDlgBltToWindowE(HWND window, HRGN region, int x, int y, LPBYTE bitmapbits, LPRECT bitmaprect, LPSIZE bitmapsize, DWORD colorkey, DWORD pattern, DWORD rop3) rBool;
BOOL STORMAPI SDlgBltToWindowI(HWND window, HRGN region, int x, int y, LPBYTE bitmapbits, LPRECT bitmaprect, LPSIZE bitmapsize, DWORD colorkey, DWORD pattern, DWORD rop3) rBool;
BOOL STORMAPI SDlgCheckTimers() rBool;
HWND STORMAPI SDlgCreateDialogIndirectParam(HINSTANCE instance, LPCDLGTEMPLATE templatedata, HWND parentwindow, DLGPROC dialogproc, LPARAM initparam) rPVoid;
HWND STORMAPI SDlgCreateDialogParam(HINSTANCE instance, LPCTSTR templatename, HWND parentwindow, DLGPROC dialogproc, LPARAM initparam) rPVoid;
BOOL STORMAPI SDlgDefDialogProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) rBool;

int  STORMAPI SDlgDialogBoxIndirectParam(HINSTANCE instance, LPCDLGTEMPLATE templatedata, HWND parentwindow, DLGPROC dialogproc, LPARAM initparam) rInt;
int  STORMAPI SDlgDialogBoxParam(HINSTANCE instance, LPCTSTR templatename, HWND parentwindow, DLGPROC dialogproc, LPARAM initparam) rInt;
BOOL STORMAPI SDlgDrawBitmap(HWND window, DWORD usage, HRGN region, int offsetx, int offsety, LPRECT boundingoffset, DWORD flags) rBool;
BOOL STORMAPI SDlgEndDialog(HWND window, int result) rBool;
BOOL STORMAPI SDlgEndPaint(HWND window, LPPAINTSTRUCT ps) rBool;
BOOL STORMAPI SDlgKillTimer(HWND window, UINT event) rBool;
BOOL STORMAPI SDlgSetBaseFont(int pointsize, int weight, DWORD flags, DWORD family, LPCTSTR face) rBool;
BOOL STORMAPI SDlgSetBitmapE(HWND window, HWND parentwindow, LPCTSTR controltype, DWORD controlstyle, DWORD usage, LPBYTE bitmapbits, LPRECT rect, int width, int height, COLORREF colorkey) rBool;
BOOL STORMAPI SDlgSetBitmapI(HWND window, HWND parentwindow, LPCTSTR controltype, DWORD controlstyle, DWORD usage, LPBYTE bitmapbits, LPRECT rect, int width, int height, COLORREF colorkey) rBool;
BOOL STORMAPI SDlgSetControlBitmaps(HWND parentwindow, LPINT controllist, LPDWORD usagelist, LPBYTE bitmapbits, LPSIZE bitmapsize, DWORD adjusttype, COLORREF colorkey) rBool;
BOOL STORMAPI SDlgSetCursor(HWND window, HCURSOR cursor, DWORD id, HCURSOR* oldcursor) rBool;
BOOL STORMAPI SDlgSetSystemCursor(LPBYTE maskbitmap, LPBYTE imagebitmap, LPSIZE size, DWORD id) rBool;
BOOL STORMAPI SDlgSetTimer(HWND window, UINT event, UINT elapse, TIMERPROC timerfunc) rBool;
BOOL STORMAPI SDlgUpdateCursor() rBool;

int STORMAPI Ordinal224(int a1) rInt;

BOOL STORMAPI SFileAuthenticateArchive(HANDLE archive, DWORD* extendedresult) rBool;
BOOL STORMAPI SFileCloseArchive(HANDLE handle) rBool;
BOOL STORMAPI SFileCloseFile(HANDLE handle) rBool;
BOOL STORMAPI SFileDdaBegin(HANDLE handle, DWORD buffersize, DWORD flags) rBool;
BOOL STORMAPI SFileDdaBeginEx(HANDLE handle, DWORD buffersize, DWORD flags, DWORD offset, LONG volume, LONG pan, LPVOID reserved) rBool;
BOOL STORMAPI SFileDdaDestroy() rBool;
BOOL STORMAPI SFileDdaEnd(HANDLE directsound) rBool;
BOOL STORMAPI SFileDdaGetPos(HANDLE handle, DWORD* position, DWORD* maxposition) rBool;
BOOL STORMAPI SFileDdaGetVolume(HANDLE handle, DWORD* volume, DWORD* pan) rBool;

BOOL STORMAPI SFileDdaInitialize(HANDLE directsound) rBool;
BOOL STORMAPI SFileDdaSetVolume(HANDLE handle, LONG volume, LONG pan) rBool;
BOOL STORMAPI SFileDestroy() rBool;
BOOL STORMAPI SFileEnableDirectAccess(BOOL enable) rBool;
BOOL STORMAPI SFileGetArchiveInfo(HANDLE archive, int* priority, BOOL* cdrom) rBool;
BOOL STORMAPI SFileGetBasePath(LPTSTR buffer, DWORD bufferchars) rBool;
BOOL STORMAPI SFileGetFileArchive(HANDLE file, HANDLE archive) rBool;
LONG STORMAPI SFileGetFileSize(HANDLE handle, LPDWORD filesizehigh) rInt;
BOOL STORMAPI SFileOpenArchive(LPCTSTR archivename, int priority, BOOL cdonly, HANDLE* handle) rBool;

BOOL STORMAPI SFileOpenFile(LPCTSTR filename, HANDLE* handle) rBool;
BOOL STORMAPI SFileOpenFileEx(HANDLE archivehandle, LPCTSTR filename, DWORD flags, HANDLE* handle) rBool;
BOOL STORMAPI SFileReadFile(HANDLE handle, LPVOID buffer, DWORD bytestoread, LPDWORD bytesread, LPOVERLAPPED overlapped) rBool;
BOOL STORMAPI SFileSetBasePath(LPCTSTR path) rBool;
BOOL STORMAPI SFileSetFilePointer(HANDLE handle, LONG distancetomove, PLONG distancetomoveHigh, DWORD movemethod) rBool;

void STORMAPI SFileSetLocale(LCID lcid) rVoid;

BOOL STORMAPI SFileSetIoErrorMode(DWORD errormode, SFILEERRORPROC errorproc) rBool;

BOOL STORMAPI SFileGetArchiveName(HANDLE archive, LPTSTR buffer, DWORD bufferchars) rBool;
BOOL STORMAPI SFileGetFileName(HANDLE file, LPTSTR buffer, DWORD bufferchars) rBool;

BOOL STORMAPI SFileLoadFile(char *filename, void *buffer, int buffersize, int a4, int a5) rBool;
BOOL STORMAPI SFileUnloadFile(HANDLE hFile) rBool;
BOOL STORMAPI SFileLoadFileEx(void *hArchive, char *filename, int a3, int a4, int a5, DWORD searchScope, struct _OVERLAPPED *lpOverlapped) rBool;

BOOL STORMAPI SBltGetSCode(DWORD rop3, LPSTR buffer, DWORD buffersize, BOOL optimize) rBool;
BOOL STORMAPI SBltROP3(LPBYTE dest, LPBYTE source, int width, int height, int destcx, int sourcecx, DWORD pattern, DWORD rop3) rBool;
BOOL STORMAPI SBltROP3Clipped(LPBYTE dest, LPRECT destrect, LPSIZE destsize, int destpitch, LPBYTE source, LPRECT sourcerect, LPSIZE sourcesize, int sourcepitch, DWORD pattern, DWORD rop3) rBool;
BOOL STORMAPI SBltROP3Tiled(LPBYTE dest, LPRECT destrect, int destpitch, LPBYTE source, LPRECT sourcerect, int sourcepitch, int sourceoffsetx, int sourceoffsety, DWORD pattern, DWORD rop3) rBool;

BOOL STORMAPI SBmpDecodeImage(DWORD imagetype, LPBYTE imagedata, DWORD imagebytes, LPPALETTEENTRY paletteentries, LPBYTE bitmapbits, DWORD buffersize, int* width, int* height, int* bitdepth) rBool;
BOOL STORMAPI SBmpLoadImage(LPCTSTR filename, LPPALETTEENTRY paletteentries, LPBYTE bitmapbits, DWORD buffersize, int* width, int* height, int* bitdepth) rBool;

BOOL STORMAPI SBmpSaveImage(LPCTSTR filename, LPPALETTEENTRY paletteentries, LPBYTE bitmapbits, int width, int height, int bitdepth) rBool;
BOOL STORMAPI SBmpAllocLoadImage(LPCTSTR filename, LPPALETTEENTRY  paletteentries, LPBYTE* returnedbuffer, int* width, int* height, int* bitdepth, int requestedbitdepth, SBMPALLOCPROC allocproc) rBool;

BOOL STORMAPI SCodeCompile(LPCSTR prologstring, LPCSTR loopstring, LPCSTR* firsterror, DWORD maxiterations, DWORD flags, HANDLE* handle) rBool;
BOOL STORMAPI SCodeDelete(HANDLE handle) rBool;

BOOL STORMAPI SCodeExecute(HANDLE handle, SCODEEXECUTEDATAPTR executedata) rBool;
BOOL STORMAPI SCodeGetPseudocode(LPCSTR scodestring, LPSTR buffer, DWORD buffersize) rBool;

BOOL STORMAPI SDrawAutoInitialize(HINSTANCE instance, LPCSTR classname, LPCSTR title, WNDPROC wndproc, int servicelevel, int width, int height, int bitdepth) rBool;
BOOL STORMAPI SDrawCaptureScreen(LPCTSTR filename) rBool;

BOOL STORMAPI SDrawClearSurface(int surfacenumber) rBool;
BOOL STORMAPI SDrawDestroy() rBool;
BOOL STORMAPI SDrawFlipPage() rBool;

HWND STORMAPI SDrawGetFrameWindow(HWND *sdraw_framewindow) rPVoid;
BOOL STORMAPI SDrawGetObjects(LPDIRECTDRAW *ddInterface, LPDIRECTDRAWSURFACE *primarySurface, LPDIRECTDRAWSURFACE *surface2, LPDIRECTDRAWSURFACE *surface3, LPDIRECTDRAWSURFACE *backSurface, LPDIRECTDRAWPALETTE *ddPalette, HPALETTE *hPalette) rBool;
BOOL STORMAPI SDrawGetScreenSize(DWORD *pdwWidth, DWORD *pdwHeight, DWORD *pdwBpp) rBool;

BOOL STORMAPI SDrawLockSurface(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused) rBool;
BOOL STORMAPI SDrawManualInitialize(HWND hWnd, LPDIRECTDRAW ddInterface, LPDIRECTDRAWSURFACE primarySurface, LPDIRECTDRAWSURFACE surface2, LPDIRECTDRAWSURFACE surface3, LPDIRECTDRAWSURFACE backSurface, LPDIRECTDRAWPALETTE ddPalette, HPALETTE hPalette) rBool;

BOOL STORMAPI SDrawPostClose() rBool;
//BOOL STORMAPI SDrawRealizePalette() rBool;

BOOL STORMAPI SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect) rBool;
BOOL STORMAPI SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4) rBool;

BOOL STORMAPI SEvtDispatch(DWORD type, DWORD subtype, DWORD id, LPVOID data) rBool;
BOOL STORMAPI SEvtRegisterHandler(DWORD type, DWORD subtype, DWORD id, DWORD flags, SEVTHANDLER handler) rBool;
BOOL STORMAPI SEvtUnregisterHandler(DWORD type, DWORD subtype, DWORD id, DWORD flags, SEVTHANDLER handler) rBool;
BOOL STORMAPI SEvtUnregisterType(DWORD type, DWORD subtype) rBool;
BOOL STORMAPI SEvtPopState(DWORD type, DWORD subtype) rBool;
BOOL STORMAPI SEvtPushState(DWORD type, DWORD subtype) rBool;
BOOL STORMAPI SEvtBreakHandlerChain(LPVOID data) rBool;

BOOL STORMAPI SGdiBitBlt(LPBYTE videobuffer, int destx, int desty, LPBYTE sourcedata, LPRECT sourcerect, int sourcecx, int sourcecy, COLORREF color, DWORD rop) rBool;
BOOL STORMAPI SGdiCreateFont(LPBYTE bits, int width, int height, int bitdepth, int filecharwidth, int filecharheight, LPSIZE charsizetable, HANDLE* handle) rBool;
BOOL STORMAPI SGdiDeleteObject(HANDLE handle) rBool;

BOOL STORMAPI SGdiExtTextOut(LPBYTE videobuffer, int x, int y, LPRECT rect, COLORREF color, int textcoloruse, int bkgcoloruse, LPCTSTR string, int chars) rBool;
BOOL STORMAPI SGdiImportFont(HFONT windowsfont, HANDLE* handle) rBool;
BOOL STORMAPI SGdiLoadFont(LPCTSTR filename, int filecharwidth, int filecharheight, int basecharwidth, LPSIZE charsizetable, HANDLE* handle) rBool;
BOOL STORMAPI SGdiRectangle(LPBYTE videobuffer, int left, int top, int right, int bottom, COLORREF color) rBool;
BOOL STORMAPI SGdiSelectObject(HANDLE handle) rBool;
BOOL STORMAPI SGdiSetPitch(int pitch) rBool;
BOOL STORMAPI SGdiTextOut(LPBYTE videobuffer, int x, int y, COLORREF color, LPCTSTR string, int chars) rBool;
BOOL STORMAPI SGdiSetTargetDimensions(int width, int height, int bitdepth, int pitch) rBool;
BOOL STORMAPI SGdiGetTextExtent(LPCTSTR string, int chars, LPSIZE size) rBool;

BOOL STORMAPI Ordinal393(char *string, int, int) rBool;

void* STORMAPI SMemAlloc(DWORD bytes, LPCSTR filename, int linenumber, DWORD flags) rPVoid;

BOOL STORMAPI SMemFree(void *location, const char *logfilename, int logline, char defaultValue) rBool;

void* STORMAPI SMemReAlloc(void *location, size_t amount, const char *logfilename, int logline, char defaultValue) rPVoid;

BOOL STORMAPI SRegLoadData(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, LPVOID buffer, DWORD buffersize, DWORD* bytesread) rBool;
BOOL STORMAPI SRegLoadString(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, LPTSTR buffer, DWORD bufferchars) rBool;
BOOL STORMAPI SRegLoadValue(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, DWORD* value) rBool;
BOOL STORMAPI SRegSaveData(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, LPVOID data, DWORD databytes) rBool;
BOOL STORMAPI SRegSaveString(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, LPCTSTR string) rBool;
BOOL STORMAPI SRegSaveValue(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, DWORD value) rBool;
BOOL STORMAPI SRegGetBaseKey(DWORD flags, LPSTR buffer, DWORD buffersize) rBool;
BOOL STORMAPI SRegDeleteValue(LPCTSTR keyname, LPCTSTR valuename, DWORD flags) rBool;

BOOL STORMAPI STransBlt(LPBYTE dest, int destx, int desty, int destpitch, HANDLE transparency) rBool;
BOOL STORMAPI STransBltUsingMask(LPBYTE dest, LPBYTE source, int destpitch, int sourcepitch, HANDLE mask) rBool;
BOOL STORMAPI STransCreateI(LPBYTE bits, int width, int height, int bitdepth, LPRECT rect, COLORREF colorkey, HANDLE* handle) rBool;
BOOL STORMAPI STransDelete(HANDLE handle) rBool;

BOOL STORMAPI STransDuplicate(HANDLE source, HANDLE* handle) rBool;
BOOL STORMAPI STransIntersectDirtyArray(HANDLE sourcemask, LPBYTE dirtyarray, BYTE dirtyarraymask, HANDLE* handle) rBool;
BOOL STORMAPI STransInvertMask(HANDLE sourcemask, HANDLE* handle) rBool;
BOOL STORMAPI STransLoadI(LPCTSTR filename, LPRECT rect, COLORREF colorkey, HANDLE* handle) rBool;
BOOL STORMAPI STransSetDirtyArrayInfo(int screencx, int screency, int cellcx, int cellcy) rBool;
BOOL STORMAPI STransUpdateDirtyArray(LPBYTE dirtyarray, BYTE dirtyvalue, int destx, int desty, HANDLE transparency, BOOL tracecontour) rBool;
BOOL STORMAPI STransIsPixelInMask(HANDLE mask, int offsetx, int offsety) rBool;
BOOL STORMAPI STransCombineMasks(HANDLE basemask, HANDLE secondmask, int offsetx, int offsety, DWORD flags, HANDLE* handle) rBool;
BOOL STORMAPI STransCreateMaskI(LPBYTE bits, int width, int height, int bitdepth, LPRECT rect, COLORREF colorkey, HANDLE* handle) rBool;
BOOL STORMAPI STransCreateE(LPBYTE bits, int width, int height, int bitdepth, LPRECT rect, COLORREF colorkey, HANDLE* handle) rBool;
BOOL STORMAPI STransCreateMaskE(LPBYTE bits, int width, int height, int bitdepth, LPRECT rect, COLORREF colorkey, HANDLE* handle) rBool;
BOOL STORMAPI STransLoadE(LPCTSTR filename, LPRECT rect, COLORREF colorkey, HANDLE* handle) rBool;

BOOL STORMAPI SVidDestroy() rBool;
BOOL STORMAPI SVidGetSize(HANDLE video, int* width, int* height, int* bitdepth) rBool;
BOOL STORMAPI SVidInitialize(HANDLE directsound) rBool;
BOOL STORMAPI SVidPlayBegin(LPCTSTR filename, LPVOID destbuffer, LPCRECT destrect, LPSIZE destsize, SVIDPALETTEUSEPTR paletteuse, DWORD flags, HANDLE* handle) rBool;
BOOL STORMAPI SVidPlayBeginFromMemory(LPVOID sourceptr, DWORD sourcebytes, LPVOID destbuffer, LPCRECT destrect, LPSIZE destsize, SVIDPALETTEUSEPTR paletteuse, DWORD flags, HANDLE* handle) rBool;
BOOL STORMAPI SVidPlayContinue() rBool;
BOOL STORMAPI SVidPlayContinueSingle(HANDLE video, BOOL forceupdate, BOOL* updated) rBool;
BOOL STORMAPI SVidPlayEnd(HANDLE video) rBool;
BOOL STORMAPI SVidSetVolume(HANDLE video, LONG volume, LONG pan, DWORD track) rBool;

BOOL STORMAPI SErrDisplayError(DWORD dwErrMsg, const char *logfilename, int logline, const char *message, BOOL allowOption, int exitCode) rBool;
BOOL STORMAPI SErrGetErrorStr(DWORD errorcode, LPTSTR buffer, DWORD bufferchars) rBool;
DWORD STORMAPI SErrGetLastError() rInt;

BOOL STORMAPI SErrRegisterMessageSource(WORD facility, HMODULE module, LPVOID reserved) rBool;

void STORMAPI SErrReportResourceLeak(LPCTSTR handlename) rVoid;
void STORMAPI SErrSetLastError(DWORD dwErrCode) rVoid;

void STORMAPI SErrSuppressErrors(BOOL suppressErrors) rVoid;

BOOL  STORMAPI SCmdCheckId(DWORD id) rBool;
BOOL  STORMAPI SCmdGetBool(DWORD id) rBool;
DWORD STORMAPI SCmdGetNum(DWORD id) rInt;
BOOL  STORMAPI SCmdGetString(DWORD id, LPTSTR buffer, DWORD bufferchars) rBool;
BOOL  STORMAPI SCmdProcess(LPCTSTR cmdline, BOOL skipprogname, SCMDEXTRACALLBACK extracallback, SCMDERRORCALLBACK errorcallback) rBool;
BOOL  STORMAPI SCmdRegisterArgList(const ARGLIST* listptr, DWORD numargs) rBool;
BOOL  STORMAPI SCmdRegisterArgument(DWORD flags, DWORD id, LPCTSTR name, LPVOID variableptr, DWORD variablebytes, DWORD setvalue, DWORD setmask, SCMDCALLBACK callback) rBool;

BOOL STORMAPI SMemFindNextBlock(HANDLE heap, LPVOID prevblock, LPVOID* nextblock, LPSMEMBLOCKDETAILS details) rBool;
BOOL STORMAPI SMemFindNextHeap(HANDLE prevheap, HANDLE* nextheap, LPSMEMHEAPDETAILS details) rBool;
HANDLE STORMAPI SMemGetHeapByCaller(LPCSTR filename, int linenumber) rPVoid;
HANDLE STORMAPI SMemGetHeapByPtr(LPVOID ptr) rPVoid;
LPVOID STORMAPI SMemHeapAlloc(HANDLE handle, DWORD flags, DWORD bytes) rPVoid;
HANDLE STORMAPI SMemHeapCreate(DWORD options, DWORD initialsize, DWORD maximumsize) rPVoid;
BOOL STORMAPI SMemHeapDestroy(HANDLE handle) rBool;
BOOL STORMAPI SMemHeapFree(HANDLE handle, DWORD flags, LPVOID ptr) rBool;

void STORMAPI SMemCopy(void *dest, const void *source, size_t size) rVoid;
void STORMAPI SMemFill(void *location, size_t length, char fillWith) rVoid;

void STORMAPI SMemZero(void *location, DWORD length) rVoid;
int  STORMAPI SMemCmp(void *location1, void *location2, DWORD size) rInt;

DWORD STORMAPI SStrCopy(LPTSTR dest, LPCTSTR src, DWORD destsize) rInt;
DWORD STORMAPI SStrHash(LPCTSTR string, DWORD flags, DWORD seed) rInt;
DWORD STORMAPI SStrPack(LPTSTR*dest, LPCTSTR source, DWORD destsize) rInt;

void STORMAPI SStrTokenize(LPCTSTR* string, LPTSTR buffer, DWORD bufferchars, LPCTSTR whitespace, BOOL* quoted) rVoid;

LPTSTR STORMAPI SStrChr(LPCTSTR string, char ch, BOOL reverse) rPVoid;
DWORD STORMAPI SStrLen(const char* string) rInt;

int STORMAPI SStrCmp(const char *string1, const char *string2, size_t size) rInt;
int STORMAPI SStrCmpI(const char *string1, const char *string2, size_t size) rInt;
char* STORMAPI SStrUpper(char* string) rPVoid;

void STORMAPI SRgnClear(HANDLE handle) rVoid;

void STORMAPI SRgnCombineRect(HANDLE handle, RECT* rect, LPVOID param, int combinemode) rVoid;
void STORMAPI SRgnCreate(HANDLE* handle, DWORD reserved) rVoid;
void STORMAPI SRgnDelete(HANDLE handle) rVoid;

void STORMAPI SRgnDuplicate(HANDLE orighandle, HANDLE* handle, DWORD reserved) rVoid;
void STORMAPI SRgnGetRectParams(HANDLE handle, LPCRECT rect, DWORD* numparams, LPVOID* buffer) rVoid;
void STORMAPI SRgnGetRects(HANDLE handle, DWORD* numrects, LPRECT buffer) rVoid;
void STORMAPI SRgnGetBoundingRect(HANDLE handle, LPRECT rect) rVoid;


void STORMAPI SLogClose(HANDLE log) rVoid;
BOOL STORMAPI SLogCreate(LPCTSTR filename, DWORD flags, HANDLE* log) rBool;

void STORMAPI SLogDump(HANDLE log, LPCVOID data, DWORD bytes) rVoid;
void STORMAPI SLogFlush(HANDLE log) rVoid;
void STORMAPI SLogFlushAll() rVoid;
void __cdecl SLogPend(HANDLE log, LPCTSTR format, ...) rVoid;
void __cdecl SLogWrite(HANDLE log, LPCTSTR format, ...) rVoid;


BOOL STORMAPI SCompCompress(LPVOID dest, DWORD* destsize, LPCVOID source, DWORD sourcesize, DWORD compressiontypes, DWORD hint, DWORD optimization) rBool;
BOOL STORMAPI SCompDecompress(LPVOID dest, DWORD* destsize, LPCVOID source, DWORD sourcesize) rBool;

void STORMAPI SLogVWrite(HANDLE log, LPCTSTR format, va_list va) rVoid;

BOOL SErrDisplayErrorFmt(DWORD dwErrMsg, const char *logfilename, int logline, BOOL allowOption, int exitCode, const char *format, ...) rBool;

void STORMAPI SErrCatchUnhandledExceptions() rVoid;

LPTSTR STORMAPI SStrChrF(LPCTSTR string, char ch) rPVoid;
LPTSTR STORMAPI SStrChrR(LPCTSTR string, char ch) rPVoid;

size_t SStrVPrintf(char *dest, size_t size, const char *format, ...) rInt;

int STORMAPI SBigDel(void *buffer) rInt;

int STORMAPI SBigFromBinary(void *buffer, const void *str, size_t size) rInt;

int STORMAPI SBigNew(void **buffer) rInt;

int STORMAPI SBigPowMod(void *buffer1, void *buffer2, int a3, int a4) rInt;

int STORMAPI SBigToBinaryBuffer(void *buffer, int length, int a3, int a4) rInt;
