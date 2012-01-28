#include <stdio.h>

#include <BWAPI.h>
#include <BWAPI/Client.h>

#include <windows.h>

#include <string>

using namespace BWAPI;

void reconnect()
{
  while(!BWAPIClient.connect())
  {
    Sleep(1000);
  }
}
int main(int argc, const char* argv[])
{
  const char* szDllPath = "";
  if (argc>=2)
    szDllPath = argv[1];
  else
  {
    char buffer[1024];
    szDllPath = buffer;
    printf("Enter path name to AI DLL:");
    scanf("%s",buffer);
  }
  BWAPI::BWAPI_init();
  printf("Connecting...");
  reconnect();
  while(true)
  {
    printf("waiting to enter match\n");
    while (!Broodwar->isInGame())
    {
      BWAPI::BWAPIClient.update();
      if (!BWAPI::BWAPIClient.isConnected())
      {
        printf("Reconnecting...\n");
        reconnect();
      }
    }
    AIModule* client = NULL;
    HMODULE hMod = LoadLibraryA(szDllPath);
    if (hMod == NULL)
    {
      printf("ERROR: Failed to load the AI Module\n");
      client = new AIModule();
      Broodwar->sendText("Error: Failed to load the AI Module");
    }
    else
    {
      typedef AIModule* (*PFNCreateA1)(BWAPI::Game*);
      PFNCreateA1 newAIModule = (PFNCreateA1)GetProcAddress(hMod, LPCSTR("newAIModule"));
      client = newAIModule(Broodwar);
    }
    printf("starting match!\n");
    while(Broodwar->isInGame())
    {
      for(std::list<Event>::iterator e=Broodwar->getEvents().begin();e!=Broodwar->getEvents().end();e++)
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
          default:
          break;
        }
      }
      BWAPI::BWAPIClient.update();
      if (!BWAPI::BWAPIClient.isConnected())
      {
        printf("Reconnecting...\n");
        reconnect();
      }
    }
    delete client;
    FreeLibrary(hMod);
    printf("Game ended\n");
  }
  system("pause");
  return 0;
}

