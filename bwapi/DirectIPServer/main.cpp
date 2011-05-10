#include "main.h"
#include <stdio.h>
#include <list>

std::list<GameStruc> serverMsg;
std::list<GameStruc> gamelist;

SOCKET s;
SOCKADDR_IN addr;

#define SOCKCHECK(x) if ( x == SOCKET_ERROR ) printf("An error has occured in the following call:\n    " #x "\n")

DWORD dwNextGameId = 1;

// @TODO: new thread, Remove old games, request updates every so often
int main(int argc, char* argv[])
{
  // Initialize winsock
  WSADATA WsaDat;
  int wsaErr = WSAStartup(MAKEWORD(2, 2), &WsaDat);
  if ( wsaErr != NO_ERROR )
  {
    printf("WSAStartup failed: %d\n", wsaErr);
    return 0;
  }

  // Declare socket address and socket information
  addr.sin_family           = AF_INET;
  addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
  addr.sin_port             = htons(6112);
  s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  
  // bind the socket
  SOCKCHECK( bind(s, (sockaddr*)&addr, sizeof(SOCKADDR_IN)) );

  printf("DirectIP Server Initialized.\n");

  // Create server message
  GameStruc serv    = GameStruc();
  serv.dwGameState  = 4;
  strcpy(serv.szGameName, "Hello, this is a test");
  serverMsg.push_back(serv);
  serv    = GameStruc();
  serv.dwGameState  = 4;
  strcpy(serv.szGameName, "for a DirectIP game server.");
  serverMsg.push_back(serv);

  // noob loop
  for (;;)
  {
    // Initialize buffers
    char bRecvBuffer[1024] = { 0 };
    SOCKADDR from          = { 0 };
    int fromsize = sizeof(from);

    // call recvfrom, should be blocking mode so that it waits until data has been received
    int iRecvSize = recvfrom(s, bRecvBuffer, 1024, NULL, &from, &fromsize);

    // If connection has closed ??
    if ( iRecvSize == 0 )
    {
      printf("Connection closed.");
      break;
    }

    // If an error occured in recvfrom
    if ( iRecvSize == SOCKET_ERROR )
    {
      char szErrBuffer[256];
      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), LANG_USER_DEFAULT, szErrBuffer, 256, NULL);
      printf("An error has occured in recvfrom: \n    %s\n", szErrBuffer);
      break;
    }

    switch ( *(DWORD*)&bRecvBuffer[0] )
    {
    case REQUEST_GAMEINFO:  // received request for game info
      {
        printf("Received a game list request.\n");
        // iterate the game list
        for ( std::list<GameStruc>::iterator i = serverMsg.begin(); i != serverMsg.end(); ++i )
        {
          // send game entry
          char szSendBuffer[512]    = { 0 };
          *(DWORD*)&szSendBuffer[0] = SEND_GAMEINFO;
          memcpy(&szSendBuffer[4], &(*i), sizeof(GameStruc));
          SOCKCHECK( sendto(s, szSendBuffer, 512, 0, &from, sizeof(from)) );
        }
        for ( std::list<GameStruc>::iterator i = gamelist.begin(); i != gamelist.end(); ++i )
        {
          // send game entry
          char szSendBuffer[512]    = { 0 };
          *(DWORD*)&szSendBuffer[0] = SEND_GAMEINFO;
          memcpy(&szSendBuffer[4], &(*i), sizeof(GameStruc));
          SOCKCHECK( sendto(s, szSendBuffer, 512, 0, &from, sizeof(from)) );
        }
        break;
      }
    case SEND_GAMEINFO: // received game info
      {
        GameStruc *pGame = NULL;
        // iterate the game list
        for ( std::list<GameStruc>::iterator i = gamelist.begin(); i != gamelist.end(); ++i )
        {
          // find an entry that matches the host that sent it
          if ( memcmp(&i->addrHost, &from, sizeof(SOCKADDR)) != 0 )
            continue;
          pGame = &(*i);
          break;
        }

        // if the game is a new game entry
        if ( !pGame )
        {
          // add the entry to the game list
          gamelist.push_back(GameStruc());
          pGame = &gamelist.back();
        }

        // copy the game data from the packet to the game entry
        memcpy(pGame, &bRecvBuffer[4], sizeof(GameStruc));
        pGame->dwTimer  = GetTickCount();
        pGame->addrHost = from;
        pGame->pExtra   = pGame->extraBytes;

        printf("Received a new game update: \"%s\"\n", pGame->szGameName);
        break;
      }
    case GAME_PACKET:
      printf("(ignored) Received game packet.");
      break;
    default:
      printf("(ignored) Bad packet ID: 0x%08X", *(DWORD*)&bRecvBuffer[0]);
    }
  }
  closesocket(s);
  WSACleanup();
  return 0;
}

GameStruc::GameStruc()
{
  memset(this, 0, sizeof(GameStruc));
  dwIndex = ++dwNextGameId;
  addrHost.sa_family  = AF_INET;
  this->dwProduct     = 'SEXP';
  this->dwTimer       = GetTickCount();
  this->dwVersion     = 205;
}

GameStruc::~GameStruc()
{
}
