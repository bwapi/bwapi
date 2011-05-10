#include <windows.h>

#define REQUEST_GAMEINFO 1
#define SEND_GAMEINFO    2
#define GAME_PACKET      3

class GameStruc
{
public:
  GameStruc();
  ~GameStruc();
  DWORD     dwIndex;
  DWORD     dwGameState;
  DWORD     dwUnk_08;
  SOCKADDR  addrHost;
  DWORD     dwUnk_1C;
  DWORD     dwTimer;
  DWORD     dwUnk_24;
  char      szGameName[128];
  char      szGameStatString[128];
  GameStruc *pNext;
  void      *pExtra;
  DWORD     dwExtraBytes;
  DWORD     dwProduct;
  DWORD     dwVersion;
  char      extraBytes[32];
};
