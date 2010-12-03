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
  int playerProcessIDs[10];
  int tableIndex;
  time_t lastUpdate;
};
struct GameInfoTable
{
  GameInfo gameInfo[256];
};
class SharedMemory
{
  public:
  SharedMemory();
  ~SharedMemory();
  bool connect();
  void disconnect();
  bool advertiseLadderGame(GameInfo* gm);
  void keepAliveLadderGame();
  void removeLadderGameAd();
  //parses thge game info table and constructs the games list
  void updateGameList();
  bool connectToLadderGame(GameInfo* gm);
  void disconnectFromLadderGame();
  bool isConnectedToSharedMemory();
  bool isConnectedToLadderGame();
  void update();
  bool sendData(const char *buf, unsigned int len, DWORD processID);
  int receiveData(const char *buf, unsigned int len, DWORD& processID, bool isBlocking = true);
  std::list<GameInfo*> games;
  HANDLE myPipeHandle;
  char myPipeName[256];
  GameInfoTable* data;
  HANDLE mapFileHandle;
  int myIndex;;
  bool ownsLadderGame;
};