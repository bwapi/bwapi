#include"BWAPIProtoClient.h"

#include<SFML/Network.hpp>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

namespace BWAPI
{
  BWAPIProtoClient::BWAPIProtoClient()
    : mt(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()))
    , connected(false)
    , udpbound(false)
    , connectionPort(1025)
  {
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

    auto currentMessage = messageFromPacket(packet);
    if (!currentMessage->has_initbroadcast())
      return;

    auto reply = std::make_unique<bwapi::message::Message>();
    auto initResponse = reply->mutable_initresponse();

    initResponse->set_port(static_cast<int>(connectionPort));

    packet.clear();
    appendMessageToPacket(reply, packet);

    udpSocket.send(packet, sender, port);
    udpSocket.unbind();
    udpbound = false;

    tcpListener.accept(tcpSocket);
    return;
  }

  void BWAPI::BWAPIProtoClient::lookForServer(int apiversion, std::string bwapiversion, bool tournament)
  {
    using namespace std::chrono_literals;

    if (isConnected())
      return;

    sf::Packet packet;
    //Look for a 1.16.1 backend first.
    auto broadcastMessage = std::make_unique<bwapi::message::Message>();
    broadcastMessage->mutable_initbroadcast();

    appendMessageToPacket(broadcastMessage, packet);

    sf::IpAddress server = sf::IpAddress::Broadcast;
    unsigned short port = 1024;

    udpSocket.send(packet, server, port);
    server = sf::IpAddress::Any;
    udpSocket.setBlocking(false);

    // Sleep to give 1.16.1 a chance to send the packet.
    std::this_thread::sleep_for(2s);

    if (udpSocket.receive(packet, server, port) == sf::Socket::Done)
    {
      auto currentMessage = messageFromPacket(packet);

      if (!currentMessage->has_initresponse())
        return;

      connectionPort = static_cast<unsigned short>(currentMessage->initresponse().port());

      tcpSocket.connect(server, connectionPort);
      if (tcpSocket.getRemoteAddress() == sf::IpAddress::None)
        std::cerr << "Connection failed." << std::endl;
    }
  }

  void BWAPIProtoClient::transmitMessages()
  {
    //Check that we are connected to a game server.
    if (!isConnected())
      return;

    std::unique_ptr<bwapi::message::Message> currentMessage;
    sf::Packet packet;
    //loop until the message queue is empty.
    while (!messageQueue.empty())
    {
      packet.clear();
      currentMessage = std::move(messageQueue.front());
      messageQueue.pop_front();

      if (currentMessage->ByteSizeLong() > 0) {
        appendMessageToPacket(currentMessage, packet);
      }
      if (tcpSocket.send(packet) != sf::Socket::Done)
      {
        //Error sending command, we should do something here?
        std::cerr << "Failed to send a Message." << std::endl;
      }
    }

    //Finished with queue, send the EndOfQueue message
    currentMessage = std::make_unique<bwapi::message::Message>();
    currentMessage->mutable_endofqueue();
    packet.clear();

    appendMessageToPacket(currentMessage, packet);
    if (tcpSocket.send(packet) != sf::Socket::Done)
    {
      //Error sending EndofQueue
      std::cerr << "Failed to send end of queue command." << std::endl;
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
      if (tcpSocket.receive(packet) != sf::Socket::Done)
      {
        std::cerr << "Failed to receive messages." << std::endl;
        return;
      }

      currentMessage = messageFromPacket(packet);
      if (currentMessage->has_endofqueue())
        return;
      //if (currentMessage->has_frameupdate())
      //  std::cout << packet.getDataSize() << std::endl;
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

  size_t BWAPIProtoClient::messageQueueSize() const
  {
    return messageQueue.size();
  }

  std::unique_ptr<bwapi::message::Message> BWAPIProtoClient::getNextMessage()
  {
    //if (!messageQueue.size())
    //  return std::make_unique<bwapi::message::Message>(nullptr);
    std::unique_ptr<bwapi::message::Message> nextMessage = std::move(messageQueue.front());
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