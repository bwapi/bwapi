#include"BWAPIProtoClient.h"

namespace BWAPI
{
    BWAPIProtoClient::BWAPIProtoClient()
    {
        connected = false;
        //tcpListener.listen(8045, "127.0.0.1");
        socketSelector.add(tcpListener);
    }

  void BWAPIProtoClient::checkForConnection(uint32_t apiVersion, std::string enginetype, std::string engineVersion)
  {
    tcpListener.listen(8045, "127.0.0.1");
    //if (!socketSelector.isReady(tcpListener))
    //{
    //    return;
    //}
    tcpSocket.setBlocking(true);
    tcpListener.accept(tcpSocket);
    sf::Packet packet;
    std::string packetContents;
    bwapi::message::Message receivedMessage;
    bwapi::message::Message sendMessage;
    tcpSocket.receive(packet);
    packet >> packetContents;
    receivedMessage.ParseFromString(packetContents);
    bwapi::init::ServerResponse serverResponse;
    //Check if this is a client broadcast
    if (!receivedMessage.has_initbroadcast())
    {
      //Construct error message and send prior to disconnecting the socket
      bwapi::error::Error error;
      error.set_code(bwapi::error::ErrorCode::REFUSED);
      error.set_reason("Missing Init Broadcast.");
      serverResponse.set_allocated_error(&error);
      sendMessage.set_allocated_initresponse(&serverResponse);
      packetContents = sendMessage.SerializeAsString();
      packet.clear();
      packet << packetContents;
      tcpSocket.send(packet);
      tcpSocket.disconnect();
    }
    serverResponse.set_apiversion(apiVersion);
    serverResponse.set_enginetype(enginetype);
    serverResponse.set_engineversion(engineVersion);
    serverResponse.set_supportedprotocols(1, bwapi::init::Protocol::PROTOBUF);
    sendMessage.set_allocated_initresponse(&serverResponse);
    packetContents = sendMessage.SerializeAsString();
    packet.clear();
    packet << packetContents;
    tcpSocket.send(packet);
    connected = true;
  }

  void BWAPI::BWAPIProtoClient::lookForServer(int apiversion, std::string bwapiversion, bool tournament)
  {
    tcpSocket.setBlocking(true);
    if (tcpSocket.connect("127.0.0.1", 8045) != sf::Socket::Done)
    {
      std::fprintf(stderr, "%s", "Could not connect to server.\n");
      return;
    }
    auto broadcast = std::make_unique<bwapi::init::ClientBroadcast>();
    broadcast->set_apiversion(apiversion);
    broadcast->set_bwapiversion(bwapiversion);
    broadcast->set_tournament(tournament);
    bwapi::message::Message message;
    message.set_allocated_initbroadcast(broadcast.release());
    std::string packetContents;
    message.SerializeToString(&packetContents);
    sf::Packet packet;
    packet << packetContents;
    if (tcpSocket.send(packet) != sf::Socket::Done)
    {
      std::fprintf(stderr, "%s", "Failed to send Client Broadcast.");
      return;
    }
      tcpSocket.setBlocking(true);
      packet.clear();
      auto status = tcpSocket.receive(packet);
      switch (status)
      {
      case sf::Socket::Status::Disconnected:
        std::fprintf(stderr, "%s", "Status: Disconnected.\n");
        break;
      case sf::Socket::Status::Done:
        std::fprintf(stderr, "%s", "Status: Done.\n");
        break;
      case sf::Socket::Status::Error:
        std::fprintf(stderr, "%s", "Status: Error.\n");
        break;
      case sf::Socket::Status::NotReady:
        std::fprintf(stderr, "%s", "Status: Not Ready.\n");
        break;
      case sf::Socket::Status::Partial:
        std::fprintf(stderr, "%s", "Status: Partial.\n");
        break;
      default:
        std::fprintf(stderr, "%s", "Status: Something else.\n");
        break;
      }
      if (status != sf::Socket::Done)
      {
          std::fprintf(stderr, "%s", "Failed to receive server response.\n");
          tcpSocket.disconnect();
          return;
      }
      packet >> packetContents;
      message.ParseFromString(packetContents);
      if (!message.has_initresponse())
      {
          std::fprintf(stderr, "Unexpected server response.");
          tcpSocket.disconnect();
          return;
      }
      //What are we going to do with this?
      bwapi::init::ServerResponse serverResponse = message.initresponse();
      //we are technically connected.
    }

    void BWAPIProtoClient::transmitMessages()
    {
        //Check that we are connected to a game server.
        if (!connected)
            return;
        std::unique_ptr<bwapi::message::Message> currentMessage;
        sf::Packet packet;
        //loop until the message queue is empty.
        while (messageQueue.size())
        {
            packet.clear();
            currentMessage = std::move(messageQueue.front());
            packet << currentMessage->SerializeAsString();
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
        packet << currentMessage->SerializeAsString();
        if (tcpSocket.send(packet) != sf::Socket::Done)
        {
          //Error sending EndofQueue
          fprintf(stderr, "Failed to send end of queue command.");
        }

    }

    void BWAPIProtoClient::receiveMessages()
    {
        //Check that we are connected to a game server or client.
        if (!connected)
            return;
        std::unique_ptr<bwapi::message::Message> currentMessage;
        sf::Packet packet;
        std::string packetContents;
        //loop until the end of queue message is received.
        while (true)
        {
            packet.clear();
            packetContents.clear();
            currentMessage = std::make_unique<bwapi::message::Message>();
            if (tcpSocket.receive(packet) != sf::Socket::Done)
            {
                fprintf(stderr, "Failed to receive messages from the server.\n");
                return;
            }
            packet >> packetContents;
            currentMessage->ParseFromString(packetContents);
            if (currentMessage->has_endofqueue())
              return;
            messageQueue.push_back(std::move(currentMessage));
        }

    }

    void BWAPIProtoClient::queueMessage(std::unique_ptr<bwapi::message::Message> newMessage)
    {
        messageQueue.push_back(std::move(newMessage));
    }
    
    bool BWAPIProtoClient::isConnected() const
    {
      return connected;
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
}