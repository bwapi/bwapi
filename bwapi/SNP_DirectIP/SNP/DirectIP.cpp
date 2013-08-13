
#include "DirectIP.h"

#include "Output.h"
#include "UDPSocket.h"
#include "SettingsDialog.h"

namespace DRIP
{
  SNP::NetworkInfo networkInfo = {"Direct IP", 'DRIP', "",
    // CAPS:
  {sizeof(CAPS), 0x20000003, SNP::PACKET_SIZE, 16, 256, 1000, 50, 8, 2}};

  UDPSocket session;

  // ----------------- game list section -----------------------
  Util::MemoryFrame adData;
  bool isAdvertising = false;

  // --------------   incoming section  ----------------------
  char recvBufferBytes[1024];

  // ---------------  packet IDs  ------------------------------
  const int PacketType_RequestGameStats = 1;
  const int PacketType_GameStats = 2;
  const int PacketType_GamePacket = 3;

  //------------------------------------------------------------------------------------------------------------------------------------
  void rebind()
  {
    int targetPort = atoi(getLocalPortString());
    if(session.getBoundPort() == targetPort)
      return;
    try
    {
      session.release();
      session.init();
      session.setBlockingMode(false);
      session.bind(targetPort);
      setStatusString("network ready");
    }
    catch(...)
    {
      setStatusString("local port fail");
    }
  }
  void DirectIP::processIncomingPackets()
  {
    try
    {
      // receive all packets
      while(true)
      {
        // receive next packet
        UDPAddr sender;
        Util::MemoryFrame packet = session.receivePacket(sender, Util::MemoryFrame(recvBufferBytes, sizeof(recvBufferBytes)));
        if(packet.isEmpty())
        {
          if(session.getState() == WSAECONNRESET)
          {
//            DropMessage(1, "target host not reachable");
            setStatusString("host IP not reachable");
            continue;
          }
          if(session.getState() == WSAEWOULDBLOCK)
            break;
          throw GeneralException("unhandled UDP state");
        }

        memset(sender.sin_zero, 0, sizeof(sender.sin_zero));

        int type = packet.readAs<int>();
        if(type == PacketType_RequestGameStats)
        {
          // -------------- PACKET: REQUEST GAME STATES -----------------------
          if(isAdvertising)
          {
            // send back game stats
            char sendBufferBytes[600];
            Util::MemoryFrame sendBuffer(sendBufferBytes, 600);
            Util::MemoryFrame spacket = sendBuffer;
            spacket.writeAs<int>(PacketType_GameStats);
            spacket.write(adData);
            session.sendPacket(sender, sendBuffer.getFrameUpto(spacket));
          }
        }
        else
        if(type == PacketType_GameStats)
        {
          // -------------- PACKET: GAME STATS -------------------------------
          // give the ad to storm
          passAdvertisement(sender, packet);
        }
        else
        if(type == PacketType_GamePacket)
        {
          // -------------- PACKET: GAME PACKET ------------------------------
          // pass strom packet to strom
          passPacket(sender, packet);
        }
      }
    }
    catch(GeneralException &e)
    {
      DropLastError("processIncomingPackets failed: %s", e.getMessage());
    }
  }
  //------------------------------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------------------------------
  void DirectIP::initialize()
  {
    showSettingsDialog();

    // bind to port
    rebind();
  }
  void DirectIP::destroy()
  {
    hideSettingsDialog();
    session.release();
  }
  void DirectIP::requestAds()
  {
    rebind();
    processIncomingPackets();

    // send game state request
    char sendBufferBytes[600];
    Util::MemoryFrame sendBuffer(sendBufferBytes, 600);
    Util::MemoryFrame ping_server = sendBuffer;
    ping_server.writeAs<int>(PacketType_RequestGameStats);

    UDPAddr host;
    host.sin_family = AF_INET;
    host.sin_addr.s_addr = inet_addr(getHostIPString());
    host.sin_port = htons(atoi(getHostPortString()));
    session.sendPacket(host, sendBuffer.getFrameUpto(ping_server));
  }
  void DirectIP::sendAsyn(const UDPAddr& him, Util::MemoryFrame packet)
  {

    processIncomingPackets();

    // create header
    char sendBufferBytes[600];
    Util::MemoryFrame sendBuffer(sendBufferBytes, 600);
    Util::MemoryFrame spacket = sendBuffer;
    spacket.writeAs<int>(PacketType_GamePacket);
    spacket.write(packet);

    // send packet
    session.sendPacket(him, sendBuffer.getFrameUpto(spacket));
  }
  void DirectIP::receive()
  {
    processIncomingPackets();
  }
  void DirectIP::startAdvertising(Util::MemoryFrame ad)
  {
    rebind();
    adData = ad;
    isAdvertising = true;
  }
  void DirectIP::stopAdvertising()
  {
    isAdvertising = false;
  }
  //------------------------------------------------------------------------------------------------------------------------------------
};
