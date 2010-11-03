#pragma once
#include <windows.h>
#include <deque>
#include "Common.h"
#include "LocalPC.h"

extern bool gbWantExit;

DWORD WINAPI RecvThread(LPVOID);
DWORD WINAPI BroadcastThread(LPVOID);


#ifndef _PKT
#define _PKT

struct broadcastPkt
{
  WORD  wChecksum;
  WORD  wSize;
  WORD  wType;
  WORD  wReserved;
  DWORD dwProduct;
  DWORD dwVersion;
  DWORD dwGameState;
};

struct pktq
{
  SOCKADDR addr;
  char     bData[LOCL_PKT_SIZE];
  DWORD    dwLength;
};

#endif

extern std::deque<pktq*> recvQueue;
extern void *gpGameAdvert;
