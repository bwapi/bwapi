#include "Commands.h"
#include "Connect.h"

void BroadcastAdvertisement()
{
  if ( gpGameAdvert )
  {
    WORD wBCSize = ((broadcastPkt*)gpGameAdvert)->wSize;
    sendto(gsBroadcast, (char*)gpGameAdvert, wBCSize, 0, &gaddrBroadcast, sizeof(SOCKADDR));
    ++gdwSendCalls;
    gdwSendBytes += wBCSize;
  }
}

void BroadcastGameListRequest()
{
  broadcastPkt pkt;
  pkt.wSize       = sizeof(broadcastPkt);
  pkt.wType       = 2;
  pkt.wReserved   = 0;
  pkt.dwVersion   = gdwVerbyte;
  pkt.dwProduct   = gdwProduct;
  pkt.dwGameState = 0;

  // @TODO: Checksum
  pkt.wChecksum   = 0;

  sendto(gsBroadcast, (char*)&pkt, sizeof(broadcastPkt), 0, &gaddrBroadcast, sizeof(SOCKADDR));
  ++gdwSendCalls;
  gdwSendBytes += sizeof(broadcastPkt);
}
