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
};
struct GameInfoTable
{
  GameInfo gameInfo[256];
  time_t gameInfoLastUpdate[256];
};
class SharedMemory
{
  public:
  SharedMemory();
  bool connect();
  void disconnect();
  bool advertiseLadderGame(GameInfo* gm);
  void removeLadderGameAd();
  //parses thge game info table and constructs the games list
  void updateGameList();
  std::list<GameInfo*> games;
  GameInfoTable* data;
  HANDLE mapFileHandle;
  int myIndex;
};