#pragma once
#include <windows.h>
#include <Winsock.h>

extern SOCKET gsGame;
extern SOCKET gsBroadcast;

extern DWORD gdwSendCalls;
extern DWORD gdwSendBytes;
extern DWORD gdwRecvCalls;
extern DWORD gdwRecvBytes;

SOCKET   MakeUDPSocket();
SOCKADDR *InitAddr(SOCKADDR *addr, const char *ip = "127.0.0.1", WORD wPort = 6112);

void InitializeSockets();
void DestroySockets();

#define i(x) MessageBox(NULL, x, "*", MB_OK | MB_ICONASTERISK)
#define w(x) MessageBox(NULL, x, "!", MB_OK | MB_ICONWARNING)
#define e(x) MessageBox(NULL, x, "Error!", MB_OK | MB_ICONERROR)
