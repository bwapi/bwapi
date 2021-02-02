#pragma once

#ifndef __BLIZZARD_STORM_HEADER
#define __BLIZZARD_STORM_HEADER

// Note to self: Linker error => forgot a return value in cpp

#include <windows.h>
#include <ddraw.h>

#ifndef STORMAPI
#define STORMAPI __stdcall
#endif

#ifdef __cplusplus

#ifndef STORM_IGNORE_INFINITY
#include <limits>
static float infinity = std::numeric_limits<float>::infinity();
#endif

struct CCritSect {
  CRITICAL_SECTION m_critsect;

  CCritSect()
  {
    InitializeCriticalSection(&m_critsect);
  }
  ~CCritSect()
  {
    DeleteCriticalSection(&m_critsect);
  }
  void Enter()
  {
    EnterCriticalSection(&m_critsect);
  }
  void Leave()
  {
    LeaveCriticalSection(&m_critsect);
  }
};
#endif


#if defined(__GNUC__) || defined(__cplusplus)
extern "C" {
#endif

#pragma region SNet

// ################################################################################################
// DEFINES/ENUMS
// ################################################################################################

// Game states
#define SNET_GM_PRIVATE      0x01
#define SNET_GM_FULL         0x02
#define SNET_GM_ADVERTISED   0x04
#define SNET_GM_UNJOINABLE   0x08
#define SNET_GM_REPLAY       0x80

#define SNET_PSF_ACTIVE        0x10000
#define SNET_PSF_TURNAVAILABLE 0x20000
#define SNET_PSF_RESPONDING    0x40000

#define  SNET_BROADCASTNONLOCALPLAYERID   0xFFFFFFFE
#define  SNET_BROADCASTPLAYERID           0xFFFFFFFF
#define  SNET_INVALIDPLAYERID             0xFFFFFFFF

#define  SNET_CAPS_PAGELOCKEDBUFFERS      0x00000001
#define  SNET_CAPS_BASICINTERFACE         0x00000002
#define  SNET_CAPS_DEBUGONLY              0x10000000
#define  SNET_CAPS_RETAILONLY             0x20000000

#define SNET_EVENT_INITDATA      1
#define SNET_EVENT_PLAYERJOIN    2
#define SNET_EVENT_PLAYERLEAVE   3
#define SNET_EVENT_SERVERMESSAGE 4

#define SNET_EXIT_PLAYERQUIT    0x40000001
#define SNET_EXIT_PLAYERKILLED  0x40000002
#define SNET_EXIT_PLAYERWON     0x40000004
#define SNET_EXIT_GAMEOVER      0x40000005
#define SNET_EXIT_NOTRESPONDING 0x40000006

// Game info fields
#define SNET_INFO_GAMENAME        1
#define SNET_INFO_GAMEPASSWORD    2
#define SNET_INFO_GAMEDESCRIPTION 3
#define SNET_INFO_GAMEMODE        4
#define SNET_INFO_INITDATA        5
#define SNET_INFO_MAXPLAYERS      6

// Traffic flags
#define SNET_TRAFFIC_NORMAL 0
#define SNET_TRAFFIC_VERIFY 1
#define SNET_TRAFFIC_RESEND 2
#define SNET_TRAFFIC_REPLY  4

// Values for arrayplayerstatus
#define SNET_PS_OK             0
#define SNET_PS_WAITING        2
#define SNET_PS_NOTRESPONDING  3
#define SNET_PS_UNKNOWN        default

// ################################################################################################
// TYPES
// ################################################################################################

typedef struct _SNETCAPS
{
  DWORD size;                   // Size of this structure  // sizeof(CAPS)
  DWORD flags;                  // 
  DWORD maxmessagesize;         // Size of the packet buffer, must be beteen 128 and 512
  DWORD maxqueuesize;           // 
  DWORD maxplayers;             // Displayed player count in the mode selection list
  DWORD bytessec;               //
  DWORD latencyms;              //
  DWORD defaultturnssec;        // must be between 1 and 20
  DWORD defaultturnsintransit;  // the number of calls before data is sent over the network // between 2 and 8; single player is set to 1
} SNETCAPS, *SNETCAPSPTR;

typedef struct _SNETGAME {
  DWORD  size;
  DWORD  id;
  LPCSTR gamename;
  LPCSTR gamedescription;
  DWORD  categorybits;
  DWORD  numplayers;
  DWORD  maxplayers;
} SNETGAME, *SNETGAMEPTR;

typedef struct _SNETPROGRAMDATA
{
  DWORD size; // 60
  LPCSTR programName;
  LPCSTR programDescription;
  DWORD programid;
  DWORD versionid;
  DWORD reserved1;
  DWORD maxPlayers;
  LPVOID initData;
  DWORD initDataBytes;
  LPVOID reserved2;
  DWORD optcategorybits; // 0xFF
  LPCSTR cdkey;
  LPCSTR cdkeyOwner;
  DWORD isShareware;
  DWORD languageid;
} SNETPROGRAMDATA, *SNETPROGRAMDATAPTR;

typedef struct _SNETPLAYERDATA
{
  DWORD size; // 16
  LPCSTR playerName;
  LPCSTR playerDescription;
  LPCSTR displayedFields;
} SNETPLAYERDATA, *SNETPLAYERDATAPTR;

typedef struct _SNETUIDATA
{
  DWORD size;   // 92
  DWORD uiflags;
  HWND  parentwindow;
  void* pfnBattleGetResource;           // artcallback
  void* pfnBattleGetErrorString;        // authcallback
  void* pfnBattleMakeCreateGameDialog;  // createcallback
  void* pfnBattleUpdateIcons;           // drawdesccallback
  DWORD dwUnk_07;                       // selectedcallback
  void* pfnBattleErrorDialog;           // messageboxcallback
  void* pfnBattlePlaySound;             // soundcallback
  DWORD dwUnk_10;                       // statuscallback
  void* pfnBattleGetCursorLink;         // getdatacallback
  DWORD dwUnk_12;                       // categorycallback
  void* pfnUnk_13;                      // categorylistcallback
  DWORD dwUnk_14;                       // newaccountcallback
  void* pfnBattleMakeProfileDialog;     // profilecallback
  char* pszProfileStrings;
  void* pfnBattleDrawProfileInfo;
  void* pfnUnk_18;
  DWORD dwUnk_19;
  void* pfnUnk_20;
  void* pfnUnk_21;
  void* pfnBattleSetLeagueName;
} SNETUIDATA, *SNETUIDATAPTR;

typedef struct _SNETVERSIONDATA
{
  DWORD  size; // 20
  LPCSTR versionstring;
  LPCSTR executablefile;
  LPCSTR originalarchivefile;
  LPCSTR patcharchivefile;
} SNETVERSIONDATA, *SNETVERSIONDATAPTR;
/*
typedef struct _game
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
  _game* pNext;
  void* pExtra;
  DWORD     dwExtraBytes;
  DWORD     dwProduct;
  DWORD     dwVersion;
} game;

typedef struct _storm_head
{
  WORD wChecksum;
  WORD wLength;
  WORD wSent;
  WORD wReceived;
  BYTE bCommandClass;
  BYTE bCommandType;
  BYTE bPlayerId;
  BYTE bFlags;
} storm_head;
*/
// Event structure
typedef struct _SNETEVENT {
  DWORD  eventid;
  DWORD  playerid;
  LPVOID data;
  DWORD  databytes;
} SNETEVENT, *SNETEVENTPTR;

typedef BOOL(CALLBACK* SNETENUMDEVICESPROC)(DWORD, LPCSTR, LPCSTR);
typedef BOOL(CALLBACK* SNETENUMGAMESEXPROC)(SNETGAMEPTR);
typedef BOOL(CALLBACK* SNETENUMGAMESPROC)(DWORD, LPCSTR, LPCSTR);
typedef BOOL(CALLBACK* SNETENUMPROVIDERSPROC)(DWORD, LPCSTR, LPCSTR, SNETCAPSPTR);
typedef void (CALLBACK* SNETEVENTPROC)(SNETEVENTPTR);

// ################################################################################################
// FUNCTIONS
// ################################################################################################


BOOL STORMAPI SNetCreateGame(LPCSTR gamename, LPCSTR gamepassword, LPCSTR gamedescription, DWORD gamecategorybits, LPVOID initdata, DWORD initdatabytes, DWORD maxplayers, LPCSTR playername, LPCSTR playerdescription, DWORD* playerid);
BOOL STORMAPI SNetDestroy();
BOOL STORMAPI SNetEnumProviders(int (STORMAPI* callback)(DWORD, DWORD, DWORD, DWORD), int mincaps); // ???? wrong?

BOOL STORMAPI SNetEnumGames(int (STORMAPI* callback)(DWORD, DWORD, DWORD), int* hintnextcall); // ???? wrong?


/// SNetDropPlayer @ 106
///
/// <summary>Drops a player from the current game.</summary>
///
/// <param name="playerid">The player ID for the player to be dropped.</param>
/// <param name="flags"></param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetDropPlayer(int playerid, DWORD flags);

/// SNetGetGameInfo @ 107
///
/// <summary>Retrieves specific game information from Storm, such as name, password,
/// stats, mode, game template, and players.</summary>
///
/// <param name="index">The type of data to retrieve. See GAMEINFO_ flags.</param>
/// <param name="dst">The destination buffer for the data.</param>
/// <param name="length">The maximum size of the destination buffer.</param>
/// <param name="byteswritten">The number of bytes written to the destination buffer.</param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetGetGameInfo(DWORD index, LPVOID buffer, DWORD buffersize, DWORD* byteswritten = NULL);

BOOL STORMAPI SNetGetNumPlayers(DWORD* firstplayerid, DWORD* lastplayerid, DWORD* activeplayers);

BOOL STORMAPI SNetGetPlayerCaps(DWORD playerid, SNETCAPSPTR caps);

/// SNetGetPlayerName @ 113
///
/// <summary>Retrieves the name of a player given their player ID.</summary>
///
/// <param name="playerid">The player's ID.</param>
/// <param name="buffer">The buffer that will receive the name.</param>
/// <param name="buffersize">The maximum size of buffer.</param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetGetPlayerName(DWORD playerid, LPSTR buffer, DWORD buffersize);

/// SNetGetProviderCaps @ 114
///
/// <summary>Retrieves network provider capacity information.</summary>
///
/// <param name="providerCaps">A pointer to a CAPS structure that will receive the information.</param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetGetProviderCaps(SNETCAPSPTR providerCaps);

/// SNetGetTurnsInTransit @ 115
///
/// <summary>Retrieves the number of turns (buffers) that have been queued
/// before sending them over the network.</summary>
///
/// <param name="turns">A pointer to an integer that will receive the value.</param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetGetTurnsInTransit(DWORD* turns);


BOOL STORMAPI SNetInitializeDevice(DWORD deviceId, SNETPROGRAMDATAPTR programData, SNETPLAYERDATAPTR playerData, SNETUIDATAPTR interfaceData, SNETVERSIONDATAPTR versionData);


/// SNetInitializeProvider @ 117
///
/// <summary>Initializes a provider by storing the provider callbacks, and calling
/// spiInitialize() using the parameters passed to this function.
/// Note: The use of the parameters is determined by the network module.</summary>
///
/// <param name="providerId">The provider's identifier. Example: 'TENB' (BNET).</param>
/// <param name="programData">A pointer to a clientInfo structure containing
///                   information about the game client.</param>
/// <param name="playerData">A pointer to a userInfo structure containing information
///                   about the player.</param>
/// <param name="interfaceData">A pointer to a battleInfo structure containing callbacks
///                   and other information that is specific to Battle.net.</param>
/// <param name="versionData">A pointer to a moduleInfo structure containing the
///                   executable information and paths to MPQ archives.</param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetInitializeProvider(DWORD providerId, SNETPROGRAMDATAPTR programData, SNETPLAYERDATAPTR playerData, SNETUIDATAPTR interfaceData, SNETVERSIONDATAPTR versionData);


BOOL STORMAPI SNetJoinGame(DWORD gameid, LPCSTR gameName, LPCSTR gamePassword, LPCSTR playerName, LPCSTR playerDescription, DWORD* playerid);

/// SNetLeaveGame @ 119
///
/// <summary>Notifies Storm that the player has left the game. Storm will
/// notify all connected peers through the network provider.</summary>
///
/// <param name="exitcode">The leave type.</param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetLeaveGame(DWORD exitcode);

BOOL STORMAPI SNetPerformUpgrade(DWORD* upgradestatus);
BOOL STORMAPI SNetReceiveMessage(DWORD* senderplayerid, LPVOID* data, DWORD* databytes);
BOOL STORMAPI SNetReceiveTurns(DWORD firstplayerid, DWORD arraysize, LPVOID* arraydata, LPDWORD arraydatabytes, LPDWORD arrayplayerstatus);

BOOL STORMAPI SNetRegisterEventHandler(DWORD eventid, SNETEVENTPROC callback);

BOOL STORMAPI SNetSelectGame(DWORD flags, SNETPROGRAMDATAPTR programdata, SNETPLAYERDATAPTR playerdata, SNETUIDATAPTR interfacedata, SNETVERSIONDATAPTR versiondata, DWORD* playerid);

/// SNetSendMessage @ 127
///
/// <summary>Sends a message to a player given their player ID. Network message
/// is sent using class 01 and is retrieved by the other client using
/// SNetReceiveMessage().</summary>
///
/// <param name="targetplayerid">The player index of the player to receive the data.
///             Conversely, this field can be one of the following constants:
///                 SNET_BROADCASTPLAYERID           | Sends the message to all players, including oneself.
///                 SNET_BROADCASTNONLOCALPLAYERID   | Sends the message to all players, except for oneself.</param>
/// <param name="data">A pointer to the data.</param>
/// <param name="databytes">The amount of bytes that the data pointer contains.</param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetSendMessage(DWORD targetplayerid, LPVOID data, DWORD databytes);

/// SNetSendTurn @ 128
///
/// <summary>Sends a turn (data packet) to all players in the game. Network data
/// is sent using class 02 and is retrieved by the other client using
/// SNetReceiveTurns().</summary>
///
/// <param name="data">A pointer to the data.</param>
/// <param name="databytes">The amount of bytes that the data pointer contains.</param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetSendTurn(LPVOID data, DWORD databytes);

/// SNetSetGameMode @ 130
///
/// <summary>Set's the game's mode flags, notifying the network
/// provider that the state of the game has changed.
/// For example: notifies Battle.net when the game is full.
///
/// You should first call SNetGetGameInfo to retrieve
/// the existing mode flags.</summary>
///
/// <param name="modeFlags">The new flags for the game mode.
///                 SNET_GM_PRIVATE     | The game is passworded.
///                 SNET_GM_FULL        | The game is full.
///                 SNET_GM_ADVERTISED  | The game is available.
///                 SNET_GM_UNJOINABLE  | The game is in progress.
///                 SNET_GM_REPLAY      | The game is a replay.</param>
/// <param name="makePublic">Used to make the game a public game, removing the GAMESTATE_PRIVATE flag.</param>
///
/// <returns>TRUE if the function was called successfully and FALSE otherwise.</returns>
BOOL STORMAPI SNetSetGameMode(DWORD modeFlags, bool makePublic = false);

#define SNMakeGamePublic() SNetSetGameMode( (DWORD mode, SNetGetGameInfo(GAMEINFO_MODEFLAGS, &mode, 4), mode), true)

BOOL STORMAPI SNetEnumGamesEx(DWORD categorybits, DWORD categorymask, int(__fastcall* callback)(DWORD, DWORD, DWORD), DWORD* hintnextcall);
BOOL STORMAPI SNetSendServerChatCommand(LPCSTR command);

BOOL STORMAPI SNetDisconnectAll(DWORD flags);
BOOL STORMAPI SNetCreateLadderGame(const char* pszGameName, const char* pszGamePassword, const char* pszGameStatString, DWORD dwGameType, DWORD dwGameLadderType, DWORD dwGameModeFlags, char* GameTemplateData, int GameTemplateSize, int playerCount, char* creatorName, char* a11, int* playerID);

#define SNET_GAME_RESULT_WIN        1
#define SNET_GAME_RESULT_LOSS       2
#define SNET_GAME_RESULT_DRAW       3
#define SNET_GAME_RESULT_DISCONNECT 4

BOOL STORMAPI SNetReportGameResult(unsigned a1, int size, int* results, const char* headerInfo, const char* detailInfo);

int  STORMAPI SNetSendLeagueCommand(char* cmd, char* callback);
int  STORMAPI SNetSendReplayPath(int a1, int a2, char* replayPath);
int  STORMAPI SNetGetLeagueName(int leagueID);
BOOL STORMAPI SNetGetPlayerNames(char** names);
int  STORMAPI SNetLeagueLogout(char* bnetName);
int  STORMAPI SNetGetLeaguePlayerName(char* curPlayerLeageName, size_t nameSize);

#define  SNETSPI_MAXCLIENTDATA    256
#define  SNETSPI_MAXSTRINGLENGTH  128

typedef struct _SNETADDR {
  BYTE address[16];
} SNETADDR, * SNETADDRPTR;

typedef struct _SNETSPI_DEVICELIST {
  DWORD                deviceid;
  SNETCAPS             devicecaps;
  char                 devicename[SNETSPI_MAXSTRINGLENGTH];
  char                 devicedescription[SNETSPI_MAXSTRINGLENGTH];
  DWORD                reserved;
  _SNETSPI_DEVICELIST* next;
} SNETSPI_DEVICELIST, * SNETSPI_DEVICELISTPTR;

typedef struct _SNETSPI_GAMELIST {
  DWORD              gameid;
  DWORD              gamemode;
  DWORD              creationtime;
  SNETADDR           owner;
  DWORD              ownerlatency;
  DWORD              ownerlasttime;
  DWORD              gamecategorybits;
  char               gamename[SNETSPI_MAXSTRINGLENGTH];
  char               gamedescription[SNETSPI_MAXSTRINGLENGTH];
  _SNETSPI_GAMELIST* next;
  LPVOID             clientdata;
  DWORD              clientdatabytes;
  DWORD              productid;
  DWORD              version;
} SNETSPI_GAMELIST, * SNETSPI_GAMELISTPTR;

typedef struct _SNETSPI {
  // The size of the vtable
  DWORD size;
  // Compares two sockaddrs with each other and returns the number of differences in dwResult
  BOOL(CALLBACK* spiCompareNetAddresses)(SNETADDRPTR addr1, SNETADDRPTR addr2, DWORD* diffmagnitude);
  // Called when the module is released
  BOOL(CALLBACK* spiDestroy)();
  // Called in order to free blocks of packet memory returned in the spiReceive functions
  BOOL(CALLBACK* spiFree)(SNETADDRPTR addr, LPVOID data, DWORD databytes);
  BOOL(CALLBACK* spiFreeExternalMessage)(LPCSTR addr, LPCSTR data, LPCSTR databytes); // TODO: possibly incorrect, investigate
  // Returns info on a specified game
  BOOL(CALLBACK* spiGetGameInfo)(DWORD gameid, LPCSTR gamename, LPCSTR gamepassword, SNETSPI_GAMELIST* gameinfo);
  // Returns packet statistics
  BOOL(CALLBACK* spiGetPerformanceData)(DWORD counterid, DWORD* countervalue, LARGE_INTEGER* measurementtime, LARGE_INTEGER* measurementfreq);
  // Called when the module is initialized
  BOOL(CALLBACK* spiInitialize)(SNETPROGRAMDATAPTR programdata, SNETPLAYERDATAPTR playerdata, SNETUIDATAPTR interfacedata, SNETVERSIONDATAPTR versiondata, HANDLE hEvent);
  BOOL(CALLBACK* spiInitializeDevice)(DWORD deviceid, SNETPROGRAMDATAPTR programdata, SNETPLAYERDATAPTR playerdata, SNETUIDATAPTR itnerfacedata, SNETVERSIONDATAPTR versiondata);
  BOOL(CALLBACK* spiLockDeviceList)(SNETSPI_DEVICELISTPTR* devicelist);
  // Called to prevent the game list from updating so that it can be processed by storm
  BOOL(CALLBACK* spiLockGameList)(DWORD categorybits, DWORD categorymask, SNETSPI_GAMELISTPTR* gamelist);
  // Return received data from a connectionless socket to storm
  BOOL(CALLBACK* spiReceive)(SNETADDRPTR* addr, LPVOID* data, DWORD* databytes);
  // Return received data from a connected socket to storm
  BOOL(CALLBACK* spiReceiveExternalMessage)(SNETADDRPTR* addr, LPVOID* data, DWORD* databytes); // TODO: possibly incorrect, investigate
  // Called when a game is selected to query information
  BOOL(CALLBACK* spiSelectGame)(DWORD flags, SNETPROGRAMDATAPTR programdata, SNETPLAYERDATAPTR playerdata, SNETUIDATAPTR interfacedata, SNETVERSIONDATAPTR versiondata, DWORD* playerid);
  // Sends data over a connectionless socket
  BOOL(CALLBACK* spiSend)(DWORD addresses, SNETADDRPTR* addrlist, LPVOID data, DWORD databytes);
  // Sends data over a connected socket
  BOOL(CALLBACK* spiSendExternalMessage)(LPCSTR senderpath, LPCSTR sendername, LPCSTR targetpath, LPCSTR targetname, LPCSTR message);
  // An extended version of spiStartAdvertisingGame
  // BOOL(CALLBACK* spiStartAdvertisingGame)(LPCSTR gamename, LPCSTR gamepassword, LPCSTR gamedescription, DWORD gamemode, DWORD gameage, DWORD gamecategorybits, DWORD optcategorybits, LPCVOID clientdata, DWORD clientdatabytes); <-- old
  BOOL(CALLBACK* spiStartAdvertisingGame)(LPCSTR gamename, LPCSTR gamepassword, LPCSTR gamedescription, DWORD gamemode, DWORD gameage, DWORD gamecategorybits, DWORD optcategorybits, DWORD, LPCVOID clientdata, DWORD clientdatabytes);
  // Called to stop advertising the game
  BOOL(CALLBACK* spiStopAdvertisingGame)();
  BOOL(CALLBACK* spiUnlockDeviceList)(SNETSPI_DEVICELISTPTR devicelist);
  // Called after the game list has been processed and resume updating
  BOOL(CALLBACK* spiUnlockGameList)(SNETSPI_GAMELISTPTR gamelist, DWORD* hintnextcall);
  BOOL(CALLBACK* spiGetLocalPlayerName)(LPCSTR namebuffer, DWORD namechars, LPCSTR descbuffer, DWORD descchars);
  void* spiReportGameResult;
  void* spiCheckDataFile;
  void* spiLeagueCommand;
  void* spiLeagueSendReplayPath;
  void* spiLeagueGetReplayPath;
  void* spiLeagueLogout;
  BOOL(CALLBACK* spiLeagueGetName)(LPSTR leaguebuffer, DWORD leaguechars);
} SNETSPI, * SNETSPIPTR;

typedef BOOL(APIENTRY* SNETSPIBIND)(DWORD, SNETSPIPTR*);
typedef BOOL(APIENTRY* SNETSPIQUERY)(DWORD, DWORD*, LPCSTR*, LPCSTR*, SNETCAPSPTR*);

#pragma endregion SNet

#pragma region SDlg

#define  SDLG_ADJUST_NONE               0
#define  SDLG_ADJUST_VERTICAL           1
#define  SDLG_ADJUST_CONTROLPOS         2

#define  SDLG_DBF_TILE                  0x00000001
#define  SDLG_DBF_VCENTER               0x00000002

#define  SDLG_STYLE_ANY                 0xFFFFFFFF
#define  SDLG_STYLE_ANYPUSHBUTTON       0x00010001

#define  SDLG_USAGE_BACKGROUND          0x00000001
#define  SDLG_USAGE_NORMAL_UNFOCUSED    0x00000010
#define  SDLG_USAGE_NORMAL_FOCUSED      0x00000020
#define  SDLG_USAGE_NORMAL              (SDLG_USAGE_NORMAL_UNFOCUSED | SDLG_USAGE_NORMAL_FOCUSED)
#define  SDLG_USAGE_SELECTED_UNFOCUSED  0x00000040
#define  SDLG_USAGE_SELECTED_FOCUSED    0x00000080
#define  SDLG_USAGE_SELECTED            (SDLG_USAGE_SELECTED_UNFOCUSED | SDLG_USAGE_SELECTED_FOCUSED)
#define  SDLG_USAGE_NORMAL_GRAYED       0x00000100
#define  SDLG_USAGE_SELECTED_GRAYED     0x00000400
#define  SDLG_USAGE_GRAYED              (SDLG_USAGE_NORMAL_GRAYED | SDLG_USAGE_SELECTED_GRAYED)
#define  SDLG_USAGE_CURSORMASK          0x00001000
#define  SDLG_USAGE_CURSORIMAGE         0x00002000

HDC  STORMAPI SDlgBeginPaint(HWND window, LPPAINTSTRUCT ps);
BOOL STORMAPI SDlgBltToWindowI(HWND window, HRGN region, int x, int y, LPBYTE bitmapbits, LPRECT bitmaprect, LPSIZE bitmapsize, DWORD colorkey = 0xFFFFFFFF, DWORD pattern = 0, DWORD rop3 = SRCCOPY);
BOOL STORMAPI SDlgCheckTimers();
HWND STORMAPI SDlgCreateDialogIndirectParam(HINSTANCE instance, LPCDLGTEMPLATE templatedata, HWND parentwindow, DLGPROC dialogproc, LPARAM initparam);
HWND STORMAPI SDlgCreateDialogParam(HINSTANCE instance, LPCTSTR templatename, HWND parentwindow, DLGPROC dialogproc, LPARAM initparam);
BOOL STORMAPI SDlgDefDialogProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

int  STORMAPI SDlgDialogBoxIndirectParam(HINSTANCE instance, LPCDLGTEMPLATE templatedata, HWND parentwindow, DLGPROC dialogproc, LPARAM initparam);
int  STORMAPI SDlgDialogBoxParam(HINSTANCE instance, LPCTSTR templatename, HWND parentwindow, DLGPROC dialogproc, LPARAM initparam);
BOOL STORMAPI SDlgDrawBitmap(HWND window, DWORD usage, HRGN region, int offsetx = 0, int offsety = 0, LPRECT boundingoffset = NULL, DWORD flags = 0);
BOOL STORMAPI SDlgEndDialog(HWND window, int result);
BOOL STORMAPI SDlgEndPaint(HWND window, LPPAINTSTRUCT ps);
BOOL STORMAPI SDlgKillTimer(HWND window, UINT event);
BOOL STORMAPI SDlgSetBaseFont(int pointsize, int weight, DWORD flags, DWORD family, LPCTSTR face);
BOOL STORMAPI SDlgSetBitmapI(HWND window, HWND parentwindow, LPCTSTR controltype, DWORD controlstyle, DWORD usage, LPBYTE bitmapbits, LPRECT rect, int width, int height, COLORREF colorkey = 0xFFFFFFFF);
BOOL STORMAPI SDlgSetControlBitmaps(HWND parentwindow, LPINT controllist, LPDWORD usagelist, LPBYTE bitmapbits, LPSIZE bitmapsize, DWORD adjusttype, COLORREF colorkey = 0xFFFFFFFF);
BOOL STORMAPI SDlgSetCursor(HWND window, HCURSOR cursor, DWORD id, HCURSOR * oldcursor);
BOOL STORMAPI SDlgSetSystemCursor(LPBYTE maskbitmap, LPBYTE imagebitmap, LPSIZE size, DWORD id = (DWORD)IDC_ARROW);
BOOL STORMAPI SDlgSetTimer(HWND window, UINT event, UINT elapse, TIMERPROC timerfunc);
BOOL STORMAPI SDlgUpdateCursor();
BOOL STORMAPI SDlgBltToWindowE(HWND window, HRGN region, int x, int y, LPBYTE bitmapbits, LPRECT bitmaprect, LPSIZE bitmapsize, DWORD colorkey = 0xFFFFFFFF, DWORD pattern = 0, DWORD rop3 = SRCCOPY);
BOOL STORMAPI SDlgSetBitmapE(HWND window, HWND parentwindow, LPCTSTR controltype, DWORD controlstyle, DWORD usage, LPBYTE bitmapbits, LPRECT rect, int width, int height, COLORREF colorkey = 0xFFFFFFFF);

int  STORMAPI Ordinal224(int a1);

#pragma endregion SDlg

#pragma region SFile

typedef BOOL(CALLBACK* SFILEERRORPROC)(LPCTSTR, DWORD);

#define  SFILE_AUTH_UNABLETOAUTHENTICATE   0
#define  SFILE_AUTH_NOSIGNATURE            1
#define  SFILE_AUTH_BADSIGNATURE           2
#define  SFILE_AUTH_UNKNOWNSIGNATURE       3
#define  SFILE_AUTH_FIRSTAUTHENTIC         5
#define  SFILE_AUTH_AUTHENTICBLIZZARD      5

#define  SFILE_DDA_LOOP                    0x00040000

#define  SFILE_ERRORMODE_RETURNCODE        0
#define  SFILE_ERRORMODE_CUSTOM            1
#define  SFILE_ERRORMODE_FATAL             2

#define  SFILE_FIND_FILES                  1
#define  SFILE_FIND_DIRECTORIES            2

BOOL STORMAPI SFileAuthenticateArchive(HANDLE archive, DWORD *extendedresult);
BOOL STORMAPI SFileCloseArchive(HANDLE handle);
BOOL STORMAPI SFileCloseFile(HANDLE handle);
BOOL STORMAPI SFileDdaBegin(HANDLE handle, DWORD buffersize, DWORD flags);
BOOL STORMAPI SFileDdaBeginEx(HANDLE handle, DWORD buffersize, DWORD flags, DWORD offset, LONG volume, LONG pan, LPVOID reserved);
BOOL STORMAPI SFileDdaDestroy();
BOOL STORMAPI SFileDdaEnd(HANDLE directsound);
BOOL STORMAPI SFileDdaGetPos(HANDLE handle, DWORD* position, DWORD* maxposition);
BOOL STORMAPI SFileDdaGetVolume(HANDLE handle, DWORD *volume, DWORD *pan);

BOOL STORMAPI SFileDdaInitialize(HANDLE directsound);
BOOL STORMAPI SFileDdaSetVolume(HANDLE handle, LONG volume, LONG pan);
BOOL STORMAPI SFileDestroy();
BOOL STORMAPI SFileEnableDirectAccess(BOOL enable);
BOOL STORMAPI SFileGetArchiveInfo(HANDLE archive, int* priority, BOOL* cdrom);
BOOL STORMAPI SFileGetBasePath(LPTSTR buffer, DWORD bufferchars);
BOOL STORMAPI SFileGetFileArchive(HANDLE file, HANDLE archive);
LONG STORMAPI SFileGetFileSize(HANDLE handle, LPDWORD filesizehigh = NULL);
BOOL STORMAPI SFileOpenArchive(LPCTSTR archivename, int priority, BOOL cdonly, HANDLE *handle);

// values for dwFlags
enum MPQFlags
{
  MPQ_NO_LISTFILE       = 0x0010,
  MPQ_NO_ATTRIBUTES     = 0x0020,
  MPQ_FORCE_V1          = 0x0040,
  MPQ_CHECK_SECTOR_CRC  = 0x0080
};


BOOL STORMAPI SFileOpenFile(LPCTSTR filename, HANDLE *handle);
BOOL STORMAPI SFileOpenFileEx(HANDLE archivehandle, LPCTSTR filename, DWORD flags, HANDLE *handle);

// values for flags to SFileOpenFileEx
enum SFileFlags
{
  SFILE_FROM_MPQ      = 0x00000000,
  SFILE_FROM_ABSOLUTE = 0x00000001,
  SFILE_FROM_RELATIVE = 0x00000002,
  SFILE_FROM_DISK     = 0x00000004
};

BOOL STORMAPI SFileReadFile(HANDLE handle, LPVOID buffer, DWORD bytestoread, LPDWORD bytesread = NULL, LPOVERLAPPED overlapped = NULL);
BOOL STORMAPI SFileSetBasePath(LPCTSTR path);
BOOL STORMAPI SFileSetFilePointer(HANDLE handle, LONG distancetomove, PLONG distancetomoveHigh, DWORD movemethod);

void STORMAPI SFileSetLocale(LCID lcid);

// mode:    0 - Silent (callback is NULL)
//          1 - Application Defined
//          2 - Handled by storm (callback is NULL)
// BOOL STORMAPI callback(const char *pszFilename, DWORD dwErrCode, DWORD dwErrCount)
BOOL STORMAPI SFileSetIoErrorMode(DWORD errormode, SFILEERRORPROC errorproc = NULL);

BOOL STORMAPI SFileGetArchiveName(HANDLE archive, LPTSTR buffer, DWORD bufferchars);
BOOL STORMAPI SFileGetFileName(HANDLE file, LPTSTR buffer, DWORD bufferchars);

BOOL STORMAPI SFileLoadFile(char *filename, void *buffer, int buffersize, int a4, int a5);
BOOL STORMAPI SFileUnloadFile(HANDLE hFile);
BOOL STORMAPI SFileLoadFileEx(void *hArchive, char *filename, int a3, int a4, int a5, DWORD searchScope, struct _OVERLAPPED *lpOverlapped);

#pragma endregion SFile

#pragma region SBlt

// Options are DWORD except for #6
// 1: [TRUE|FALSE] - If true, reports resource leaks (SErrReportResourceLeak/SErrReportNamedResourceLeak) to the attached debugger instead of a message box.
// 2: This option is unused.
// 3: [TRUE|FALSE] - If true, reports general memory leaks to the attached debugger instead of a message box.
// 4: This option is unused.
// 5: [TRUE|FALSE] - If true, reports log messages and log dumps to the attached debugger.
// 6: { DWORD blocks_allocated; DWORD blocks_freed; } Used to determine the amount of memory/heap blocks that have been allocated and freed by storm.
//    Can also be used for custom allocations outside of storm.
//
//BOOL STORMAPI StormGetOption(int type, void *pValue, size_t *pSize);
//BOOL STORMAPI StormSetOption(int type, void *pValue, size_t size);

BOOL STORMAPI SBltGetSCode(DWORD rop3, LPSTR buffer, DWORD buffersize, BOOL optimize);
BOOL STORMAPI SBltROP3(LPBYTE dest, LPBYTE source, int width, int height, int destcx, int sourcecx, DWORD pattern, DWORD rop3);
BOOL STORMAPI SBltROP3Clipped(LPBYTE dest, LPRECT destrect, LPSIZE destsize, int destpitch, LPBYTE source, LPRECT sourcerect, LPSIZE sourcesize, int sourcepitch, DWORD pattern, DWORD rop3);
BOOL STORMAPI SBltROP3Tiled(LPBYTE dest, LPRECT destrect, int destpitch, LPBYTE source, LPRECT sourcerect, int sourcepitch, int sourceoffsetx, int sourceoffsety, DWORD pattern, DWORD rop3);

#pragma endregion SBlt

#pragma region SBmp

#define SBMP_IMAGETYPE_AUTO  0
#define SBMP_IMAGETYPE_BMP   1
#define SBMP_IMAGETYPE_PCX   2
#define SBMP_IMAGETYPE_TGA   3

typedef LPVOID(STORMAPI* SBMPALLOCPROC)(DWORD);

/*  SBmpDecodeImage @ 321
 * 
 *  Decodes an image that has already been loaded into a buffer.
 *  
 *  imagetype:        Optional, the image type. See SBMP_ macros.
 *  imagedata:        A pointer to the source buffer.
 *  imagebytes:       The size of the data in the source buffer.
 *  paletteentries:   An optional buffer that receives the image palette.
 *  bitmapbits:       A buffer that receives the image data.
 *  buffersize:       The size of the specified image buffer. If the size of the 
 *                    destination buffer is 0, then the destination buffer is not used.
 *  width:            An optional variable that receives the image width.
 *  height:           An optional variable that receives the image height.
 *  bitdepth:         An optional variable that receives the image bits per pixel.
 *
 *  <returns>TRUE if the image was supported and decoded correctly, FALSE otherwise.</returns>
 */
BOOL 
STORMAPI 
SBmpDecodeImage(
    DWORD          imagetype,
    LPBYTE         imagedata,
    DWORD          imagebytes,
    LPPALETTEENTRY paletteentries = NULL,
    LPBYTE         bitmapbits     = NULL,
    DWORD          buffersize     = 0,
    int            *width         = NULL,
    int            *height        = NULL,
    int            *bitdepth      = NULL);


/*  SBmpLoadImage @ 323
 * 
 *  Load an image from an available archive into a buffer.
 *  
 *  filename:       The name of the graphic in an active archive.
 *  paletteentries: An optional buffer that receives the image palette.
 *  bitmapbits:     A buffer that receives the image data.
 *  buffersize:     The size of the specified image buffer.
 *  width:          An optional variable that receives the image width.
 *  height:         An optional variable that receives the image height.
 *  bitdepth:       An optional variable that receives the image bits per pixel.
 *
 *  <returns>TRUE if the image was supported and loaded correctly, FALSE otherwise.</returns>
 */
BOOL
STORMAPI
SBmpLoadImage(
  LPCTSTR         filename,
  LPPALETTEENTRY  paletteentries = NULL,
  LPBYTE          bitmapbits = NULL,
  DWORD           buffersize = 0,
  int             *width = NULL,
  int             *height = NULL,
  int             *bitdepth = NULL);

/*  SBmpSaveImage @ 324
 * 
 *  Save an image from a buffer to a file. The image format is determined 
 *  from the filename and is either .gif, .pcx, .tga, or .bmp being the default.
 *  
 *  filename:       The name of the file to create.
 *  paletteentries: A pointer to a palette array containing 256 entries.
 *  bitmapbits:     A buffer containing the image data.
 *  width:          The width of the image.
 *  height:         The height of the image.
 *  bitdepth:       The bits per pixel.
 *
 *  <returns>TRUE if the image was saved correctly, FALSE otherwise.</returns>
 */
BOOL
STORMAPI
SBmpSaveImage(
  LPCTSTR         filename,
  LPPALETTEENTRY  paletteentries,
  LPBYTE          bitmapbits,
  int             width,
  int             height,
  int             bitdepth = 8);

BOOL STORMAPI SBmpAllocLoadImage(LPCTSTR filename, LPPALETTEENTRY  paletteentries, LPBYTE* returnedbuffer, int* width = NULL, int* height = NULL, int* bitdepth = NULL, int requestedbitdepth = 0, SBMPALLOCPROC allocproc = NULL);

#pragma endregion SBmp


#pragma region SCode

#define  SCODE_CF_AUTOALIGNDWORD  0x00040000
#define  SCODE_CF_USESALTADJUSTS  0x04000000

typedef struct _SCODEEXECUTEDATA {
  DWORD  size;
  DWORD  flags;
  int    xiterations;
  int    yiterations;
  int    adjustdest;
  int    adjustsource;
  LPVOID dest;
  LPVOID source;
  LPVOID table;
  DWORD  a;
  DWORD  b;
  DWORD  c;
  int    adjustdestalt;
  int    adjustsourcealt;
  DWORD  reserved[2];
} SCODEEXECUTEDATA, * SCODEEXECUTEDATAPTR;

BOOL STORMAPI SCodeCompile(LPCSTR prologstring, LPCSTR loopstring, LPCSTR* firsterror, DWORD maxiterations, DWORD flags, HANDLE* handle);
BOOL STORMAPI SCodeDelete(HANDLE handle);

BOOL STORMAPI SCodeExecute(HANDLE handle, SCODEEXECUTEDATAPTR executedata);
BOOL STORMAPI SCodeGetPseudocode(LPCSTR scodestring, LPSTR buffer, DWORD buffersize);
//BOOL STORMAPI SCodeGetJumpTable(HSCODESTREAM handle, LPBYTE** jumptableptr, LPDWORD* prologpatchlocation, LPDWORD* looppatchlocation, LPDWORD* epilogpatchlocation);

#pragma endregion SCode

#pragma region SDraw

#define  SDRAW_SERVICE_BASIC         1
#define  SDRAW_SERVICE_PAGEFLIP      2
#define  SDRAW_SERVICE_DOUBLEBUFFER  3
#define  SDRAW_SERVICE_MAX           3

#define  SDRAW_SURFACE_FRONT         0
#define  SDRAW_SURFACE_BACK          1
#define  SDRAW_SURFACE_SYSTEM        2
#define  SDRAW_SURFACE_TEMPORARY     3

BOOL STORMAPI SDrawAutoInitialize(HINSTANCE instance, LPCSTR classname, LPCSTR title, WNDPROC wndproc, int servicelevel = SDRAW_SERVICE_BASIC, int width = 640, int height = 480, int bitdepth = 8);

/*  SDrawCaptureScreen @ 342
 * 
 *  Saves a screenshot from the primary surface being handled by Storm.
 *  
 *  filename: The name of the output file. The save format is automatically set by the extension.
 *            The extensions supported are .gif, .pcx, .tga, and .bmp. It will write a bitmap by default.
 *
 *  <returns>TRUE if successful and FALSE otherwise.</returns>
 */
BOOL STORMAPI SDrawCaptureScreen(LPCTSTR filename);


BOOL STORMAPI SDrawClearSurface(int surfacenumber);
BOOL STORMAPI SDrawDestroy();
BOOL STORMAPI SDrawFlipPage();


/*  SDrawGetFrameWindow @ 346
 *
 *  Retrieves the window handle that was specified in
 *  SDrawManualInitialize or created in SDrawAutoInitialize.
 *  
 *  window: Optional variable that receives the returned handle.
 *
 *  <returns>the handle of the window.</returns>
 */
HWND STORMAPI SDrawGetFrameWindow(HWND *window = NULL);


/*  SDrawGetObjects @ 347
 *  
 *  Retrieves the object information that was initialized using
 *  SDrawManualInitialize or SDrawAutoInitialize.
 *  
 *  directdraw:       The DirectDraw interface.
 *  frontbuffer:      The primary DirectDraw surface.
 *  backbuffer:       A second unknown surface.
 *  systembuffer:     A third unknown surface.
 *  temporarybuffer:  The back DirectDraw surface.
 *  palette:          The DirectDraw palette.
 *  gdipalette:       The GDI palette handle.
 *
 *  <returns>FALSE if the direct draw interface has not been initialized.</returns>
 */
BOOL
STORMAPI
SDrawGetObjects(
    LPDIRECTDRAW        *directdraw = NULL,
    LPDIRECTDRAWSURFACE *frontbuffer = NULL,
    LPDIRECTDRAWSURFACE *backbuffer = NULL,
    LPDIRECTDRAWSURFACE *systembuffer = NULL,
    LPDIRECTDRAWSURFACE *temporarybuffer = NULL,
    LPDIRECTDRAWPALETTE *palette = NULL,
    HPALETTE            *gdipalette = NULL);


/*  SDrawGetScreenSize @ 348
 *  
 *  Obtains information for the current screen resolution.
 *
 *  width:    Optional variable that receives the screen width.
 *  height:   Optional variable that receives the screen height.
 *  bitdepth: Optional variable that receives the bits per pixel.
 *
 *  <returns>FALSE if no variables were specified.</returns>
 */
BOOL
STORMAPI
SDrawGetScreenSize(
    int *width,
    int *height,
    int *bitdepth = NULL);


BOOL STORMAPI SDrawLockSurface(int surfacenumber, LPRECT rect, LPBYTE *ptr, int *pitch, DWORD flags = 0);


/*  SDrawManualInitialize @ 351
 *  
 *  Sets the DirectDraw variables to be referenced in Storm.
 *
 *  framewindow:      The handle of the DirectDraw window.
 *  directdraw:       The DirectDraw interface.
 *  frontbuffer:      The first and primary surface.
 *  backbuffer:       A second surface. Behaviour not completely known.
 *  systembuffer:     A third surface. Behaviour not completely known.
 *  temporarybuffer:  The fourth and final surface. The back surface.
 *  palette:          The DirectDraw palette if the application requires it.
 *  gdipalette:       The palette handle that belongs to the window.
 *                    If this is NULL and ddPalette is specified, then it
 *                    will be created automatically. A palette can be created
 *                    using the CreatePalette WinAPI function.
 *
 *  <returns>FALSE if no variables were specified.</returns>
 */
BOOL
STORMAPI
SDrawManualInitialize(
     HWND framewindow = NULL,
     LPDIRECTDRAW directdraw = NULL,
     LPDIRECTDRAWSURFACE frontbuffer = NULL,
     LPDIRECTDRAWSURFACE backbuffer = NULL,
     LPDIRECTDRAWSURFACE systembuffer = NULL,
     LPDIRECTDRAWSURFACE temporarybuffer = NULL,
     LPDIRECTDRAWPALETTE palette = NULL,
     HPALETTE gdipalette = NULL);


int APIENTRY SDrawMessageBox(LPCTSTR text, LPCTSTR title, UINT flags);


/*  SDrawPostClose @ 353
 *
 *  Posts a WM_QUIT message to the active drawing window specified 
 *  in SDrawManualInitialize or created in SDrawAutoInitialize.
 *
 *  <returns>TRUE if successful and FALSE otherwise.</returns>
 */
BOOL STORMAPI SDrawPostClose();


BOOL STORMAPI SDrawRealizePalette();

BOOL STORMAPI SDrawSelectGdiSurface(BOOL select, BOOL copy);

BOOL STORMAPI SDrawUnlockSurface(int surfacenumber, LPBYTE ptr, DWORD numrects = 0, LPCRECT rectarray = NULL);
BOOL STORMAPI SDrawUpdatePalette(DWORD firstentry, DWORD numentries, LPPALETTEENTRY entries, BOOL reservedentries = FALSE);
BOOL STORMAPI SDrawUpdateScreen(LPCRECT rect);

#pragma endregion SDraw

#pragma region SEvt

typedef void (CALLBACK* SEVTHANDLER)(LPVOID);

BOOL STORMAPI SEvtDispatch(DWORD type, DWORD subtype, DWORD id, LPVOID data);
BOOL STORMAPI SEvtRegisterHandler(DWORD type, DWORD subtype, DWORD id, DWORD flags, SEVTHANDLER handler);
BOOL STORMAPI SEvtUnregisterHandler(DWORD type, DWORD subtype, DWORD id, DWORD flags, SEVTHANDLER handler);
BOOL STORMAPI SEvtUnregisterType(DWORD type, DWORD subtype);
BOOL STORMAPI SEvtPopState(DWORD type, DWORD subtype);
BOOL STORMAPI SEvtPushState(DWORD type, DWORD subtype);
BOOL STORMAPI SEvtBreakHandlerChain(LPVOID data);

#pragma endregion SEvt

#pragma region SGdi

#define  ETO_TEXT_TRANSPARENT  0
#define  ETO_TEXT_COLOR        1
#define  ETO_TEXT_BLACK        2
#define  ETO_TEXT_WHITE        3
#define  ETO_BKG_TRANSPARENT   0
#define  ETO_BKG_COLOR         1
#define  ETO_BKG_BLACK         2
#define  ETO_BKG_WHITE         3

BOOL STORMAPI SGdiBitBlt(LPBYTE videobuffer, int destx, int desty, LPBYTE sourcedata, LPRECT sourcerect, int sourcecx, int sourcecy, COLORREF color = 0, DWORD rop = SRCCOPY);
BOOL STORMAPI SGdiCreateFont(LPBYTE bits, int width, int height, int bitdepth, int filecharwidth, int filecharheight, LPSIZE charsizetable, HANDLE* handle);
BOOL STORMAPI SGdiDeleteObject(HANDLE handle);

BOOL STORMAPI SGdiExtTextOut(LPBYTE videobuffer, int x, int y, LPRECT rect, COLORREF color, int textcoloruse, int bkgcoloruse, LPCTSTR string, int chars = -1);
BOOL STORMAPI SGdiImportFont(HFONT windowsfont, HANDLE *handle);
BOOL STORMAPI SGdiLoadFont(LPCTSTR filename, int filecharwidth, int filecharheight, int basecharwidth, LPSIZE charsizetable, HANDLE* handle);
BOOL STORMAPI SGdiRectangle(LPBYTE videobuffer, int left, int top, int right, int bottom, COLORREF color);
BOOL STORMAPI SGdiSelectObject(HANDLE handle);
BOOL STORMAPI SGdiSetPitch(int pitch);
BOOL STORMAPI SGdiTextOut(LPBYTE videobuffer, int x, int y, COLORREF color, LPCTSTR string, int chars = -1);
BOOL STORMAPI SGdiSetTargetDimensions(int width, int height, int bitdepth, int pitch);
BOOL STORMAPI SGdiGetTextExtent(LPCTSTR string, int chars, LPSIZE size);

#pragma endregion SGdi

#pragma region SMem

#define  SMEM_FLAG_ZEROMEMORY         0x00000008
#define  SMEM_FLAG_UNKNOWN__          0x04000000    // a valid flag but purpose is not yet known
#define  SMEM_FLAG_PRESERVEONDESTROY  0x08000000

/*  SMemAlloc @ 401
 *  
 *  Allocates a block of memory. This block is different
 *  from the standard malloc by including a header containing
 *  information about the block. 
 *
 *  amount:       The amount of memory to allocate, in bytes.
 *  logfilename:  The name of the file or object that this call belongs to.
 *  logline:      The line in the file or one of the SLOG_ macros.
 *  defaultValue: The default value of a byte in the allocated memory.
 *
 *  <returns>a pointer to the allocated memory. This pointer does NOT include
 *  the additional storm header.</returns>
 */
LPVOID STORMAPI SMemAlloc(DWORD bytes, LPCSTR filename = NULL, int linenumber = 0, DWORD flags = 0);

#define SMAlloc(amount) SMemAlloc((amount), __FILE__, __LINE__)


/*  SMemFree @ 403
 *  
 *  Frees a block of memory that was created using SMemAlloc, 
 *  includes the log file and line for debugging purposes.
 *
 *  location:     The memory location to be freed.
 *  logfilename:  The name of the file or object that this call belongs to.
 *  logline:      The line in the file or one of the SLOG_ macros.
 *  defaultValue: 
 *
 *  <returns>TRUE if the call was successful and FALSE otherwise.</returns>
 */
BOOL
STORMAPI
SMemFree(
    void *location,
    const char *logfilename,
    int  logline,
    char defaultValue = 0);

#define SMFree(loc) SMemFree((loc), __FILE__, __LINE__)


/*  SMemReAlloc @ 405
 *  
 *  Reallocates a block of memory that was created using SMemAlloc, 
 *  includes the log file and line for debugging purposes.
 *
 *  location:     The memory location to be re-allocated. If this parameter
 *                is NULL, then SMemAlloc is called with the remaining parameters.
 *  amount:       The amount of memory to re-allocate.
 *  logfilename:  The name of the file or object that this call belongs to.
 *  logline:      The line in the file or one of the SLOG_ macros.
 *  defaultValue: 
 *
 *  <returns>a pointer to the re-allocated memory. This pointer does NOT include
 *  the additional storm header.</returns>
 */
void*
STORMAPI
SMemReAlloc(
    void    *location,
    size_t  amount,
    const char    *logfilename,
    int     logline,
    char    defaultValue = 0);

#define SMReAlloc(loc,s) SMemReAlloc((loc),(s), __FILE__, __LINE__)

// Can be provided instead of logline/__LINE__ parameter to indicate different errors.
#define SLOG_EXPRESSION    0
#define SLOG_FUNCTION     -1
#define SLOG_OBJECT       -2
#define SLOG_HANDLE       -3
#define SLOG_FILE         -4
#define SLOG_EXCEPTION    -5

#pragma endregion SMem

#pragma region SReg

BOOL STORMAPI SRegLoadData(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, LPVOID buffer, DWORD buffersize, DWORD *bytesread);
BOOL STORMAPI SRegLoadString(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, LPTSTR buffer, DWORD bufferchars);
BOOL STORMAPI SRegLoadValue(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, DWORD *value);
BOOL STORMAPI SRegSaveData(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, LPVOID data, DWORD databytes);
BOOL STORMAPI SRegSaveString(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, LPCTSTR string);
BOOL STORMAPI SRegSaveValue(LPCTSTR keyname, LPCTSTR valuename, DWORD flags, DWORD value);
BOOL STORMAPI SRegGetBaseKey(DWORD flags, LPSTR buffer, DWORD buffersize);
BOOL STORMAPI SRegDeleteValue(LPCTSTR keyname, LPCTSTR valuename, DWORD flags);

// Flags for SReg functions

// Default behaviour checks both HKEY_LOCAL_MACHINE and HKEY_CURRENT_USER
// relative to the "Software\\Blizzard Entertainment\\" key in both hives.
#define SREG_FLAG_NONE              0x00000000
#define SREG_FLAG_USERSPECIFIC      0x00000001  // excludes checking the HKEY_LOCAL_MACHINE hive
#define SREG_FLAG_BATTLENET         0x00000002  // sets the relative key to "Software\\Battle.net\\" instead
#define SREG_FLAG_ALLUSERS          0x00000004  // excludes checking the HKEY_CURRENT_USER hive
#define SREG_FLAG_FLUSHTODISK       0x00000008
#define SREG_FLAG_ABSOLUTE          0x00000010  // specifies that the key is not a relative key
#define SREG_FLAG_MULTISZ           0x00000080

#pragma endregion SReg

#pragma region STrans

BOOL STORMAPI STransBlt(LPBYTE dest, int destx, int desty, int destpitch, HANDLE transparency);
BOOL STORMAPI STransBltUsingMask(LPBYTE dest, LPBYTE source, int destpitch, int sourcepitch, HANDLE mask);
BOOL STORMAPI STransCreateI(LPBYTE bits, int width, int height, int bitdepth, LPRECT rect, COLORREF colorkey, HANDLE* handle);
BOOL STORMAPI STransDelete(HANDLE handle);

BOOL STORMAPI STransDuplicate(HANDLE source, HANDLE *handle);
BOOL STORMAPI STransIntersectDirtyArray(HANDLE sourcemask, LPBYTE dirtyarray, BYTE dirtyarraymask, HANDLE *handle);
BOOL STORMAPI STransInvertMask(HANDLE sourcemask, HANDLE *handle);
BOOL STORMAPI STransLoadI(LPCTSTR filename, LPRECT rect, COLORREF colorkey, HANDLE *handle);
BOOL STORMAPI STransSetDirtyArrayInfo(int screencx, int screency, int cellcx, int cellcy);
BOOL STORMAPI STransUpdateDirtyArray(LPBYTE dirtyarray, BYTE dirtyvalue, int destx, int desty, HANDLE transparency, BOOL tracecontour);
BOOL STORMAPI STransIsPixelInMask(HANDLE mask, int offsetx, int offsety);
BOOL STORMAPI STransCombineMasks(HANDLE basemask, HANDLE secondmask, int offsetx, int offsety, DWORD flags, HANDLE* handle);
BOOL STORMAPI STransCreateMaskI(LPBYTE bits, int width, int height, int bitdepth, LPRECT rect, COLORREF colorkey, HANDLE* handle);
BOOL STORMAPI STransCreateE(LPBYTE bits, int width, int height, int bitdepth, LPRECT rect, COLORREF colorkey, HANDLE* handle);
BOOL STORMAPI STransCreateMaskE(LPBYTE bits, int width, int height, int bitdepth, LPRECT rect, COLORREF colorkey, HANDLE* handle);
BOOL STORMAPI STransLoadE(LPCTSTR filename, LPRECT rect, COLORREF colorkey, HANDLE* handle);

#pragma endregion STrans

#pragma region SVid


#define  SVID_FLAG_DOUBLESCANS            0x00000001
#define  SVID_FLAG_INTERPOLATE            0x00000002
#define  SVID_FLAG_INTERLACE              0x00000004
#define  SVID_FLAG_AUTOQUALITY            0x00000008
#define  SVID_FLAG_1XSIZE                 0x00000100
#define  SVID_FLAG_2XSIZE                 0x00000200
#define  SVID_FLAG_AUTOSIZE               0x00000800
#define  SVID_FLAG_FILEHANDLE             0x00010000
#define  SVID_FLAG_PRELOAD                0x00020000
#define  SVID_FLAG_LOOP                   0x00040000
#define  SVID_FLAG_FULLSCREEN             0x00080000
#define  SVID_FLAG_USECURRENTPALETTE      0x00100000
#define  SVID_FLAG_CLEARSCREEN            0x00200000
#define  SVID_FLAG_NOSKIP                 0x00400000
#define  SVID_FLAG_NEEDPAN                0x02000000
#define  SVID_FLAG_NEEDVOLUME             0x04000000
#define  SVID_FLAG_TOSCREEN               0x10000000
#define  SVID_FLAG_TOBUFFER               0x20000000

typedef struct _SVIDPALETTEUSE {
  DWORD size;
  DWORD firstentry;
  DWORD numentries;
} SVIDPALETTEUSE, * SVIDPALETTEUSEPTR;

BOOL STORMAPI SVidDestroy();
BOOL STORMAPI SVidGetSize(HANDLE video, int *width, int *height, int *bitdepth = NULL);
BOOL STORMAPI SVidInitialize(HANDLE directsound);
BOOL STORMAPI SVidPlayBegin(LPCTSTR filename, LPVOID destbuffer, LPCRECT destrect, LPSIZE destsize, SVIDPALETTEUSEPTR paletteuse, DWORD flags, HANDLE *handle);
BOOL STORMAPI SVidPlayBeginFromMemory(LPVOID sourceptr, DWORD sourcebytes, LPVOID destbuffer, LPCRECT destrect, LPSIZE destsize, SVIDPALETTEUSEPTR paletteuse, DWORD flags, HANDLE *handle);
BOOL STORMAPI SVidPlayContinue();
BOOL STORMAPI SVidPlayContinueSingle(HANDLE video, BOOL forceupdate, BOOL *updated);
BOOL STORMAPI SVidPlayEnd(HANDLE video);
BOOL STORMAPI SVidSetVolume(HANDLE video, LONG volume, LONG pan, DWORD track = 0);

#pragma endregion SVid

#pragma region SErr


#define  SERR_LINECODE_FUNCTION  -1
#define  SERR_LINECODE_OBJECT    -2
#define  SERR_LINECODE_HANDLE    -3
#define  SERR_LINECODE_FILE      -4

/* SErrDisplayError @ 461
 *
 * Displays a formatted error message. The message is detailed and flexible for many applications.
 * The message will be different if there is a debugger attached. Will typically terminate the application
 * unless the option to continue is given.
 *
 *  errorcode:   The error code. See SErrGetLastError and GetLastError.
 *  filename:    The name of the file or object that this call belongs to.
 *  linenumber:  The line in the file or one of the SLOG_ macros.
 *  description: A message or expression with additional information.
 *  recoverable: If TRUE, allows the user the option to continue execution, otherwise the program will terminate.
 *  exitcode:    The exit code used for program termination.
 *
 *  <returns>TRUE if the user chose to continue execution, FALSE otherwise.</returns>
 */
BOOL
STORMAPI
SErrDisplayError(
  DWORD errorcode,
  LPCTSTR filename,
  int linenumber,
  LPCTSTR description = NULL,
  BOOL recoverable = FALSE,
  int exitcode = 1);

#define SAssert(x) { if ( !(x) ) SErrDisplayError(STORM_ERROR_ASSERTION, __FILE__, __LINE__, #x) }

#define SEDisplayError(err) SErrDisplayError(e, __FILE__, __LINE__)

/*  SErrGetErrorStr @ 462
 *  
 *  Retrieves a string that describes the specified error code for
 *  the system, Storm, DirectDraw, or DirectSound.
 *
 *  errorcode:    The error code to look up.
 *  buffer:       The destination buffer to receive the string.
 *  bufferchars:  The size of the destination buffer.
 *
 *  <returns>TRUE if the call was successful and FALSE otherwise.</returns>
 */
BOOL STORMAPI SErrGetErrorStr(DWORD errorcode, LPTSTR buffer, DWORD bufferchars);


/*  SErrGetLastError @ 463
 *  
 *  Retrieves the last error that was specifically
 *  set for the Storm library.
 *
 *  <returns>the last error set within the Storm library.</returns>
 */
DWORD STORMAPI SErrGetLastError();


// Registers a module as a message source for SErrGetErrorStr, always returns TRUE
// facility is a group in a MessageTable entry for example in STORM_ERROR_BAD_ARGUMENT 0x85100065, 0x510 is the group.
BOOL STORMAPI SErrRegisterMessageSource(WORD facility, HMODULE module, LPVOID reserved = NULL);

void STORMAPI SErrReportResourceLeak(LPCTSTR handlename);

/*  SErrSetLastError @ 465
 *  
 *  Sets the last error for the Storm library and the Kernel32 library.
 *
 *  errorcode:  The error code that will be set.
 */
void STORMAPI SErrSetLastError(DWORD errorcode = NO_ERROR);

// 
// void STORMAPI SErrReportNamedResourceLeak(const char *pszMsg, const char *pszSubMsg = nullptr)
// void STORMAPI SErrReportResourceLeak(const char *pszMsg)

void STORMAPI SErrSuppressErrors(BOOL suppress);

// Values for dwErrCode
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
#define STORM_ERROR_REQUIRES_CODEC               0x85100074
#define STORM_ERROR_REQUIRES_DDRAW               0x85100075
#define STORM_ERROR_REQUIRES_DSOUND              0x85100076
#define STORM_ERROR_REQUIRES_UPGRADE             0x85100077
#define STORM_ERROR_STILL_ACTIVE                 0x85100078
#define STORM_ERROR_VERSION_MISMATCH             0x85100079
#define STORM_ERROR_MEMORY_ALREADY_FREED         0x8510007a
#define STORM_ERROR_MEMORY_CORRUPT               0x8510007b
#define STORM_ERROR_MEMORY_INVALID_BLOCK         0x8510007c
#define STORM_ERROR_MEMORY_MANAGER_INACTIVE      0x8510007d
#define STORM_ERROR_MEMORY_NEVER_RELEASED        0x8510007e
#define STORM_ERROR_HANDLE_NEVER_RELEASED        0x8510007f
#define STORM_ERROR_ACCESS_OUT_OF_BOUNDS         0x85100080
#define STORM_ERROR_MEMORY_NULL_POINTER          0x85100081
#define STORM_ERROR_CDKEY_MISMATCH               0x85100082
#define STORM_ERROR_FILE_CORRUPTED               0x85100083
#define STORM_ERROR_FATAL                        0x85100084
#define STORM_ERROR_GAMETYPE_UNAVAILABLE         0x85100085

#pragma endregion SErr

#pragma region SCmd

typedef struct _CMDERROR {
  DWORD   errorcode;
  LPCTSTR itemstr;
  LPCTSTR errorstr;
} CMDERROR, * CMDERRORPTR;

typedef struct _CMDPARAMS {
  DWORD   flags;
  DWORD   id;
  LPCTSTR name;
  LPVOID  variable;
  DWORD   setvalue;
  DWORD   setmask;
  union {
    BOOL    boolvalue;
    LONG    signedvalue;
    DWORD   unsignedvalue;
    LPCTSTR stringvalue;
  };
} CMDPARAMS, * CMDPARAMSPTR;

typedef BOOL(CALLBACK* SCMDCALLBACK)(CMDPARAMSPTR, LPCTSTR);
typedef void (CALLBACK* SCMDERRORCALLBACK)(CMDERRORPTR);
typedef BOOL(CALLBACK* SCMDEXTRACALLBACK)(LPCTSTR);

typedef struct _ARGLIST {
  DWORD        flags;
  DWORD        id;
  LPCTSTR      name;
  SCMDCALLBACK callback;
} ARGLIST, * ARGLISTPTR;

BOOL  STORMAPI SCmdCheckId(DWORD id);
BOOL  STORMAPI SCmdGetBool(DWORD id);
DWORD STORMAPI SCmdGetNum(DWORD id);
BOOL  STORMAPI SCmdGetString(DWORD id, LPTSTR buffer, DWORD bufferchars);
BOOL  STORMAPI SCmdProcess(LPCTSTR cmdline, BOOL skipprogname, SCMDEXTRACALLBACK extracallback, SCMDERRORCALLBACK errorcallback);
BOOL  STORMAPI SCmdRegisterArgList(const ARGLIST *listptr, DWORD numargs);
BOOL  STORMAPI SCmdRegisterArgument(DWORD flags, DWORD id, LPCTSTR name, LPVOID variableptr = NULL, DWORD variablebytes = 0, DWORD setvalue = TRUE, DWORD setmask = 0xFFFFFFFF, SCMDCALLBACK callback = NULL);

#pragma endregion SCmd

#pragma region SMem

typedef struct _SMEMBLOCKDETAILS {
  DWORD  size;
  LPVOID ptr;
  BOOL   allocated;
  BOOL   valid;
  DWORD  bytes;
  DWORD  overhead;
} SMEMBLOCKDETAILS, * LPSMEMBLOCKDETAILS;

typedef struct _SMEMHEAPDETAILS {
  DWORD  size;
  HANDLE handle;
  char   filename[MAX_PATH];
  int    linenumber;
  DWORD  regions;
  DWORD  committedbytes;
  DWORD  reservedbytes;
  DWORD  maximumsize;
  DWORD  allocatedblocks;
} SMEMHEAPDETAILS, * LPSMEMHEAPDETAILS;


BOOL STORMAPI SMemFindNextBlock(HANDLE heap, LPVOID prevblock, LPVOID* nextblock, LPSMEMBLOCKDETAILS details);
BOOL STORMAPI SMemFindNextHeap(HANDLE prevheap, HANDLE* nextheap, LPSMEMHEAPDETAILS details);
HANDLE STORMAPI SMemGetHeapByCaller(LPCSTR filename, int linenumber);
HANDLE STORMAPI SMemGetHeapByPtr(LPVOID ptr);
LPVOID STORMAPI SMemHeapAlloc(HANDLE handle, DWORD flags, DWORD bytes);
HANDLE STORMAPI SMemHeapCreate(DWORD options, DWORD initialsize, DWORD maximumsize);
BOOL STORMAPI SMemHeapDestroy(HANDLE handle);
BOOL STORMAPI SMemHeapFree(HANDLE handle, DWORD flags, LPVOID ptr);

/*  SMemCopy @ 491
 *  
 *  Copies a block of memory from source to destination.
 *  This function immediately calls memcpy. See online documentation
 *  of memcpy for more details.
 *
 *  dest:   The destination buffer.
 *  source: The source buffer.
 *  size:   The number of bytes to copy.
 */
void
STORMAPI
SMemCopy(
    void *dest, 
    const void *source, 
    size_t size);

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
    void *location,
    size_t length,
    char fillWith = 0);

