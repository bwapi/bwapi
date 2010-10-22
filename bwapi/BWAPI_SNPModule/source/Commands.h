#pragma once
#include <windows.h>
#include "../../Storm/storm.h"

#include "Connect.h"
#ifndef SNP_PKT
#define SNP_PKT
struct pkt
{
  DWORD dwCommand;
  char  bData[512];
};

struct pktq
{
  SOCKADDR_IN addr;
  pkt         packet;
  DWORD       dwLength;
};


#endif

extern DWORD gdwBroadcastCount;

int BroadcastCommand(DWORD dwCmdID, char *data = NULL, DWORD dwDataSize = 0);
int SendCommand(SOCKADDR *dstAddr, DWORD dwCmdID, char *data = NULL, DWORD dwDataSize = 0);
void ParseCommand(SOCKADDR *from, char *data, DWORD dwDataSize);
