#include "LocalPC.h"
#include "Connect.h"

namespace LUDP
{
  bool __stdcall fxn0(int a1, int a2, int a3)
  {
    // not important right now
    i(__FUNCTION__);
    return true;
  }

  bool __stdcall spiFree(void *a1, int a2, int a3)
  {
    // This function is complete
    if ( a1 && a2 )
    {
      SMemFree(a1, __FILE__, __LINE__, 0);
      return true;
    }
    else
    {
      SetLastError(ERROR_INVALID_PARAMETER);
      return false;
    }
  }

  bool __stdcall spiError(int a1, int a2, int a3)
  {
    // This function is complete
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }

  bool __stdcall spiInitializeDevice(int a1, void *a2, void *a3, DWORD *a4, void *a5)
  {
    // This function is complete
    return false;
  }

  bool __stdcall spiEnumDevices(DWORD *a1)
  {
    // This function is complete
    *a1 = 0;
    return true;
  }

  bool __stdcall spiReceive(SOCKADDR **addr, char **data, DWORD *databytes)
  {
    // This function is complete
    if ( addr )
      *addr = NULL;
    if ( data )
      *data = NULL;
    if ( databytes )
      *databytes = 0;
    SetLastError(STORM_ERROR_NO_MESSAGES_WAITING);
    return false;
  }

  bool __stdcall spiSelectGame(int a1, clientInfo *gameClientInfo, userInfo *userData, battleInfo *bnCallbacks, moduleInfo *moduleData, int a6)
  {
    /* Looks like an old function and doesn't seem like it's used anymore
       UDPN's function Creates an IPX game select dialog window  */
    return false;
  }

  bool __stdcall spiSend(int a1, int a2, int a3, int a4, int a5)
  {
    // This function is complete
    return false;
  }

  bool __stdcall spiInitialize()
  {
    // This function is complete
    return true;
  }

  bool __stdcall spiLeagueGetName(char *pszDest, DWORD dwSize)
  {
    // This function is complete
    return true;
  }
};
