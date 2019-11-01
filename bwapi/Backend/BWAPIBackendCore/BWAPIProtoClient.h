#pragma once

#include<iostream>
#include<deque>
#include<fstream>
#include<random>
#include<chrono>

#include<SFML/Network.hpp>

// Protobuf headers have some warnings we don't care about
#pragma warning(push, 0)
#include "..\Messages\generated\cpp\message.pb.h"
#include "..\..\SCRAPIMessages\generated\cpp\scrapi.pb.h"
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
        void queueMessage(std::unique_ptr<SCRAPIProtocol::Request> newMessage);
        std::unique_ptr<bwapi::message::Message> getNextMessage();
        std::unique_ptr<SCRAPIProtocol::Response> getNextResponse();
        
        bool isConnected() const;
        bool isRemaster() const;
        int messageQueueSize() const;
        int responseQueueSize() const;

    private:
      sf::UdpSocket udpSocket;
      sf::TcpSocket tcpSocket;
      sf::TcpListener tcpListener;
      sf::SocketSelector socketSelector;
      std::deque<std::unique_ptr<bwapi::message::Message>> messageQueue;
      std::deque<std::unique_ptr<SCRAPIProtocol::Request>> requestQueue;
      std::deque<std::unique_ptr<SCRAPIProtocol::Response>> responseQueue;
      std::mt19937 mt;
        
      int getRandomInteger(int min, int max) {
        std::uniform_int_distribution<int> random_number(min, max);
        return random_number(mt);
      }

      bool connected;
      bool udpbound;
      bool remaster;
      unsigned short connectionPort;
    };
}