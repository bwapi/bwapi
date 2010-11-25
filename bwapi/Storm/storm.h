#pragma once
#include <windows.h>
#include <WinUser.h>
#include <Winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <ddraw.h>

/* Note to self: If you get a linker error then you were retarded
 *               and forgot to return a value in the cpp.
 */

#ifndef STORMAPI
#define STORMAPI __stdcall
#endif

#define arrsize(x) (sizeof(x)/sizeof(x[0]))   // Official name
#define countof(x) arrsize(x)                 // Personal name

#ifndef BLIZZ_STRUCTS
#define BLIZZ_STRUCTS
struct WRECT
{
  WORD  left;
  WORD  top;
  WORD  right;
  WORD  bottom;
};

struct WPOINT
{
  WORD  x;
  WORD  y;
};

struct WSIZE
{
  WORD  cx;
  WORD  cy;
};
#endif

#ifndef SGAME_STATE
#define SGAME_STATE

#define GAMESTATE_PRIVATE 0x01
#define GAMESTATE_FULL    0x02
#define GAMESTATE_ACTIVE  0x04
#define GAMESTATE_STARTED 0x08
#define GAMESTATE_REPLAY  0x80
#endif

BOOL STORMAPI SNetCreateGame(const char *pszGameName, const char *pszGamePassword, const char *pszGameStatString, DWORD dwGameType, char *GameTemplateData, int GameTemplateSize, int playerCount, char *creatorName, char *a11, int *playerID);
BOOL STORMAPI SNetDestroy();
BOOL STORMAPI SNetEnumProviders(int (STORMAPI *callback)(DWORD, DWORD, DWORD, DWORD), int mincaps);

BOOL STORMAPI SNetEnumGames(int (STORMAPI *callback)(DWORD, DWORD, DWORD), int *hintnextcall);
BOOL STORMAPI SNetDropPlayer(int playerid, DWORD flags);
BOOL STORMAPI SNetGetGameInfo(int type, char *src, unsigned int length, int *byteswritten);

#ifndef GAME_INFO
#define GAME_INFO

#define GAMEINFO_NAME     1
#define GAMEINFO_PASSWORD 2
#define GAMEINFO_STATS    3
#define GAMEINFO_MODEFLAG 4
#define GAMEINFO_UNK_5    5
#define GAMEINFO_UNK_6    6

#endif

BOOL STORMAPI SNetGetNumPlayers(int *firstplayerid, int *lastplayerid, int *activeplayers);

#ifndef CAPS_STRUCT
#define CAPS_STRUCT
struct caps
{
  DWORD dwSize;
  DWORD dwUnk1;
  DWORD dwBufferSize;
  DWORD dwUnk3;
  DWORD dwUnk4;
  DWORD dwUnk5;
  DWORD dwPingTimeout;
  DWORD dwPlayerCount;
  DWORD dwLatencyCalls;
};
#endif
BOOL STORMAPI SNetGetPlayerCaps(char playerid, caps *playerCaps);
BOOL STORMAPI SNetGetPlayerName(int playerid, char *buffer, size_t buffersize);
BOOL STORMAPI SNetGetProviderCaps(caps *providerCaps);
BOOL STORMAPI SNetGetTurnsInTransit(int *turns);
BOOL STORMAPI SNetInitializeDevice(int a1, int a2, int a3, int a4, int *a5);

#ifndef PROVIDER_STRUCTS
#define PROVIDER_STRUCTS
struct clientInfo
{
  DWORD dwSize; // 60
  char  *pszName;
  char  *pszVersion;
  DWORD dwProduct;
  DWORD dwVerbyte;
  DWORD dwUnk5;
  DWORD dwMaxPlayers;
  DWORD dwUnk7;
  DWORD dwUnk8;
  DWORD dwUnk9;
  DWORD dwUnk10; // 0xFF
  char  *pszCdKey;
  char  *pszCdOwner;
  DWORD dwIsShareware;
  DWORD dwLangId;
};

struct userInfo
{
  DWORD dwSize; // 16
  char  *pszPlayerName;
  char  *pszUnknown;
  DWORD dwUnknown;
};

