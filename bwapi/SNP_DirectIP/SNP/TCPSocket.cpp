#include "TCPSocket.h"

// constructors

TCPSocket::TCPSocket()
{
}

TCPSocket::~TCPSocket()
{
  release();
}

void TCPSocket::init()
{
    throw std::logic_error("Function not yet implemented");
}

void TCPSocket::release() throw()
{
    throw std::logic_error("Function not yet implemented");
}

void TCPSocket::bind(int port)
{
    throw std::logic_error("Function not yet implemented");
}

/*void TCPSocket::connect(const Addr& target) {
    this->target = target;
}*/

/*SocketMode TCPSocket::getMode() const {
    return SocketMode::CLIENT; //TODO: Implement correctly
}*/

void TCPSocket::sendPacket(const Addr& target, Util::MemoryFrame data)
{
    throw std::logic_error("Function not yet implemented");
}

Util::MemoryFrame TCPSocket::receivePacket(Addr &target, Util::MemoryFrame dest)
{
    throw std::logic_error("Function not yet implemented");
}

int TCPSocket::getNextPacketSize()
{
    throw std::logic_error("Function not yet implemented");
}

void TCPSocket::setBlockingMode(bool block)
{
    throw std::logic_error("Function not yet implemented");
}
