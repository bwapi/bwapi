
#include "LocalPC.h"

#include "../SNP/Output.h"

#include "../Util/SharedMemory.h"
#include "../Util/SharedStructure.h"
#include "../Util/Mutex.h"

namespace SMEM
{
  SNP::NetworkInfo networkInfo = {"Local PC", 'SMEM', "",
    // CAPS:
  {sizeof(SNETCAPS), SNET_CAPS_RETAILONLY | SNET_CAPS_BASICINTERFACE | SNET_CAPS_PAGELOCKEDBUFFERS, SNP::PACKET_SIZE, 16, 256, 1000, 50, 8, 2}};

#define INTERLOCKED Util::Mutex::Lock globalLock(mutex);

  struct Packet
  {
    int sender;
    int length;
    char data[508];
  };
  struct PeerData
  {
    unsigned lastOccupied; // timer
    char ad[512];
    bool isAdvertising;
    int incomingCount;
    Packet incoming[16];

    bool inline isOccupied() const
    {
      return lastOccupied > 0; // && GetTickCount() < lastOccupied + 3000;
    }
  };
  struct SharedData
  {
    PeerData peer[8];
  };
  Util::SharedStructure<SharedData> session;
  Util::Mutex mutex("Local\\LocalPC_Network_Mutex");

  SharedData *shd = NULL;

  int self;

  //------------------------------------------------------------------------------------------------------------------------------------
  void LocalPC::processIncomingPackets()
  {
    INTERLOCKED;
    try
    {
      // mark our slot
      shd->peer[self].lastOccupied = GetTickCount();

      // pass all packets to storm
      for(int i = 0; i < shd->peer[self].incomingCount; i++)
      {
        Packet &packet = shd->peer[self].incoming[i];
        passPacket(packet.sender, Util::MemoryFrame(packet.data, packet.length));
      }

      // clear incoming queue
      shd->peer[self].incomingCount = 0;
    }
    catch(GeneralException &e)
    {
      DropLastError("processIncomingPackets failed: %s", e.getMessage().c_str());
    }
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------------------------
  void LocalPC::initialize()
  {
    INTERLOCKED;

    // init shared stuff
    bool just_created = session.create("Local\\LocalPC_Network_Memory");
    shd = &session.get();

    // if the shared memory was just created, init it
    if(just_created)
    {
      DropMessage(0, "Memory newly created");
      memset(shd, 0, sizeof(SharedData));
    }

    // reserve an index for oneself
    self = -1;
    for(int i = 0; i < 8; i++)
    {
      if(!shd->peer[i].isOccupied())
      {
        self = i;
        break;
      }
    }
    DropMessage(0, "self: %d", self);
    if(self == -1)
    {
      DropMessage(2, "could not reserve self. exiting");
      exit(1);
    }
    PeerData &selfData = shd->peer[self];
    selfData.lastOccupied = GetTickCount();
    selfData.isAdvertising = false;
    selfData.incomingCount = 0;

  }
  void LocalPC::destroy()
  {
    if (shd)
      shd->peer[self].lastOccupied = 0;
    session.release();
  }
  void LocalPC::requestAds()
  {
    processIncomingPackets();

    INTERLOCKED;
    for(int i = 0; i < 8; i++)
    {
      if(shd->peer[i].isOccupied())
      {
        if(shd->peer[i].isAdvertising)
        {
          passAdvertisement(i, Util::MemoryFrame::from(shd->peer[i].ad));
        }
      }
    }
  }
  void LocalPC::sendAsyn(const SNETADDR& him, Util::MemoryFrame packet)
  {
    processIncomingPackets();

    int peerid = *reinterpret_cast<const int*>(&him);
    if(!shd->peer[peerid].isOccupied())
      return;

    INTERLOCKED;
    // push the packet on target's packetqueue
    PeerData &peerData = shd->peer[peerid];
    int slotIndex = peerData.incomingCount++;
    if(slotIndex >= 16)
    {
      DropMessage(1, "stacked 16 packets, no space");
      return;
    }
    if(packet.size() > 508)
    {
      DropMessage(1, "packet bigger than 508 bytes (%d), being cut", packet.size());
    }
    Util::MemoryFrame::from(peerData.incoming[slotIndex].data).write(packet);
    peerData.incoming[slotIndex].length = packet.size();
    peerData.incoming[slotIndex].sender = self;
  }
  void LocalPC::receive()
  {
    processIncomingPackets();
  }
  void LocalPC::startAdvertising(Util::MemoryFrame ad)
  {
    INTERLOCKED;
    Util::MemoryFrame::from(shd->peer[self].ad).write(ad);
    shd->peer[self].isAdvertising = true;
  }
  void LocalPC::stopAdvertising()
  {
    shd->peer[self].isAdvertising = false;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
};
