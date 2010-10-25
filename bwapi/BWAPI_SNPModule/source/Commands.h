#pragma once
#include <windows.h>
#include <queue>
#include "../../Storm/storm.h"

#include "LocalPC.h"
#include "Connect.h"

#ifndef SNP_PKT
#define SNP_PKT
struct pkt
{
  DWORD dwCommand;
  char  bData[LOCL_PKT_SIZE];
};

struct pktq
{
  SOCKADDR_IN addr;
  pkt         packet;
  DWORD       dwLength;
};


#endif

extern std::queue<pktq*> recvQueue;
extern DWORD gdwBroadcastCount;

int BroadcastCommand(DWORD dwCmdID, char *data = NULL, DWORD dwDataSize = 0);
int SendCommand(SOCKADDR *dstAddr, DWORD dwCmdID, char *data = NULL, DWORD dwDataSize = 0);
void ParseCommand(SOCKADDR *from, char *data, DWORD dwDataSize);