struct battleInfo
{
  DWORD dwSize;   // 92
  DWORD dwUnkType;
  HWND  hFrameWnd;
  void  *pfnBattleGetResource;
  void  *pfnBattleGetErrorString;
  void  *pfnBattleMakeCreateGameDialog;
  void  *pfnBattleUpdateIcons;
  DWORD dwUnk_07;
  void  *pfnBattleErrorDialog;
  void  *pfnBattlePlaySound;
  DWORD dwUnk_10;
  void  *pfnBattleGetCursorLink;
  DWORD dwUnk_12;
  void  *pfnUnk_13;
  DWORD dwUnk_14;
  void  *pfnBattleMakeProfileDialog;
  char  *pszProfileStrings;
  void  *pfnBattleDrawProfileInfo;
  void  *pfnUnk_18;
  DWORD dwUnk_19;
  void  *pfnUnk_20;
  void  *pfnUnk_21;
  void  *pfnBattleSetLeagueName;
};

struct moduleInfo
{
  DWORD dwSize; // 20
  char  *pszVersionString;
  char  *pszModuleName;
  char  *pszMainArchive;
  char  *pszPatchArchive;
};

struct gameStruc
{
  DWORD     dwIndex;
  DWORD     dwGameState;
  DWORD     dwUnk_08;
  SOCKADDR  saHost;
  DWORD     dwUnk_1C;
  DWORD     dwTimer;
  DWORD     dwUnk_24;
  char      szGameName[128];
  char      szGameStatString[128];
  gameStruc *pNext;
  void      *pExtra;
  DWORD     dwExtraBytes;
  DWORD     dwProduct;
  DWORD     dwVersion;
};

struct stormHead
{
  WORD wChecksum;
  WORD wLength;
  WORD wSent;
  WORD wReceived;
  BYTE bCommandClass;
  BYTE bCommandType;
  BYTE bPlayerId;
  BYTE bFlags;
};

#define STRAFFIC_NORMAL 0
#define STRAFFIC_VERIFY 1
#define STRAFFIC_RESEND 2
#define STRAFFIC_REPLY  4

#endif
BOOL STORMAPI SNetInitializeProvider(DWORD providerName, clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData);
BOOL STORMAPI SNetJoinGame(unsigned int a1, char *gameName, char *gamePassword, char *playerName, char *userStats, int *playerid);

/*  SNetLeaveGame @ 119
 * 
 *  Notifies Storm that you have left the game. Storm will notify all connected peers through the network provider.
 *  
 *  type: The leave type. It's not particularly important.
 *
 *  Returns TRUE if the function was called successfully and FALSE otherwise.
 */
BOOL
STORMAPI
SNetLeaveGame(
    __in int type);

BOOL STORMAPI SNetPerformUpgrade(DWORD *upgradestatus);
BOOL STORMAPI SNetReceiveMessage(int *senderplayerid, char **data, int *databytes);
BOOL STORMAPI SNetReceiveTurns(int a1, int arraysize, char **arraydata, unsigned int *arraydatabytes, DWORD *arrayplayerstatus);

// Values for arrayplayerstatus
#ifndef SNET_PS_
#define SNET_PS_

#define SNET_PS_OK             0
#define SNET_PS_WAITING        2
#define SNET_PS_NOTRESPONDING  3
#define SNET_PS_UNKNOWN        default

#endif

#ifndef STORM_EVENT
#define STORM_EVENT

struct s_evt
{
  DWORD dwFlags;
  int   dwPlayerId;
  BYTE  *pData;
  DWORD dwSize;
};
#endif

HANDLE STORMAPI SNetRegisterEventHandler(int type, void (STORMAPI *sEvent)(s_evt *evt));

int  STORMAPI SNetSelectGame(int a1, int a2, int a3, int a4, int a5, int *playerid);

BOOL STORMAPI SNetSendMessage(int playerID, char *data, unsigned int databytes);
BOOL STORMAPI SNetSendTurn(char *data, unsigned int databytes);

#ifndef SNPLAYER_
#define SNPLAYER_

#define SNPLAYER_ALL    -1
#define SNPLAYER_OTHERS -2

#endif

BOOL STORMAPI SNetSetGameMode(DWORD modeFlags, char a2);

