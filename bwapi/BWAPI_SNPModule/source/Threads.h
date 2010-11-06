#pragma once
#include <windows.h>
#include "Common.h"
#include "LocalPC.h"

namespace LUDP
{
  extern bool gbWantExit;

  DWORD WINAPI RecvThread(LPVOID);

  #ifndef _PKT
  #define _PKT

  struct packet
  {
    WORD wType;
    WORD wSize;
    DWORD dwGameState;
  };

  struct pktq
  {
    SOCKADDR_IN saFrom;
    char        bData[LUDP_PKT_SIZE];
    DWORD       dwLength;
    pktq        *pNext;
  };

  #endif

  extern volatile pktq *gpRecvQueue;
  extern volatile void *gpGameAdvert;
};
