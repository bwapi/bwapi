#pragma once

#include "SNPNetwork.h"

#include <Util/Types.h>
#include "UDPSocket.h"

namespace DRIP
{
  extern SNP::NetworkInfo networkInfo;

  class DirectIP : public SNP::Network<Addr>
  {
  public:
    DirectIP(){};
    ~DirectIP(){};

    void initialize();
    void destroy();
    void requestAds();
    void sendAsyn(const Addr& to, Util::MemoryFrame packet);
    void receive();
    void startAdvertising(Util::MemoryFrame ad);
    void stopAdvertising();

    void processIncomingPackets();
  };
};