BOOL STORMAPI SNetEnumGamesEx(int a1, int a2, int (__fastcall *callback)(DWORD, DWORD, DWORD), int *hintnextcall);
int  STORMAPI SNetSendServerChatCommand(const char *command);

BOOL STORMAPI SNetGetPlayerNames(DWORD flags);
BOOL STORMAPI SNetCreateLadderGame(const char *pszGameName, const char *pszGamePassword, const char *pszGameStatString, DWORD dwGameType, DWORD dwGameLadderType, DWORD dwGameModeFlags, char *GameTemplateData, int GameTemplateSize, int playerCount, char *creatorName, char *a11, int *playerID);
BOOL STORMAPI SNetReportGameResult(unsigned int a1, int size, int a3, int a4, int a5);

int  STORMAPI SNetSendLeagueCommand(char *cmd, char *callback);
int  STORMAPI SNetSendReplayPath(int a1, int a2, char *replayPath);
int  STORMAPI SNetGetLeagueName(int leagueID);
BOOL STORMAPI SNet144(char *buffer);
int  STORMAPI SNetLeagueLogout(char *bnetName);
int  STORMAPI SNetGetLeaguePlayerName(int *curPlayerID, size_t nameSize);

HGDIOBJ STORMAPI SDlgDefDialogProc(HWND hDlg, signed int DlgType, HDC textLabel, HWND hWnd);

HANDLE STORMAPI SDlgDialogBoxIndirectParam(HMODULE hModule, LPCSTR lpName, HWND hWndParent, LPVOID lpParam, LPARAM lParam);

BOOL STORMAPI SDlgEndDialog(HWND hDlg, HANDLE nResult);

BOOL STORMAPI SDlgSetControlBitmaps(HWND parentwindow, int *id, int a3, char *buffer2, char *buffer, int flags, int mask);

BOOL STORMAPI SDlgBltToWindowE(HWND hWnd, HRGN a2, char *a3, int a4, void *buffer, RECT *rct, SIZE *size, int a8, int a9, DWORD rop);
BOOL STORMAPI SDlgSetBitmapE(HWND hWnd, int a2, char *src, int mask1, int flags, int a6, int a7, int width, int a9, int mask2);

int  STORMAPI Ordinal224(int a1);

BOOL STORMAPI SFileCloseArchive(HANDLE hArchive);
BOOL STORMAPI SFileCloseFile(HANDLE hFile);

BOOL STORMAPI SFileDdaBeginEx(HANDLE directsound, DWORD flags, DWORD mask, unsigned __int32 lDistanceToMove, signed __int32 volume, signed int a6, int a7);
BOOL STORMAPI SFileDdaDestroy();
BOOL STORMAPI SFileDdaEnd(HANDLE directsound);
BOOL STORMAPI SFileDdaGetPos(HANDLE directsound, int a2, int a3);

BOOL STORMAPI SFileDdaInitialize(HANDLE directsound);
BOOL STORMAPI SFileDdaSetVolume(HANDLE directsound, signed int bigvolume, signed int volume);

BOOL STORMAPI SFileGetFileArchive(HANDLE hFile, HANDLE archive);
LONG STORMAPI SFileGetFileSize(HANDLE hFile, LPDWORD lpFileSizeHigh);
BOOL STORMAPI SFileOpenArchive(const char *szMpqName, DWORD dwPriority, DWORD dwFlags, HANDLE *phMpq);

// values for dwFlags
#ifndef MPQ_
#define MPQ_

#define MPQ_NO_LISTFILE       0x0010
#define MPQ_NO_ATTRIBUTES     0x0020
#define MPQ_FORCE_V1          0x0040
#define MPQ_CHECK_SECTOR_CRC  0x0080

#endif

BOOL STORMAPI SFileOpenFile(const char *filename, HANDLE *phFile);
BOOL STORMAPI SFileOpenFileEx(HANDLE hMpq, const char *szFileName, DWORD dwSearchScope, HANDLE *phFile);

// values for dwSearchScope
#ifndef SFILE_
#define SFILE_

#define SFILE_FROM_MPQ        0x00000000
#define SFILE_FROM_ABSOLUTE   0x00000001
#define SFILE_FROM_RELATIVE   0x00000002
#define SFILE_UNKNOWN_04      0x00000004

#endif

