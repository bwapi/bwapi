#pragma once

#include "../SNP/SNPNetwork.h"

#include "../Util/Types.h"
#include "UDPSocket.h"

namespace DRIP
{
  extern SNP::NetworkInfo networkInfo;

  class DirectIP : public SNP::Network<UDPAddr>
  {
  public:
    DirectIP(){};
    ~DirectIP(){};

    void initialize() override;
    void destroy() override;
    void requestAds() override;
    void sendAsyn(const SNETADDR& to, Util::MemoryFrame packet) override;
    void receive() override;
    void startAdvertising(Util::MemoryFrame ad) override;
    void stopAdvertising() override;

    void processIncomingPackets();
  };
};