#define SMFill(l,f) (SMemFill(l, sizeof(l), f))

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
    void *location,
    size_t length);

#define SMZero(l) (SMemZero(l, sizeof(l)))


int STORMAPI SMemCmp(void *location1, void *location2, DWORD size);

#pragma endregion SMem

#pragma region SStr

#define  SSTR_UNBOUNDED           0x7FFFFFFF

/*  SStrCopy @ 501
 *  
 *  Copies a string from src to dest (including NULL terminator)
 *  until the max_length is reached.
 *
 *  dest:         The destination array.
 *  src:          The source array.
 *  max_length:   The maximum length of dest.
 *
 *  <returns>the number of characters copied.</returns>
 */
DWORD STORMAPI SStrCopy(LPTSTR dest, LPCTSTR source, DWORD destsize = SSTR_UNBOUNDED);


#define  SSTR_HASH_CASESENSITIVE  0x00000001


/*  SStrHash @ 502
 *  
 *  Creates a simple hash for the string. This function
 *  should NOT be used for sensitive information.
 *
 *  string:   The input string.
 *  flags:    If STORM_HASH_ABSOLUTE is set then this
              function uses the absolute string, otherwise
              it will convert backslashes to forward
              slashes and some other processing.
 *  seed:     The hash seed. If this value is 0 then the
 *            default value 0x7FED7FED will be used.
 *
 *  <returns>the 32-bit hash of the string.</returns>
 */
