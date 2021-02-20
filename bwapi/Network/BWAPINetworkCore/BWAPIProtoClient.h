#pragma once

#include <deque>
#include <random>

#include <SFML/Network.hpp>

// Protobuf headers have some warnings we don't care about
#pragma warning(push, 0)
#include <generated/cpp/message.pb.h>
#pragma warning(pop)


namespace BWAPI
{
  class BWAPIProtoClient
  {
  public:
    BWAPIProtoClient();

    void checkForConnection(uint32_t apiVersion, std::string enginetype, std::string engineVersion);
    void lookForServer(int apiversion, std::string bwapiversion, bool tournament);
    void transmitMessages();
    void receiveMessages();
    void disconnect();
    void initListen();
    void stopListen();

    void queueMessage(std::unique_ptr<bwapi::message::Message> newMessage);
    std::unique_ptr<bwapi::message::Message> getNextMessage();
        
    bool isConnected() const;
    size_t messageQueueSize() const;

  private:
    sf::UdpSocket udpSocket;
    sf::TcpSocket tcpSocket;
    sf::TcpListener tcpListener;
    std::deque<std::unique_ptr<bwapi::message::Message>> messageQueue;
    std::mt19937 mt;
        
    int getRandomInteger(int min, int max) {
      std::uniform_int_distribution<int> random_number(min, max);
      return random_number(mt);
    }

    static std::unique_ptr<bwapi::message::Message> messageFromPacket(const sf::Packet& packet) {
      auto message = std::make_unique<bwapi::message::Message>();
      message->ParseFromArray(packet.getData(), static_cast<int>(packet.getDataSize()));
      return message;
    }

    static void appendMessageToPacket(const std::unique_ptr<bwapi::message::Message>& message, sf::Packet& packet) {
      static thread_local std::vector<char> buffer;
      auto size = message->ByteSizeLong();

      buffer.clear();
      buffer.resize(size);

      message->SerializeToArray(buffer.data(), size);
      packet.append(buffer.data(), size);
    }

    bool connected;
    bool udpbound;
    unsigned short connectionPort;
  };
}