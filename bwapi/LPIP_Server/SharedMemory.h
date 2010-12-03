#pragma once
#include <windows.h>
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
  bool connect();
  void disconnect();
  GameInfoTable* data;
  HANDLE mapFileHandle;
};