DWORD STORMAPI SStrHash(LPCTSTR string, DWORD flags = 0, DWORD Seed = 0);

// AKA strncat (string concatenation with cutoff)
// Note: has return value in newer versions of Storm, should be void in older versions (i.e. Diablo)
// Returns the concatenated string size
DWORD STORMAPI SStrPack(LPTSTR dest, LPCTSTR source, DWORD destsize = SSTR_UNBOUNDED);

void STORMAPI SStrTokenize(LPCTSTR* string, LPTSTR buffer, DWORD bufferchars, LPCTSTR whitespace, BOOL* quoted = NULL);

/*  SStrChr @ 505
 *
 *  Searches a string for the given character. See strchr documentation for more details.
 *
 *  string:   The string to search.
 *  ch:       The character to search for.
 *  reverse:  Whether to search from the end of the string (TRUE) or the beginning (FALSE). Default: FALSE.
 *
 *  <returns>a pointer to the first occurance of the character.</returns>
 */
LPTSTR STORMAPI SStrChr(LPCTSTR string, char ch, BOOL reverse = FALSE);

/*  SStrLen @ 506
 *  
 *  Retrieves the length of a string.
 *
 *  string: The input string of which to obtain a length for.
 *
 *  <returns>the length of the string.</returns>
 */
