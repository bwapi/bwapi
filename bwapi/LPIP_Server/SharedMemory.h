#pragma once
#include <windows.h>
#include <list>
struct GameInfo
{
	int szSize;
	int flType;
	DWORD dwGameState;
	char chGameName[128];
	char chGameStats[128];
  int tableIndex;
  DWORD serverProcessID;
  time_t lastUpdate;
};
struct PlayerInfo
{
  DWORD procID;
  int tableIndex;
  time_t lastUpdate;
};
#define GAME_MAX 256
#define PLAYER_MAX 10
struct GameInfoTable
{
  GameInfo gameInfo[256];
  PlayerInfo playerInfo[10];
};
class SharedMemory
{
  public:
  SharedMemory();
  ~SharedMemory();
  bool connect();
  bool connectSharedMemory();
  void disconnect();
  bool advertiseLadderGame(GameInfo* gm);
  void removeLadderGameAd();
  //parses thge game info table and constructs the games list
  void updateGameList();
  bool isConnectedToPipe();
  bool isConnectedToSharedMemory();
  void update();
  bool sendData(const char *buf, unsigned int len, DWORD processID);
  int receiveData(const char *buf, unsigned int len, DWORD *processID, bool isBlocking = true);
  void initGameInfoTable();
  bool playerExists(int i);
  bool gameExists(int i);
  std::list<GameInfo*> games;
  HANDLE myPipeHandle;
  HANDLE pipeHandle[10];
  char pipeName[10][256];
  GameInfoTable* data;
  HANDLE mapFileHandle;
  int gameIndex;
  int playerIndex;
  DWORD id;
  bool ownsLadderGame;
};