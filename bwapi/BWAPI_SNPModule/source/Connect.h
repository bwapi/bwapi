#pragma once
#include <windows.h>
#include <Winsock.h>

extern SOCKET   gsSend;
extern SOCKET   gsRecv;
extern SOCKADDR gaddrSend;
extern SOCKADDR gaddrRecv;
extern SOCKADDR gaddrBCFrom;

extern char gszThisIP[16];

extern SOCKET   gsBroadcast;
extern SOCKADDR gaddrBroadcast;

extern volatile DWORD gdwSendCalls;
extern volatile DWORD gdwSendBytes;
extern volatile DWORD gdwRecvCalls;
extern volatile DWORD gdwRecvBytes;

int SendData(SOCKET s, const char *buf, int len, const SOCKADDR *to);

SOCKET   MakeUDPSocket();
SOCKADDR *InitAddr(SOCKADDR *addr, const char *ip = "127.0.0.1", WORD wPort = 6112);
SOCKADDR *InitAddr(SOCKADDR *addr, DWORD dwSeed, WORD wPort = 6112);

bool InitializeSockets();
void DestroySockets();

