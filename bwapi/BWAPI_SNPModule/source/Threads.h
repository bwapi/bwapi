#pragma once
#include <windows.h>
#include "Common.h"
#include "LocalPC.h"

namespace LUDP
{
  extern bool gbWantExit;

  DWORD WINAPI RecvThread(LPVOID);

  #ifndef _PKTUDP
  #define _PKTUDP

  struct packet
  {
    WORD wType;
    WORD wSize;
  };

  struct gameInfo
  {
    DWORD dwGameState;
    char  info[1];
  };

  struct pktq
  {
    char        bData[PKT_SIZE];
    SOCKADDR_IN saFrom;
    DWORD       dwLength;
    pktq        *pNext;
  };

  #endif

  extern volatile pktq *gpRecvQueue;
  extern volatile void *gpGameAdvert;
};

namespace LTST
{
  extern bool gbWantExit;

  DWORD WINAPI RecvThread(LPVOID);

  #ifndef _PKTTST
  #define _PKTTST

  struct pktq
  {
    SOCKADDR_IN saFrom;
    char        bData[PKT_SIZE];
    DWORD       dwLength;
    pktq        *pNext;
  };

  #endif

  extern volatile pktq *gpRecvQueue;
};