DWORD STORMAPI SStrLen(LPCTSTR string);

/*  SStrCmp @ 508
 *  
 *  Compares two strings case sensitive.
 *
 *  string1:  The first string.
 *  string2:  The second string.
 *  size:     The maximum amount of characters to compare.
 *
 *  <returns>0 if strings are equal. See strcmp documentation for more details.</returns>
 */
int STORMAPI SStrCmp(const char *string1, const char *string2, size_t size);


/*  SStrCmpI @ 509
 *  
 *  Compares two strings case insensitive.
 *
 *  string1:  The first string.
 *  string2:  The second string.
 *  size:     The maximum amount of characters to compare.
 *
 *  <returns>0 if strings are equal. See strcmpi documentation for more details.</returns>
 */
int STORMAPI SStrCmpI(const char *string1, const char *string2, size_t size);


/*  SStrUpper @ 510
 *  
 *  Converts all lower-case alpha characters of a string to upper-case.
 *
 *  string:   The string to convert.
 *  
 *  <returns>the same pointer given in the input.</returns>
 */
char* STORMAPI SStrUpper(char* string);

#pragma endregion SStr

#pragma region SRgn

void STORMAPI SRgnClear(HANDLE handle);

void STORMAPI SRgnCombineRect(HANDLE handle, RECT *rect, LPVOID param, int combinemode);
void STORMAPI SRgnCreate(HANDLE *handle, DWORD reserved = 0);
void STORMAPI SRgnDelete(HANDLE handle);

