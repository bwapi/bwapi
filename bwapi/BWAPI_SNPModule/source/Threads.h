#pragma once
#include <windows.h>
#include "Common.h"
#include "LocalPC.h"

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
  SOCKADDR saFrom;
  char     bData[LOCL_PKT_SIZE];
  DWORD    dwLength;
  pktq     *pNext;
};

#endif

extern pktq *gpRecvQueue;
extern bool gbRecvShit;
extern void *gpGameAdvert;
