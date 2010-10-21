#pragma once
#include <windows.h>
#include <Winsock.h>

extern SOCKET   gsGame;
extern SOCKADDR gaddrRecvGame;

extern SOCKET   gsBroadcast;
extern SOCKADDR gaddrRecvBroadcast;

extern DWORD gdwSendCalls;
extern DWORD gdwSendBytes;
extern DWORD gdwRecvCalls;
extern DWORD gdwRecvBytes;

SOCKET   MakeUDPSocket();
SOCKADDR *InitAddr(SOCKADDR *addr, const char *ip = "127.0.0.1", WORD wPort = 6112);

bool InitializeSockets();
void DestroySockets();