void STORMAPI SRgnDuplicate(HANDLE orighandle, HANDLE* handle, DWORD reserved = 0);
void STORMAPI SRgnGetRectParams(HANDLE handle, LPCRECT rect, DWORD* numparams, LPVOID* buffer);
void STORMAPI SRgnGetRects(HANDLE handle, DWORD* numrects, LPRECT buffer);
void STORMAPI SRgnGetBoundingRect(HANDLE handle, LPRECT rect);

#pragma endregion SRgn

#pragma region SLog

void STORMAPI SLogClose(HANDLE log);
BOOL STORMAPI SLogCreate(LPCTSTR filename, DWORD flags, HANDLE *log);

void STORMAPI SLogDump(HANDLE log, LPCVOID data, DWORD bytes);
void STORMAPI SLogFlush(HANDLE log);
void STORMAPI SLogFlushAll();
void __cdecl SLogPend(HANDLE log, LPCTSTR format, ...);
void __cdecl SLogWrite(HANDLE log, LPCTSTR format, ...);

#pragma endregion SLog

#pragma region SComp

#define  SCOMP_HINT_NONE                0
#define  SCOMP_HINT_BINARY              1
#define  SCOMP_HINT_TEXT                2
#define  SCOMP_HINT_EXECUTABLE          3
#define  SCOMP_HINT_ADPCM4              4
#define  SCOMP_HINT_ADPCM6              5
#define  SCOMP_HINTS                    6

