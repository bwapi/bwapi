#pragma once

#include "../Util/MemoryFrame.h"
#include "../Util/Types.h"
#include <storm.h>

//
// The Network interface separates the Storm stuff from pure networking
//

namespace SNP
{
  const int PACKET_SIZE = 500;

  struct NetworkInfo
  {
    const char    *pszName;
    DWORD         dwIdentifier;
    const char    *pszDescription;
    SNETCAPS      caps;
  };

  extern void passAdvertisement(const SNETADDR& host, Util::MemoryFrame ad);
  extern void removeAdvertisement(const SNETADDR& host);
  extern void passPacket(const SNETADDR& host, Util::MemoryFrame packet);

  class BaseNetwork
  {
  public:
    BaseNetwork() = default;
    virtual ~BaseNetwork() = default;

    // network plug functions
    virtual void initialize() = 0;
    virtual void destroy() = 0;
    virtual void requestAds() = 0;
    virtual void sendAsyn(const SNETADDR& to, Util::MemoryFrame packet) = 0;
    virtual void receive() = 0;
    virtual void startAdvertising(Util::MemoryFrame ad) = 0;
    virtual void stopAdvertising() = 0;
  };

  template<typename PEERID>
  class Network : public BaseNetwork
  {
  public:
    Network() = default;
    virtual ~Network() = default;

    SNETADDR makeBin(const PEERID& src)
    {
      static_assert(sizeof(PEERID) <= sizeof(SNETADDR));
      SNETADDR retval = {0};
      memcpy_s(&retval, sizeof(SNETADDR), &src, sizeof(PEERID));
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
  };

  extern SNETSPI spiFunctions;
  extern BaseNetwork* pluggedNetwork;
}
