#pragma once

#include <Util\MemoryFrame.h>
#include <Util\Types.h>
#include "../../Storm/storm.h"

//
// The Network interface separates the Storm stuff from pure networking
//

namespace SNP
{
  const int PACKET_SIZE = 500;

  struct NetworkInfo
  {
    char          *pszName;
    DWORD         dwIdentifier;
    char          *pszDescription;
    CAPS          caps;
  };

  extern void passAdvertisement(const SOCKADDR& host, Util::MemoryFrame ad);
  extern void removeAdvertisement(const SOCKADDR& host);
  extern void passPacket(const SOCKADDR& host, Util::MemoryFrame packet);

  template<typename PEERID>
  class Network
  {
  public:
    Network()
    {
    }
    virtual ~Network()
    {
    }

    SOCKADDR makeBin(const PEERID& src)
    {
      SOCKADDR retval;
      memcpy_s(&retval, sizeof(SOCKADDR), &src, sizeof(PEERID));
      memset(((BYTE*)&retval)+sizeof(PEERID), 0, sizeof(SOCKADDR) - sizeof(PEERID));
      return retval;
    }

    // callback functions that take network specific arguments and cast them away
    void passAdvertisement(const PEERID& host, Util::MemoryFrame ad)
    {
      SNP::passAdvertisement(makeBin(host), ad);
    }
    void removeAdvertisement(const PEERID& host)
    {
      SNP::removeAdvertisement(makeBin(host));
    }
    void passPacket(const PEERID& host, Util::MemoryFrame packet)
    {
      SNP::passPacket(makeBin(host), packet);
    }

    // network plug functions
    virtual void initialize() = 0;
    virtual void destroy() = 0;
    virtual void requestAds() = 0;
    virtual void sendAsyn(const PEERID& to, Util::MemoryFrame packet) = 0;
    virtual void receive() = 0;
    virtual void startAdvertising(Util::MemoryFrame ad) = 0;
    virtual void stopAdvertising() = 0;
  };

  typedef Network<SOCKADDR> BinNetwork;
}