#define  SCOMP_OPT_DEFAULT              0
#define  SCOMP_OPT_COMPRESSION          1
#define  SCOMP_OPT_SPEED                2
#define  SCOMP_OPT_QUALITY              3

#define  SCOMP_TYPE_HUFFMAN             0x01
#define  SCOMP_TYPE_PKWARE              0x08
#define  SCOMP_TYPE_LOSSY_ADPCM_MONO    0x10
#define  SCOMP_TYPE_LOSSY_ADPCM_STEREO  0x20

BOOL STORMAPI SCompCompress(LPVOID dest, DWORD * destsize, LPCVOID source, DWORD sourcesize, DWORD compressiontypes, DWORD hint, DWORD optimization);
BOOL STORMAPI SCompDecompress(LPVOID dest, DWORD * destsize, LPCVOID source, DWORD sourcesize); 

#pragma endregion SComp

void STORMAPI SLogVWrite(HANDLE log, LPCTSTR format, va_list va);

#pragma region SErr


/* SErrDisplayErrorFmt @ 562
 *
 * Displays a formatted error message. The message is detailed and flexible for many applications.
 * The message will be different if there is a debugger attached. Will typically terminate the application
 * unless the option to continue is given.
 *
 *  dwErrMessage:   The error code. See SErrGetLastError and GetLastError.
 *  logfilename:    The name of the file or object that this call belongs to.
 *  logline:        The line in the file or one of the SLOG_ macros.
 *  allowOption:    If TRUE, allows the user the option to continue execution, otherwise the program will terminate.
 *  exitCode:       The exit code used for program termination.
 *  format:         Additional message formatting. See printf.
 *
 *  <returns>TRUE if the user chose to continue execution, FALSE otherwise.</returns>
 */