BOOL STORMAPI SFileReadFile(HANDLE hFile, void *buffer, DWORD nNumberOfBytesToRead, DWORD *read, LONG lpDistanceToMoveHigh);

void STORMAPI SFileSetLocale(LCID lcLocale);

BOOL STORMAPI SFileSetIoErrorMode(int mode, BOOL (STORMAPI *callback)(char*,int,int) );
BOOL STORMAPI SFileGetArchiveName(HANDLE hArchive, char *name, int length);
BOOL STORMAPI SFileGetFileName(HANDLE hFile, char *buffer, int length);

BOOL STORMAPI SFileLoadFile(char *filename, void *buffer, int buffersize, int a4, int a5);
BOOL STORMAPI SFileUnloadFile(HANDLE hFile);
BOOL STORMAPI SFileLoadFileEx(void *hArchive, char *filename, int a3, int a4, int a5, DWORD searchScope, struct _OVERLAPPED *lpOverlapped);

BOOL STORMAPI SBltROP3(void *lpDstBuffer, void *lpSrcBuffer, int width, int height, int a5, int a6, int a7, DWORD rop);
BOOL STORMAPI SBltROP3Clipped(void *lpDstBuffer, RECT *lpDstRect, POINT *lpDstPt, int a4, void *lpSrcBuffer, RECT *lpSrcRect, POINT *lpSrcPt, int a8, int a9, DWORD rop);

#ifndef _SBMP
#define _SBMP

#define SBMP_DEFAULT  0
#define SBMP_BMP      1
#define SBMP_PCX      2
#define SBMP_TGA      3

#endif


/*  SBmpDecodeImage @ 321
 * 
 *  Decodes an image that has already been loaded into a buffer.
 *  
 *  dwImgType:        Optional, the image type. See SBMP_ macros.
 *  pSrcBuffer:       A pointer to the source buffer.
 *  dwSrcBuffersize:  The size of the data in the source buffer.
 *  pPalette:         An optional buffer that receives the image palette.
 *  pDstBuffer:       A buffer that receives the image data.
 *  dwDstBuffersize:  The size of the specified image buffer. If the size of the 
 *                    destination buffer is 0, then the destination buffer is not used.
 *  pdwWidth:         An optional variable that receives the image width.
 *  pdwHeight:        An optional variable that receives the image height.
 *  pdwBpp:           An optional variable that receives the image bits per pixel.
 *
 *  Returns TRUE if the image was supported and decoded correctly, FALSE otherwise.
 */
BOOL 
STORMAPI 
SBmpDecodeImage(
    __in_opt  DWORD        dwImgType,
    __in      void         *pSrcBuffer,
    __in      DWORD        dwSrcBuffersize,
    __out_opt PALETTEENTRY *pPalette        = NULL,
    __out     void         *pDstBuffer      = NULL,
    __out     DWORD        dwDstBuffersize  = 0,
    __out_opt DWORD        *pdwWidth        = NULL,
    __out_opt DWORD        *pdwHeight       = NULL,
    __out_opt DWORD        *pdwBpp          = NULL);


/*  SBmpLoadImage @ 323
 * 
 *  Load an image from an available archive into a buffer.
 *  
 *  pszFileName:  The name of the graphic in an active archive.
 *  pPalette:     An optional buffer that receives the image palette.
 *  pBuffer:      A buffer that receives the image data.
 *  dwBuffersize: The size of the specified image buffer.
 *  pdwWidth:     An optional variable that receives the image width.
 *  pdwHeight:    An optional variable that receives the image height.
 *  pdwBpp:       An optional variable that receives the image bits per pixel.
 *
 *  Returns TRUE if the image was supported and loaded correctly, FALSE otherwise.
 */
BOOL
STORMAPI
SBmpLoadImage(
    __in      const char   *pszFileName,
    __out_opt PALETTEENTRY *pPalette    = NULL,
    __out     void         *pBuffer     = NULL,
    __out     DWORD        dwBuffersize = 0,
    __out_opt DWORD        *pdwWidth    = NULL,
    __out_opt DWORD        *pdwHeight   = NULL,
    __out_opt DWORD        *pdwBpp      = NULL);


HANDLE STORMAPI SBmpAllocLoadImage(const char *fileName, PALETTEENTRY *palette, void **buffer, int *width, int *height, int unused6, int unused7, void *(STORMAPI *allocFunction)(DWORD));

