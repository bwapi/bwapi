#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <string>

#include <Windows.h>

#include <BWAPI/Client.h>
#include <BWAPI.h>


using namespace BWAPI;

void reconnect()
{
  while (!BWAPIClient.connect())
    std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
}

int main(int argc, const char* argv[])
{
  std::string dllPath;

  if ( argc >= 2 )
  {
    dllPath = argv[1];
  }
  else
  {
    std::cout << "Enter path name to AI DLL: " << std::endl;
    std::getline(std::cin, dllPath);
  }
  
  std::cout << "Connecting..." << std::endl;

  assert(BWAPIClient.isConnected() == false);
  reconnect();
  assert(BroodwarPtr != nullptr);

  while( true )
  {
    std::cout << "waiting to enter match" << std::endl;
    while ( !Broodwar->isInGame() )
    {
      //std::cout << "attempting update" << std::endl;
      BWAPI::BWAPIClient.update();
      if (!BWAPI::BWAPIClient.isConnected())
      {
        std::cout << "Reconnecting..." << std::endl;
        reconnect();
      }
    }
    std::cout << "entered match" << std::endl;

    AIModule* client = NULL;
    HMODULE hMod = LoadLibraryA(dllPath.c_str());
    if (hMod == NULL)
    {
      std::cerr << "ERROR: Failed to load the AI Module" << std::endl;
      client = new AIModule();
      Broodwar->sendText("Error: Failed to load the AI Module");
    }
    else
    {
      typedef AIModule* (*PFNCreateA1)();
      typedef void (*PFNGameInit)(Game *);

      PFNGameInit newGame = (PFNGameInit)GetProcAddress(hMod, "gameInit");
      PFNCreateA1 newAIModule = (PFNCreateA1)GetProcAddress(hMod, "newAIModule");

      if ( !newGame || !newAIModule )
      {
        std::cerr << "ERROR: Failed to find AI Module exports" << std::endl;
        client = new AIModule();
        Broodwar->sendText("Error: Failed to find AI Module exports");
      }
      else
      {
        newGame(BroodwarPtr);
        client = newAIModule();
      }
    }
    std::cout << "starting match!" << std::endl;
    while ( Broodwar->isInGame() )
    {
      for ( std::list<Event>::const_iterator e = Broodwar->getEvents().begin(); e != Broodwar->getEvents().end(); ++e )
      {
        EventType::Enum et=e->getType();
        switch (et)
        {
          case EventType::MatchStart:
            client->onStart();
            break;
          case EventType::MatchEnd:
            client->onEnd(e->isWinner());
            break;
          case EventType::MatchFrame:
            client->onFrame();
            break;
          case EventType::MenuFrame:
            break;
          case EventType::SendText:
            client->onSendText(e->getText());
            break;
          case EventType::ReceiveText:
            client->onReceiveText(e->getPlayer(), e->getText());
            break;
          case EventType::PlayerLeft:
            client->onPlayerLeft(e->getPlayer());
            break;
          case EventType::NukeDetect:
            client->onNukeDetect(e->getPosition());
            break;
          case EventType::UnitDiscover:
            client->onUnitDiscover(e->getUnit());
            break;
          case EventType::UnitEvade:
            client->onUnitEvade(e->getUnit());
            break;
          case EventType::UnitShow:
            client->onUnitShow(e->getUnit());
            break;
          case EventType::UnitHide:
            client->onUnitHide(e->getUnit());
            break;
          case EventType::UnitCreate:
            client->onUnitCreate(e->getUnit());
            break;
          case EventType::UnitDestroy:
            client->onUnitDestroy(e->getUnit());
            break;
          case EventType::UnitMorph:
            client->onUnitMorph(e->getUnit());
            break;
          case EventType::UnitRenegade:
            client->onUnitRenegade(e->getUnit());
            break;
          case EventType::SaveGame:
            client->onSaveGame(e->getText());
            break;
          case EventType::UnitComplete:
            client->onUnitComplete(e->getUnit());
            break;
          default:
            break;
        }
      }
      BWAPI::BWAPIClient.update();
      if (!BWAPI::BWAPIClient.isConnected())
      {
        std::cout << "Reconnecting..." << std::endl;
        reconnect();
      }
    }
    delete client;
    FreeLibrary(hMod);
    std::cout << "Game ended" << std::endl;
  }
  std::cout << "Press ENTER to continue..." << std::endl;
  std::cin.ignore();
  return 0;
}