BOOL
__cdecl
SErrDisplayErrorFmt(
    DWORD dwErrMsg,
    const char *logfilename,
    int logline,
    BOOL allowOption,
    int exitCode,
    const char *format,
    ...);

#define SEDisplayErrorFmt(err,...) SErrDisplayErrorFmt(err, __FILE__, __LINE__, FALSE, 1, __VA_ARGS__)

/*  SErrCatchUnhandledExceptions @ 567
 *  
 *  Registers a top-level exception filter managed entirely by Storm.
 *  The registered filter will display formatted exception information by calling SErrDisplayError.
 */
void STORMAPI SErrCatchUnhandledExceptions();

#pragma endregion SErr

#pragma region SStr


/*  SStrChrF @ 571
 *  
 *  Searches a string for the given character. See 
 *  strchr documentation for more details.
 *
 *  string:   The string to search.
 *  ch:       The character to search for.
 *  
 *  <returns>a pointer to the first occurance of the character.</returns>
 */
LPTSTR STORMAPI SStrChrF(LPCTSTR string, char ch);
LPTSTR STORMAPI SStrChrR(LPCTSTR string, char ch); // reverse


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
 *  <returns>the number of characters written.</returns>
 */
size_t
__cdecl
SStrVPrintf(
    char *dest, 
    size_t size, 
    const char *format, ...);

