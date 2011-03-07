#include "UDPSocket.h"

TWSAInitializer::TWSAInitializer()
{
	WSADATA WsaDat;
    if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
		throw GeneralException("WSA initialization failed");
	// TWSAInitializer::completion_port = CreateIoCompletionPort(NULL, NULL, NULL, 0);
}

TWSAInitializer::~TWSAInitializer()
{
	WSACleanup();
}

TWSAInitializer _init_wsa;

// constructors

UDPSocket::UDPSocket()
	: _s(NULL), _bound(0)
{
}

UDPSocket::~UDPSocket()
{
  release();
}

void UDPSocket::init()
{
	release();
  _s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  _bound = 0;
	if(_s == INVALID_SOCKET)
	{
		throw GeneralException("socket failed");
	}
}

void UDPSocket::release() throw()
{
	if(_s)
	{
		::closesocket(_s);
		_s = NULL;
    _bound = 0;
	}
}

void UDPSocket::bind(int port)
{
	if(!_s)
		return;
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons((u_short)port);
	if(::bind(_s, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		throw GeneralException("::bind failed");
	}
  _bound = port;
}

void UDPSocket::sendPacket(const UDPAddr &target, Util::MemoryFrame data)
{
	int success = ::sendto(_s, (char*)data.begin(), data.size(), NULL, (sockaddr*)&target, sizeof(sockaddr_in));
	if(success == SOCKET_ERROR)
	{
		throw GeneralException("::send failed");
	}
}

Util::MemoryFrame UDPSocket::receivePacket(UDPAddr &target, Util::MemoryFrame dest)
{
  _state = 0;
  int fromlen = sizeof(sockaddr);
	int byteCount = ::recvfrom(_s, (char*)dest.begin(), dest.size(), NULL, (sockaddr*)&target, &fromlen);
	if(byteCount == SOCKET_ERROR)
	{
    _state = WSAGetLastError();
    if( _state == WSAEWOULDBLOCK
      ||_state == WSAECONNRESET)
      return Util::MemoryFrame();
		throw GeneralException("::recv failed");
	}
  return dest.getSubFrame(0, byteCount);
}

int UDPSocket::getNextPacketSize()
{
	int success = ::recv(_s, NULL, NULL, MSG_PEEK);
	if(success == SOCKET_ERROR)
	{
		throw GeneralException("::recv failed");
	}
	return success;
}

void UDPSocket::setBlockingMode(bool block)
{
  u_long nonblock = !block;
  if(::ioctlsocket(_s, FIONBIO, &nonblock) == SOCKET_ERROR)
  {
    throw GeneralException("::ioctlsocket failed");
  }
}

int UDPSocket::getState() const
{
  return _state;
}

int UDPSocket::getBoundPort() const
{
  return _bound;
}
