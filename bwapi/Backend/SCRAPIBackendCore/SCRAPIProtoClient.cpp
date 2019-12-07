#include"SCRAPIProtoClient.h"

#include<SFML/Network.hpp>

namespace BWAPI
{
  SCRAPIProtoClient::SCRAPIProtoClient() : mt(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()))
  {
    connected = false;
    udpbound = false;
    remaster = false;
    connectionPort = 1025;
  }

  void BWAPI::SCRAPIProtoClient::lookForServer(int apiversion, std::string bwapiversion, bool tournament)
  {
    if (isConnected())
      return;

    sf::Packet packet;
    bool skip = false;
    //Look for a 1.16.1 backend first.

    //Look for a Remaster backend
    auto broadcastMessage = std::make_unique<SCRAPIProtocol::Request>();

    broadcastMessage->mutable_connect();

    auto size = broadcastMessage->ByteSize();
    std::unique_ptr<char[]> buffer(new char[size]);

    broadcastMessage->SerializeToArray(&buffer[0], size);
    packet.append(buffer.get(), size);

    sf::IpAddress server = sf::IpAddress::Broadcast;
    unsigned short port = 1024;

    udpSocket.send(packet, server, port);
    server = sf::IpAddress::Any;
    udpSocket.setBlocking(false);

    // Sleep to give Remaster a chance to send the packet.
    {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(2s);
    }

    if (udpSocket.receive(packet, server, port) != sf::Socket::Done)
      return;

    size = packet.getDataSize();
    std::unique_ptr<char[]> replyBuffer(new char[size]);
    memcpy(replyBuffer.get(), packet.getData(), size);

    auto currentMessage = std::make_unique<SCRAPIProtocol::Response>();
    currentMessage->ParseFromArray(replyBuffer.get(), size);

    if (!currentMessage->has_connect())
      return;

    connectionPort = 8999;
    tcpSocket.connect(server, connectionPort);
    if (tcpSocket.getRemoteAddress() == sf::IpAddress::None)
      fprintf(stderr, "%s", "Connection failed.\n");
    remaster = true;
  }

  void SCRAPIProtoClient::transmitMessages()
  {
    //Check that we are connected to a game server.
    if (!isConnected())
      return;
    
    std::unique_ptr<SCRAPIProtocol::Request> currentMessage;
    sf::Packet packet;
    //loop until the message queue is empty.
    while (!requestQueue.empty())
    {
      packet.clear();
      currentMessage = std::move(requestQueue.front());
      requestQueue.pop_front();
      auto size = currentMessage->ByteSize();
      std::unique_ptr<char[]> buffer(new char[size]);
      currentMessage->SerializeToArray(&buffer[0], size);
      packet.append(buffer.get(), size);
      if (tcpSocket.send(packet) != sf::Socket::Done)
      {
        //Error sending command, we should do something here?
        fprintf(stderr, "Failed to send a Message.\n");
      }
    }

    //Finished with queue, send the EndOfQueue message.
    auto endOfQueue = std::make_unique<SCRAPIProtocol::Request>();
    endOfQueue->mutable_end_of_queue();
    packet.clear();
    auto size = endOfQueue->ByteSize();
    std::unique_ptr<char[]> buffer(new char[size]);
    endOfQueue->SerializeToArray(&buffer[0], size);
    packet.append(buffer.get(), size);
    if (tcpSocket.send(packet) != sf::Socket::Done)
    {
      //Error sending EndofQueue
      fprintf(stderr, "Failed to send end of queue command.");
    }
  }

  void SCRAPIProtoClient::receiveMessages()
  {
    //Check that we are connected to a game server or client.
    if (!isConnected())
      return;

    sf::Packet packet;
    //loop until the end of queue message is received.
    while (true)
    {
      packet.clear();
      auto currentMessage = std::make_unique<SCRAPIProtocol::Response>();
      if (tcpSocket.receive(packet) != sf::Socket::Done)
      {
        fprintf(stderr, "Failed to receive messages.\n");
        return;
      }
      auto size = packet.getDataSize();
      std::unique_ptr<char[]> packetContents(new char[size]);
      memcpy(packetContents.get(), packet.getData(), size);
      currentMessage->ParseFromArray(packetContents.get(), packet.getDataSize());
      if (currentMessage->has_end_of_queue())
        return;
      responseQueue.push_back(std::move(currentMessage));
    }
  }

  void SCRAPIProtoClient::queueMessage(std::unique_ptr<SCRAPIProtocol::Request> newMessage)
  {
    requestQueue.push_back(std::move(newMessage));
  }

  bool SCRAPIProtoClient::isConnected() const
  {
    return tcpSocket.getRemoteAddress() != sf::IpAddress::None;
  }

  int SCRAPIProtoClient::responseQueueSize() const
  {
    return responseQueue.size();
  }

  std::unique_ptr<SCRAPIProtocol::Response> SCRAPIProtoClient::getNextResponse()
  {
    auto nextMessage = std::move(responseQueue.front());
    responseQueue.pop_front();
    return nextMessage;
  }

  void SCRAPIProtoClient::disconnect()
  {
    if (!isConnected())
      return;
    tcpSocket.disconnect();
  }
  void SCRAPIProtoClient::initListen()
  {
    tcpListener.setBlocking(true);
    while (tcpListener.listen(connectionPort) != sf::Socket::Done)
      connectionPort = static_cast<unsigned short>(getRandomInteger(1025, 49151));
  }
  void SCRAPIProtoClient::stopListen()
  {
    tcpListener.close();
  }
}