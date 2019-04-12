#pragma once

#include<iostream>
#include<deque>
#include<SFML\Network.hpp>

#include "..\Messages\generated\cpp\message.pb.h"

namespace BWAPI
{
    class BWAPIProtoClient
    {
    public:
        BWAPIProtoClient();
        ~BWAPIProtoClient();

        void checkForConnection(uint32_t apiVersion, std::string enginetype, std::string engineVersion);
        void lookForServer(std::string* allocatedbwapiversion, int apiversion, char* bwapiversion, bool tournament);
        void transmitMessages();
        void receiveMessages();

        void queueMessage(std::unique_ptr<bwapi::message::Message> newMessage);
        //void queueMessage(bwapi::message::Message *newMessage) const;


    private:
        sf::TcpSocket tcpSocket;
        sf::TcpListener tcpListener;
        sf::SocketSelector socketSelector;
        std::deque<std::unique_ptr<bwapi::message::Message>> messageQueue;

        bool connected;
    };
}