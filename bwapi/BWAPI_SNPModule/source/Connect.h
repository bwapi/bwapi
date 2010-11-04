#pragma once
#include <windows.h>
#include <Winsock.h>

extern SOCKET   gsBCSend;
extern SOCKET   gsBCRecv;
extern SOCKADDR gaddrBCSend;
extern SOCKADDR gaddrBCRecv;
extern SOCKADDR gaddrBCFrom;

extern SOCKET   gsBroadcast;
extern SOCKADDR gaddrBroadcast;

extern DWORD gdwSendCalls;
extern DWORD gdwSendBytes;
extern DWORD gdwRecvCalls;
extern DWORD gdwRecvBytes;

SOCKET   MakeUDPSocket();
SOCKADDR *InitAddr(SOCKADDR *addr, const char *ip = "127.0.0.1", WORD wPort = 6112);
SOCKADDR *InitAddr(SOCKADDR *addr, DWORD dwSeed, WORD wPort = 6112);

bool InitializeSockets();
void DestroySockets();

