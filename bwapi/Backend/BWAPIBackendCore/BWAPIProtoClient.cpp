#include"BWAPIProtoClient.h"

namespace BWAPI
{
    BWAPIProtoClient::BWAPIProtoClient()
    {
        connected = false;
        tcpListener.listen(8045);
        socketSelector.add(tcpListener);
    }

    void BWAPIProtoClient::checkForConnection(uint32_t apiVersion, std::string enginetype, std::string engineVersion)
    {
        if (!socketSelector.isReady(tcpListener))
        {
            return;
        }
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
        
    }

    void BWAPI::BWAPIProtoClient::lookForServer(std::string* allocatedbwapiversion, int apiversion, char* bwapiversion, bool tournament)
    {
        if (tcpSocket.connect("127.0.0.1", 8045) != sf::Socket::Done)
        {
            std::fprintf(stderr,"%s", "Could not connect to server.");
            return;
        }
        bwapi::init::ClientBroadcast broadcast;
        broadcast.set_allocated_bwapiversion(allocatedbwapiversion);
        broadcast.set_apiversion(apiversion);
        broadcast.set_bwapiversion(bwapiversion);
        broadcast.set_tournament(tournament);
        bwapi::message::Message message;
        message.set_allocated_initbroadcast(&broadcast);
        std::string packetContents;
        message.SerializeToString(&packetContents);
        sf::Packet packet;
        packet << packetContents;
        tcpSocket.send(packet);

        if (tcpSocket.receive(packet) != sf::Socket::Done)
        {
            std::fprintf(stderr, "%s", "Failed to receive server response.");
            tcpSocket.disconnect();
            return;
        }
        packet >> packetContents;
        message.ParseFromString(packetContents);
        if (!message.has_initresponse)
        {
            std::fprintf(stderr, "Unexpected server response.");
            tcpSocket.disconnect();
            return;
        }
        //What are we going to do with this?
        bwapi::init::ServerResponse serverResponse = message.initresponse;
        //we are technically connected.
    }
}