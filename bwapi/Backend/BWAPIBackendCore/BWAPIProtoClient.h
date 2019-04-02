#pragma once

#include<deque>

#include "..\Messages\generated\cpp\message.pb.h"
#include "..\Messages\generated\cpp\test.pb.h"

namespace BWAPI
{
    class BWAPIProtoClient
    {
    public:
        BWAPIProtoClient();
        ~BWAPIProtoClient();

        void listenForConnection();
        void lookForServer(std::string* allocatedbwapiversion, int apiversion, char* bwapiversion, bool tournament);


    private:
        //easywsclient::WebSocket::pointer connection;
        std::deque<bwapi::message::Message*> messageQueue;

        bool connected;
    };
}