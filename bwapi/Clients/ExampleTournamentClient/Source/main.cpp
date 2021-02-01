#include <BWAPI.h>
#include "..\Frontend\BWAPIFrontendClient\ProtoClient.h"

#include <iostream>

using namespace BWAPI;

int main(int argc, const char* argv[])
{
  BWAPIProtoClient connectionToClient;
  BWAPIProtoClient connectionToServer;
  std::cout << "Connecting to server..." << std::endl;
  while (!connectionToServer.isConnected())
  {
    connectionToServer.lookForServer(0, "x", false);
  }
  std::cout << "Connected to server!" << std::endl;

  std::cout << "Waiting for client to connect." << std::endl;
  connectionToClient.initListen();
  while (!connectionToClient.isConnected())
  {
    connectionToClient.checkForConnection(0, "x", "x");
  }
  connectionToClient.stopListen();

  while (true)
  {
    connectionToServer.receiveMessages();
    while (connectionToServer.messageQueueSize())
      connectionToClient.queueMessage(connectionToServer.getNextMessage());
    connectionToClient.transmitMessages();
    connectionToClient.receiveMessages();
    while (connectionToClient.messageQueueSize())
      connectionToServer.queueMessage(connectionToClient.getNextMessage());
    connectionToServer.transmitMessages();
  }

  return 0;
}