BOOL STORMAPI SCodeCompile(char *directives1, char *directives2, char *loopstring, unsigned int maxiterations, unsigned int flags, HANDLE handle);
BOOL STORMAPI SCodeDelete(HANDLE handle);

int  STORMAPI SCodeExecute(HANDLE handle, int a2);

BOOL STORMAPI SDrawAutoInitialize(HINSTANCE hInst, LPCSTR lpClassName, LPCSTR lpWindowName, WNDPROC pfnWndProc, int nMode, int nWidth, int nHeight, int nBits);


/*  SDrawCaptureScreen @ 342
 * 
 *  Saves a screenshot from the primary surface being handled by Storm.
 *  
 *  pszOutput: The name of the output file. The save format is automatically set by the extension.
 *             The extensions supported are .gif, .pcx, .tga, and .bmp. It will write a bitmap by default.
 *
 *  Returns TRUE if successful and FALSE otherwise.
 */
BOOL 
STORMAPI 
SDrawCaptureScreen(
    __in char *pszOutput);


/*  SDrawGetFrameWindow @ 346
 *
 *  Retrieves the window handle that was specified in
 *  SDrawManualInitialize or created in SDrawAutoInitialize.
 *  
 *  sdraw_framewindow: Optional variable that receives the returned handle.
 *
 *  Returns the handle of the window.
 */
HWND
STORMAPI
SDrawGetFrameWindow(
    __out_opt HWND *sdraw_framewindow = NULL);


/*  SDrawGetObjects @ 347
 *  
 *  Retrieves the object information that was initialized using
 *  SDrawManualInitialize or SDrawAutoInitialize.
 *  
 *  ddInterface:    The DirectDraw interface.
 *  primarySurface: The primary DirectDraw surface.
 *  surface2:       A second unknown surface.
 *  surface3:       A third unknown surface.
 *  backSurface:    The back DirectDraw surface.
 *  ddPalette:      The DirectDraw palette.
 *  hPalette:       The palette handle.
 *
 *  Returns FALSE if the direct draw interface has not been initialized.
 */
BOOL
STORMAPI
SDrawGetObjects(
    __out_opt LPDIRECTDRAW        *ddInterface    = NULL,
    __out_opt LPDIRECTDRAWSURFACE *primarySurface = NULL,
    __out_opt LPDIRECTDRAWSURFACE *surface2       = NULL,
    __out_opt LPDIRECTDRAWSURFACE *surface3       = NULL,
    __out_opt LPDIRECTDRAWSURFACE *backSurface    = NULL,
    __out_opt LPDIRECTDRAWPALETTE *ddPalette      = NULL,
    __out_opt HPALETTE            *hPalette       = NULL);


/*  SDrawGetScreenSize @ 348
 *  
 *  Obtains information for the current screen resolution.
 *  SDrawManualInitialize or created in SDrawAutoInitialize.
 *
 *  pdwWidth:   Optional variable that receives the screen width.
 *  pdwHeight:  Optional variable that receives the screen height.
 *  pdwBpp:     Optional variable that receives the bits per pixel.
 *
 *  Returns FALSE if no variables were specified.
 */
BOOL
STORMAPI
SDrawGetScreenSize(
    __out_opt DWORD *pdwWidth,
    __out_opt DWORD *pdwHeight,
    __out_opt DWORD *pdwBpp);


// undefined
BOOL STORMAPI SDrawLockSurface(int surfacenumber, RECT *lpDestRect, void **lplpSurface, int *lpPitch, int arg_unused);


/*  SDrawManualInitialize @ 351
 *  
 *  Sets the DirectDraw variables to be referenced in Storm.
 *
 *  hWnd:           The handle of the DirectDraw window.
 *  ddInterface:    The DirectDraw interface.
 *  primarySurface: The first and primary surface.
 *  surface2:       A second surface. Behaviour not completely known.
 *  surface3:       A third surface. Behaviour not completely known.
 *  backSurface:    The fourth and final surface. The back surface.
 *  ddPalette:      The DirectDraw palette if the application requires it.
 *  hPalette:       The palette handle that belongs to the window.
 *                  If this is NULL and ddPalette is specified, then it
 *                  will be created automatically. A palette can be created
 *                  using the CreatePalette WinAPI function.
 *
 *  Returns FALSE if no variables were specified.
 */
