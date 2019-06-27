#include"BWAPIProtoClient.h"

namespace BWAPI
{
  BWAPIProtoClient::BWAPIProtoClient() :mt(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()))
    {
        connected = false;
        udpbound = false;
        connectionPort = 1025;
    }

  void BWAPIProtoClient::checkForConnection(uint32_t apiVersion, std::string enginetype, std::string engineVersion)
  {
    if (isConnected())
      return;
    sf::Packet packet;
    udpSocket.setBlocking(false);
    if (!udpbound)
    {
      if (udpSocket.bind(1024, sf::IpAddress::Any) != sf::Socket::Status::Done)
        return;
      udpbound = true;
    }

    auto sender = sf::IpAddress::Any;
    auto port = udpSocket.getLocalPort();
    if (udpSocket.receive(packet, sender, port) != sf::Socket::Done)
      return;

    auto size = packet.getDataSize();
    std::unique_ptr<char[]> packetContents(new char[size]);
    memcpy(packetContents.get(), packet.getData(), size);

    auto currentMessage = std::make_unique<bwapi::message::Message>();
    currentMessage->ParseFromArray(packetContents.get(), static_cast<int>(size));

    if (!currentMessage->has_initbroadcast())
      return;

    auto reply = std::make_unique<bwapi::message::Message>();
    auto initResponse = reply->mutable_initresponse();

    initResponse->set_port(static_cast<int>(connectionPort));

    
    packet.clear();
    size = reply->ByteSize();
    std::unique_ptr<char[]> buffer(new char[size]);

    reply->SerializeToArray(&buffer[0], size);
    packet.append(buffer.get(), size);

    udpSocket.send(packet, sender, port);
    udpSocket.unbind();
    udpbound = false;

    tcpListener.accept(tcpSocket);
    return;
  }

  void BWAPI::BWAPIProtoClient::lookForServer(int apiversion, std::string bwapiversion, bool tournament)
  {
    if (isConnected())
      return;

    sf::Packet packet;

    auto broadcastMessage = std::make_unique<bwapi::message::Message>();
    auto initResponse = broadcastMessage->mutable_initbroadcast();

    auto size = broadcastMessage->ByteSize();
    std::unique_ptr<char[]> buffer(new char[size]);

    broadcastMessage->SerializeToArray(&buffer[0], size);
    packet.append(buffer.get(), size);

    sf::IpAddress server = sf::IpAddress::Broadcast;
    unsigned short port = 1024;
    
    udpSocket.send(packet, server, port);
    server = sf::IpAddress::Any;
    udpSocket.receive(packet, server, port);

    size = packet.getDataSize();
    std::unique_ptr<char[]> replyBuffer(new char[size]);
    memcpy(replyBuffer.get(), packet.getData(), size);

    auto currentMessage = std::make_unique<bwapi::message::Message>();
    currentMessage->ParseFromArray(replyBuffer.get(), size);

    if (!currentMessage->has_initresponse())
      return;

    connectionPort = static_cast<unsigned short>(currentMessage->initresponse().port());



    tcpSocket.connect(server, connectionPort);
    if (tcpSocket.getRemoteAddress() == sf::IpAddress::None)
      fprintf(stderr, "%s", "Connection failed.\n");
  }

  void BWAPIProtoClient::transmitMessages()
  {
    //Check that we are connected to a game server.
    if (!isConnected())
        return;
    std::unique_ptr<bwapi::message::Message> currentMessage;
    sf::Packet packet;
    //loop until the message queue is empty.
    while (messageQueue.size())
    {
      packet.clear();
      currentMessage = std::move(messageQueue.front());
      messageQueue.pop_front();
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
    //Finished with queue, send the EndOfQueue message
    auto endOfQueue = std::make_unique<bwapi::game::EndOfQueue>();
    currentMessage = std::make_unique<bwapi::message::Message>();
    currentMessage->set_allocated_endofqueue(endOfQueue.release());
    packet.clear();
    auto size = currentMessage->ByteSize();
    std::unique_ptr<char[]> buffer(new char[size]);
    currentMessage->SerializeToArray(&buffer[0], size);
    packet.append(buffer.get(), size);
    if (tcpSocket.send(packet) != sf::Socket::Done)
    {
      //Error sending EndofQueue
      fprintf(stderr, "Failed to send end of queue command.");
    }
  }

  void BWAPIProtoClient::receiveMessages()
  {
    //Check that we are connected to a game server or client.
    if (!isConnected())
      return;
    std::unique_ptr<bwapi::message::Message> currentMessage;
    sf::Packet packet;
    //loop until the end of queue message is received.
    while (true)
    {
      packet.clear();
      currentMessage = std::make_unique<bwapi::message::Message>();
      if (tcpSocket.receive(packet) != sf::Socket::Done)
      {
        fprintf(stderr, "Failed to receive messages.\n");
        return;
      }
      auto size = packet.getDataSize();
      std::unique_ptr<char[]> packetContents(new char[size]);
      memcpy(packetContents.get(), packet.getData(), size);
      currentMessage->ParseFromArray(packetContents.get(), packet.getDataSize());
      if (currentMessage->has_endofqueue())
        return;
      if (currentMessage->has_frameupdate())
        std::cout << packet.getDataSize() << std::endl;
      messageQueue.push_back(std::move(currentMessage));
    }
  }

  void BWAPIProtoClient::queueMessage(std::unique_ptr<bwapi::message::Message> newMessage)
  {
    messageQueue.push_back(std::move(newMessage));
  }
    
  bool BWAPIProtoClient::isConnected() const
  {
    return tcpSocket.getRemoteAddress() != sf::IpAddress::None;
  }

  int BWAPIProtoClient::messageQueueSize() const
  {
    return messageQueue.size();
  }

  std::unique_ptr<bwapi::message::Message> BWAPIProtoClient::getNextMessage()
  {
    //if (!messageQueue.size())
    //  return std::make_unique<bwapi::message::Message>(nullptr);
    auto nextMessage = std::move(messageQueue.front());
    messageQueue.pop_front();
    return nextMessage;
  }
  void BWAPIProtoClient::disconnect()
  {
    if (!isConnected())
      return;
    tcpSocket.disconnect();
  }
  void BWAPIProtoClient::initListen()
  {
    tcpListener.setBlocking(true);
    while (tcpListener.listen(connectionPort) != sf::Socket::Done)
      connectionPort = static_cast<unsigned short>(getRandomInteger(1025, 49151));
  }
  void BWAPIProtoClient::stopListen()
  {
    tcpListener.close();
  }
}