#pragma endregion SStr

#pragma region SBig

int STORMAPI SBigDel(void *buffer);

int STORMAPI SBigFromBinary(void *buffer, const void *str, size_t size);

int STORMAPI SBigNew(void **buffer);

int STORMAPI SBigPowMod(void *buffer1, void *buffer2, int a3, int a4);

int STORMAPI SBigToBinaryBuffer(void *buffer, int length, int a3, int a4);

#pragma endregion SBig

#if defined(__GNUC__) || defined(__cplusplus)
}
#endif

#ifdef __cplusplus
/////////////////////////////////////////////////////////////////////////
/// High Level wrappers
#include <string>

namespace Storm
{
  class CFile;
  class CArchive;

  class CFile
  {
  private:
    HANDLE hFile;
    bool valid;
  public:
    CFile()
      : hFile(nullptr)
      , valid(false)
    {}

    CFile(const std::string &sFileName, unsigned flags = SFILE_FROM_MPQ, HANDLE hMpq = nullptr)
      : hFile(nullptr)
      , valid( SFileOpenFileEx(hMpq, sFileName.c_str(), flags, &hFile) != FALSE )
    { }

    CFile(CFile &&other)
      : hFile( other.hFile )
      , valid( other.valid )
    {
      other.valid = false;
      other.hFile = nullptr;
    }

    ~CFile()
    {
      close();
    }

    HANDLE handle() const
    {
      return hFile;
    }

    bool open(const std::string &sFileName, unsigned flags = SFILE_FROM_MPQ, HANDLE hMpq = nullptr)
    {
      valid = SFileOpenFileEx(hMpq, sFileName.c_str(), flags, &hFile) != FALSE;
      return isValid();
    }

    bool close()
    {
      bool result = false;
      if (isValid())
        result = SFileCloseFile(hFile) != FALSE;
      hFile = nullptr;
      return result;
    }

    size_t size() const
    {
      return SFileGetFileSize(hFile, nullptr);
    }

    bool read(void *buffer, size_t buffSize) const
    {
      DWORD read = 0;
      BOOL result = SFileReadFile(hFile, buffer, buffSize, &read, 0);
      
      return result != FALSE && read == buffSize;
    }

    bool isValid() const { return valid && hFile; }
    explicit operator bool() const { return isValid(); }
    bool operator !() const { return !isValid(); }
  };


  class CArchive
  {
  private:
    HANDLE hMpq;
    bool valid;
  public:
    CArchive()
      : hMpq(nullptr)
      , valid(false)
    {}

    CArchive(const std::string &sArchiveFileName, unsigned flags = 0, unsigned priority = 0)
      : hMpq(nullptr)
      , valid( SFileOpenArchive(sArchiveFileName.c_str(), priority, flags, &hMpq) != FALSE )
    { }

    CArchive(CArchive &&other)
      : hMpq(other.hMpq)
      , valid(other.valid)
    {
      other.valid = false;
    }

    ~CArchive()
    {
      close();
    }

    bool open(const std::string &sArchiveFileName, unsigned flags = 0, unsigned priority = 0)
    {
      valid = SFileOpenArchive(sArchiveFileName.c_str(), priority, flags, &hMpq) != FALSE;
      return isValid();
    }

    bool close()
    {
      bool result = false;
      if (isValid())
        result = SFileCloseArchive(hMpq) != FALSE;
      hMpq = nullptr;
      return result;
    }

    HANDLE handle() const
    {
      return hMpq;
    }

    bool isValid() const { return valid && hMpq; }
    explicit operator bool() const { return isValid(); }
    bool operator !() const { return !isValid(); }

    CFile openFile( const std::string &sFileName ) const
    {
      return CFile(sFileName, SFILE_FROM_MPQ, hMpq);
    }
  };

};

#endif


#endif
