#pragma once

#include<iostream>
#include<deque>
#include<SFML\Network.hpp>
#include<fstream>
#include<random>
#include<chrono>

#include "..\Messages\generated\cpp\message.pb.h"

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
        int messageQueueSize() const;

    private:
      sf::UdpSocket udpSocket;
        sf::TcpSocket tcpSocket;
        sf::TcpListener tcpListener;
        sf::SocketSelector socketSelector;
        std::deque<std::unique_ptr<bwapi::message::Message>> messageQueue;
        std::mt19937 mt;
        
        int getRandomInteger(int min, int max) {
          std::uniform_int_distribution<int> random_number(min, max);
          return random_number(mt);
        }

        bool connected;
        bool udpbound;
        unsigned short connectionPort;
    };
}