#include "Socket.h"

TWSAInitializer::TWSAInitializer()
{
  WSADATA WsaDat;
  if ( WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0 )
    throw GeneralException("WSA initialization failed");
  // TWSAInitializer::completion_port = CreateIoCompletionPort(NULL, NULL, NULL, 0);
}

TWSAInitializer::~TWSAInitializer()
{
  WSACleanup();
}

TWSAInitializer _init_wsa;

// constructors

Socket::Socket()
  : _s(NULL), _state(0), _bound(0)
{
}

Socket::~Socket()
{
}

int Socket::getState() const
{
  return _state;
}

int Socket::getBoundPort() const
{
  return _bound;
}
