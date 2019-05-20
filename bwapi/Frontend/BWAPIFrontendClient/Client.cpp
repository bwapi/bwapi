#include "Client.h"
#include <windows.h>
#include <sstream>
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

#undef max

namespace BWAPI
{
  //Client BWAPIClient;
  Client::Client()
  {}
  Client::~Client()
  {
    this->disconnect();
  }
  bool Client::isConnected() const
  {
    return protoClient.isConnected();
  }
  bool Client::connect()
  {
    if ( protoClient.isConnected() )
    {
      std::cout << "Already connected." << std::endl;
      return true;
    }

    while (true)
    {
      std::cout << "Connecting..." << std::endl;
      protoClient.lookForServer(0, "x", false);
      if (protoClient.isConnected())
        return true;
    }
    return protoClient.isConnected();
  }
  void Client::disconnect()
  {
    if (!connected)
      return;
    protoClient.disconnect();

  }
  void Client::queueMessage(std::unique_ptr<bwapi::message::Message> message)
  {
    protoClient.queueMessage(std::move(message));
  }
  void Client::update(Game& game)
  {
    protoClient.transmitMessages();
    protoClient.receiveMessages();
    while (protoClient.messageQueueSize())
    {
      auto message = protoClient.getNextMessage();
      if (message->has_frameupdate())
      {
        //update game here
        if (message->frameupdate().has_gamedata())
        {
          game.gameData->isInGame = message->frameupdate().gamedata().isingame();
        }
      }
      if (message->has_endofqueue())
        return;
    }
  }
  void Client::onMatchFrame(Game& game)
  {
  }
  void Client::clearAll()
  {
  }
  void Client::onMatchStart(Game& game)
  {
  }
  void Client::initForces(Game& game)
  {
  }
  void Client::initPlayers(Game& game)
  {
  }
  void Client::initInitialUnits(Game& game)
  {
  }
  void Client::initRegions(Game& game)
  {
  }
  void Client::onMatchEnd(Game& game)
  {
  }

  void Client::initGame(Game& game)
  {
  }
  void Client::updateGame(Game& game)
  {
  }
}
