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
    
    game.flush();
    protoClient.transmitMessages();
    protoClient.receiveMessages();
    while (protoClient.messageQueueSize())
    {
      auto message = protoClient.getNextMessage();
      if (message->has_frameupdate())
      {
        //update game here
        if (message->frameupdate().has_game())
        {
          auto gameMessage = message->frameupdate().game();
          if (gameMessage.has_gamedata())
          {
            auto gameUpdate = gameMessage.gamedata();
            game.gameData->isInGame = gameUpdate.isingame();
            game.gameData->randomSeed = gameUpdate.randomseed();
          }
          for (auto& u : gameMessage.units())
          {
            bool found = false;
            UnitData newUnit(game, static_cast<UnitID>(u.id()));
            newUnit.type = static_cast<UnitType>(u.type());
            units.insert(newUnit);
            //newUnit.type = static_cast<UnitType>(u.type());
            //units.emplace(game, static_cast<UnitID>(u.id()));
            //newUnit.id
            /*for (auto itr = units.begin(); itr != units.end(); itr++)
            {
              if (itr->id == static_cast<UnitID>(u.id()))
              {
                auto unitCopy = *itr;
                unitCopy.exists = u.exists();
                unitCopy.type = static_cast<UnitType>(u.type());
                units.erase(itr);
                units.insert(unitCopy);
                itr = units.end();
                found = true;
              }
              if (!found)
              {
                UnitData newUnit(game, static_cast<UnitID>(u.id()));
                newUnit.type = static_cast<UnitType>(u.type());
                units.insert(newUnit);
              }
            }*/
          }
        }
      }
      else if (message->has_event())
      {
        Event e2;
        auto e = message->event();
        if (e.has_matchend())
        {
          e2.setType(EventType::MatchEnd);
          e2.setWinner(e.matchend().winner());
          game.addEvent(e2);
        }
        else if (e.has_sendtext())
        {
          e2.setType(EventType::SendText);
          e2.setText(e.sendtext().text().c_str());
          game.addEvent(e2);
        }
        else if (e.has_savegame())
        {
          e2.setType(EventType::SaveGame);
          e2.setText(e.savegame().text().c_str());
          game.addEvent(e2);
        }
        else if (e.has_playerleft())
        {
          e2.setType(EventType::PlayerLeft);
          e2.setPlayer(game.getPlayer(PlayerID{ e.playerleft().player() }));
          game.addEvent(e2);
        }
        else if (e.has_receivetext())
        {
          e2.setType(EventType::ReceiveText);
          e2.setText(e.receivetext().text().c_str());
          game.addEvent(e2);
        }
      }
      else if (message->has_endofqueue())
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