BOOL
STORMAPI
SDrawManualInitialize(
    __in_opt  HWND hWnd                           = NULL,
    __in_opt  LPDIRECTDRAW ddInterface            = NULL,
    __in_opt  LPDIRECTDRAWSURFACE primarySurface  = NULL,
    __in_opt  LPDIRECTDRAWSURFACE surface2        = NULL,
    __in_opt  LPDIRECTDRAWSURFACE surface3        = NULL,
    __in_opt  LPDIRECTDRAWSURFACE backSurface     = NULL,
    __in_opt  LPDIRECTDRAWPALETTE ddPalette       = NULL,
    __in_opt  HPALETTE hPalette                   = NULL);


/*  SDrawPostClose @ 353
 *
 *  Posts a WM_QUIT message to the active drawing window specified 
 *  in SDrawManualInitialize or created in SDrawAutoInitialize.
 *
 *  Returns TRUE if successful and FALSE otherwise.
 */
BOOL
STORMAPI
SDrawPostClose();


// undefined
BOOL STORMAPI SDrawRealizePalette();

BOOL STORMAPI SDrawUnlockSurface(int surfacenumber, void *lpSurface, int a3, RECT *lpRect);
BOOL STORMAPI SDrawUpdatePalette(unsigned int firstentry, unsigned int numentries, PALETTEENTRY *pPalEntries, int a4);

BOOL STORMAPI SEvtDispatch(DWORD dwMessageID, DWORD dwFlags, int type, s_evt *pEvent);

BOOL STORMAPI SGdiDeleteObject(HANDLE handle);

BOOL STORMAPI SGdiExtTextOut(int a1, int a2, int a3, int a4, unsigned int a8, signed int a6, signed int a7, const char *string, unsigned int arg20);
BOOL STORMAPI SGdiImportFont(HGDIOBJ handle, int windowsfont);

BOOL STORMAPI SGdiSelectObject(int handle);
BOOL STORMAPI SGdiSetPitch(int pitch);

BOOL STORMAPI Ordinal393(char *string, int, int);


void* STORMAPI SMemAlloc(int amount, char *logfilename, int logline, char defaultValue = 0);
#ifndef SMAlloc
#define SMAlloc(amount) SMemAlloc((amount), __FILE__, __LINE__)
#endif


BOOL STORMAPI SMemFree(void *location, char *logfilename, int logline, char defaultValue = 0);
#ifndef SMFree
#define SMFree(loc) SMemFree((loc), __FILE__, __LINE__)
#endif


#ifndef SLOG_
#define SLOG_

#define SLOG_EXPRESSION    0
#define SLOG_FUNCTION     -1
#define SLOG_OBJECT       -2
#define SLOG_HANDLE       -3
#define SLOG_FILE         -4

#endif

BOOL STORMAPI SRegLoadData(char *keyname, char *valuename, int size, LPBYTE lpData, BYTE flags, LPDWORD lpcbData);
BOOL STORMAPI SRegLoadString(char *keyname, char *valuename, BYTE flags, char *buffer, size_t buffersize);
BOOL STORMAPI SRegLoadValue(char *keyname, char *valuename, BYTE flags, int *value);
BOOL STORMAPI SRegSaveData(char *keyname, char *valuename, int size, BYTE *lpData, DWORD cbData);
BOOL STORMAPI SRegSaveString(char *keyname, char *valuename, BYTE flags, char *string);
BOOL STORMAPI SRegSaveValue(char *keyname, char *valuename, BYTE flags, DWORD result);

BOOL STORMAPI SRegDeleteValue(char *keyname, char *valuename, BYTE flags);

#ifndef SREG_
#define SREG_
// Flags for SReg functions; Default is CURRENT_USER and "Blizzard Entertainment" key

#define SREG_CURRENT_USER  0x00000001
#define SREG_BATTLE_NET    0x00000002
#define SREG_LOCAL_MACHINE 0x00000004
#define SREG_RAW_KEY       0x00000010

#endif

