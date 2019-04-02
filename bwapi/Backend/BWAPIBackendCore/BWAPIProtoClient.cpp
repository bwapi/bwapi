#include"BWAPIProtoClient.h"

namespace BWAPI
{
    BWAPIProtoClient::BWAPIProtoClient()
    {
        connected = false;
        connection = nullptr;
    }

    void BWAPIProtoClient::listenForConnection()
    {
        auto onDataRecieved = [=](const std::string& message)
        {
            bwapi::message::Message* response = new bwapi::message::Message();
            response->ParseFromString(message);
            if (response->has_initbroadcast())
            {
                bwapi::init::ServerResponse* serverResponse = new bwapi::init::ServerResponse();
                serverResponse->set_apiversion(5000);
                bwapi::message::Message* broadcast = new bwapi::message::Message();
                broadcast->set_allocated_initresponse(serverResponse);
                std::string serverResponseString;
                broadcast->SerializeToString(&serverResponseString);
                connection->send(serverResponseString);
            }
        };

        if (connection == nullptr)
        {
            std::string url = "ws://localhost:8045/bwapi";
            //connection = easywsclient::WebSocket::from_url(url);
        }
        while (!connected)
        {
            bwapi::message::Message* response = new bwapi::message::Message();
            connection->poll();
            connection->dispatch(onDataRecieved);
        }
    }

    void BWAPI::BWAPIProtoClient::lookForServer(std::string* allocatedbwapiversion, int apiversion, char* bwapiversion, bool tournament)
    {
        if (connection == nullptr)
        {
            std::string url = "ws://localhost:8045/bwapi";
            //connection = easywsclient::WebSocket::from_url(url);
        }
        while (!connected)
        {
            bwapi::init::ClientBroadcast* broadcast = new bwapi::init::ClientBroadcast;
            broadcast->set_allocated_bwapiversion(allocatedbwapiversion);
            broadcast->set_apiversion(apiversion);
            broadcast->set_bwapiversion(bwapiversion);
            broadcast->set_tournament(tournament);
            bwapi::message::Message* message = new bwapi::message::Message();
            message->set_allocated_initbroadcast(broadcast);
            std::string messageString;
            message->SerializeToString(&messageString);
            connection->send(messageString);

            connection->poll(5000);
            //connection
        }
    }
}