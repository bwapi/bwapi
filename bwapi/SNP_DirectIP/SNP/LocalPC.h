#pragma once

#include "SNPNetwork.h"

#include <Util\Types.h>
#include "UDPSocket.h"

namespace SMEM
{
  extern SNP::NetworkInfo networkInfo;

  class LocalPC : public SNP::Network<int>
  {
  public:
    LocalPC(){};
    ~LocalPC(){};

    void initialize();
    void destroy();
    void requestAds();
    void sendAsyn(const int& to, Util::MemoryFrame packet);
    void receive();
    void startAdvertising(Util::MemoryFrame ad);
    void stopAdvertising();

    void processIncomingPackets();
  };
};