BOOL STORMAPI STransBlt(HANDLE hTrans, int a2, int a3, int a4, int a5);
BOOL STORMAPI STransBltUsingMask(void *lpSurface, void *pMask, int pitch, int width, HANDLE hTrans);

BOOL STORMAPI STransDelete(HANDLE hTrans);

BOOL STORMAPI STransDuplicate(HANDLE hTransSource, HANDLE hTransDest);
BOOL STORMAPI STransIntersectDirtyArray(HANDLE hTrans, int dirtyarraymask, unsigned __int8 dirtyarray, int sourcemask);
BOOL STORMAPI STransInvertMask(HANDLE hTrans, int sourcemask);

BOOL STORMAPI STransSetDirtyArrayInfo(int width, int height, int depth, int bits);

BOOL STORMAPI STrans442(int a1, int a2, int a3, char *a4, size_t a5);
BOOL STORMAPI STransCombineMasks(HANDLE hTrans, int a2, int a3, int a4, int depth, int a6);

BOOL STORMAPI STransCreateE(void *pBuffer, int width, int height, int bpp, int a5, int bufferSize, HANDLE *phTransOut);

BOOL STORMAPI SVidDestroy();
BOOL STORMAPI SVidGetSize(HANDLE video, int width, int height, int zero);
BOOL STORMAPI SVidInitialize(HANDLE video);
BOOL STORMAPI SVidPlayBegin(char *filename, int arg4, int a3, int a4, int a5, int a6, HANDLE video);

BOOL STORMAPI SVidPlayContinueSingle(HANDLE video, int a2, int a3);
BOOL STORMAPI SVidPlayEnd(HANDLE video);


/*  SErrGetErrorStr @ 462
 *  
 *  Retrieves a string that describes the specified error code for
 *  the system, Storm, DirectDraw, or DirectSound.
 *
 *  dwErrCode:    The error code to look up.
 *  buffer:       The destination buffer to receive the string.
 *  bufferchars:  The size of the destination buffer.
 *
 *  Returns TRUE if the call was successful and FALSE otherwise.
 */
BOOL
STORMAPI
SErrGetErrorStr(
    __in  DWORD dwErrCode,
    __out char *buffer,
    __in  size_t bufferchars);


/*  SErrGetLastError @ 463
 *  
 *  Retrieves the last error that was specifically
 *  set for the Storm library.
 *
 *  Returns the last error set within the Storm library.
 */
DWORD
STORMAPI
SErrGetLastError();


/*  SErrSetLastError @ 465
 *  
 *  Sets the last error for the Storm library and the Kernel32 library.
 *
 *  dwErrCode:  The error code that will be set.
 */
void
STORMAPI
SErrSetLastError(
    __in DWORD dwErrCode = NO_ERROR);


void STORMAPI SErrSuppressErrors(BOOL suppressErrors);

// Values for dwErrCode
#ifndef STORM_ERROR_
#define STORM_ERROR_

#define STORM_ERROR_ASSERTION                    0x85100000
#define STORM_ERROR_BAD_ARGUMENT                 0x85100065
#define STORM_ERROR_GAME_ALREADY_STARTED         0x85100066
#define STORM_ERROR_GAME_FULL                    0x85100067
#define STORM_ERROR_GAME_NOT_FOUND               0x85100068
#define STORM_ERROR_GAME_TERMINATED              0x85100069
#define STORM_ERROR_INVALID_PLAYER               0x8510006a
#define STORM_ERROR_NO_MESSAGES_WAITING          0x8510006b
#define STORM_ERROR_NOT_ARCHIVE                  0x8510006c
#define STORM_ERROR_NOT_ENOUGH_ARGUMENTS         0x8510006d
#define STORM_ERROR_NOT_IMPLEMENTED              0x8510006e
#define STORM_ERROR_NOT_IN_ARCHIVE               0x8510006f
#define STORM_ERROR_NOT_IN_GAME                  0x85100070
#define STORM_ERROR_NOT_INITIALIZED              0x85100071
#define STORM_ERROR_NOT_PLAYING                  0x85100072
#define STORM_ERROR_NOT_REGISTERED               0x85100073
#define STORM_ERROR_REQUIRES_CODEC1              0x85100074
#define STORM_ERROR_REQUIRES_CODEC2              0x85100075
#define STORM_ERROR_REQUIRES_CODEC3              0x85100076
#define STORM_ERROR_REQUIRES_UPGRADE             0x85100077
#define STORM_ERROR_STILL_ACTIVE                 0x85100078
#define STORM_ERROR_VERSION_MISMATCH             0x85100079
#define STORM_ERROR_MEM_NOT_ALLOCATED            0x8510007a
#define STORM_ERROR_MEM_CORRUPTED                0x8510007b
#define STORM_ERROR_MEM_INVALID                  0x8510007c
#define STORM_ERROR_MEM_MANAGER_NOT_INITIALIZED  0x8510007d
#define STORM_ERROR_MEM_NOT_FREED                0x8510007e
#define STORM_ERROR_RESOURCES_NOT_RELEASED       0x8510007f
#define STORM_ERROR_OUT_OF_BOUNDS                0x85100080
#define STORM_ERROR_NULL_POINTER                 0x85100081
#define STORM_ERROR_CDKEY_MISMATCH               0x85100082
#define STORM_ERROR_FILE_CORRUPTED               0x85100083
#define STORM_ERROR_FATAL                        0x85100084
#define STORM_ERROR_GAMETYPE_UNAVAILABLE         0x85100085

#endif


/*  SMemCopy @ 491
 *  
 *  Copies a block of memory from source to destination.
 *  This function immediately calls memcpy. See online documentation
 *  of memcpy for more details.
 *
 *  dest:   The destination buffer.
 *  source: The size of the destination buffer.
 *  size:   The format to use.
 */
void
STORMAPI
SMemCopy(
    __out void *dest, 
    __in  const void *source, 
    __in  size_t size);


/*  SMemFill @ 492
 *  
 *  Fills a block of memory with the specified character.
 *  This function immediately calls memset. See online documentation
 *  of memset for more details.
 *
 *  dest:   The destination buffer.
 *  source: The size of the destination buffer.
 *  size:   The format to use.
 */
void
STORMAPI
SMemFill(
    __in  void *location,
    __in  size_t length,
    __in  char fillWith = 0);


/*  SMemZero @ 494
 *  
 *  Fills a block of memory with the integer 0x00 (Zero).
 *
 *  location: The location to write at.
 *  length:   The amount of bytes to write.
 */
void 
STORMAPI 
SMemZero(
    __in  void *location,
    __in  size_t length);


int   STORMAPI SMemCmp(void *location1, void *location2, DWORD size);

int   STORMAPI SStrCopy(char *dest, const char *src, DWORD max_length);
DWORD STORMAPI SStrHash(const char *string, DWORD flags, DWORD Seed);
int   STORMAPI SStrNCat(char *dest, const char *src, DWORD max_length);

int   STORMAPI SStrLen(const char* string);

int   STORMAPI SStrCmp(const char *string1, const char *string2, size_t size);
int   STORMAPI SStrCmpI(const char *string1, const char *string2, size_t size);
char* STORMAPI SStrUpper(char* string);

void  STORMAPI SRgn523(int handle, RECT *rect, int a3, int a4);
void  STORMAPI SRgn524(int handle, int a2);
void  STORMAPI SRgn525(int handle);

void  STORMAPI SRgn529i(int handle, int a2, int a3);

char* STORMAPI SStrChr(const char *string, char c);
char* STORMAPI SStrChrR(const char *string, char c);


/*  SStrVPrintf @ 578
 *  
 *  Prints a formatted string to a destination buffer.
 *  This function calls vsnprintf with some extra error handling.
 *  See online documentation of vsnprintf for more details.
 *
 *  dest:   The destination buffer.
 *  size:   The size of the destination buffer.
 *  format: The format to use.
 *
 *  Returns the number of characters written.
 */
size_t
__cdecl
SStrVPrintf(
    __out char *dest, 
    __in  size_t size, 
    __in  const char *format, ...);


int STORMAPI SBigDel(void *buffer);

int STORMAPI SBigFromBinary(void *buffer, const void *str, size_t size);

int STORMAPI SBigNew(void **buffer);

int STORMAPI SBigPowMod(void *buffer1, void *buffer2, int a3, int a4);

int STORMAPI SBigToBinaryBuffer(void *buffer, int length, int a3, int a4);
