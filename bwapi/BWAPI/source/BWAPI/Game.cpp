#include "Game.h"

#include "../BWAPI/Player.h"
#include "../BWAPI/Unit.h"
#include "../BWAPI/UnitPrototype.h"
#include "../BWAPI/UnitPrototypeDefinitions.h"
#include "../BWAPI/Command.h"
#include "../BWAPI/CommandCancelTrain.h"

#include "../BW/Offsets.h"
#include "../BW/UnitData.h"
#include "../BW/OrderTypes.h"

#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>


namespace BWAPI 
{
  //------------------------------ CONSTRUCTOR ----------------------------------
  Game::Game()
  :inGame(false)
  {
    unitArrayCopy = new BW::UnitArray;
    unitArrayCopyLocal = new BW::UnitArray;

    for (int i = 0; i < 12; i++)
      players[i] = new Player((u8)i);    
    
    players[11]->setName("Player 12 (Neutral)");
    
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      units[i] = new Unit(&unitArrayCopy->unit[i], 
                          &BW::BWXFN_UnitNodeTable->unit[i],
                          &unitArrayCopyLocal->unit[i]);

    this->update();
    this->latency = 2; // @todo read from the address in update
  }
  //------------------------------- DESTRUCTOR ----------------------------------
  Game::~Game()
  {
    delete unitArrayCopy;
    delete unitArrayCopyLocal;

    for (int i = 0; i < 12; i++)
      delete players[i];

    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete units[i];
  }
  //------------------------------- ISSUE COMMAND -------------------------------
  void __fastcall Game::IssueCommand(PBYTE pbBuffer, int iSize) 
  {
     static const int BWFXNIssuseCommand = 0x4858F0;
      __asm 
        {
        MOV ECX, pbBuffer
        MOV EDX, iSize
        CALL [BWFXNIssuseCommand]
        }
     }
  //---------------------------------- UPDATE -----------------------------------
  void Game::update()
  {
    memcpy(this->unitArrayCopy, BW::BWXFN_UnitNodeTable, sizeof(BW::UnitArray));
    memcpy(this->unitArrayCopyLocal, BW::BWXFN_UnitNodeTable, sizeof(BW::UnitArray));
    for (int i = 0; i < 12; i++)
      this->players[i]->update();
    this->players[11]->setName("Player 12 (Neutral)");
    std::vector<Command *> a;
    while (this->commandBuffer.size() > latency)
      this->commandBuffer.erase(this->commandBuffer.begin());
    this->commandBuffer.push_back(a);
    for (unsigned int i = 0; i < this->commandBuffer.size(); i++)
       for (unsigned int j = 0; j < this->commandBuffer[i].size(); j++)
       {
         this->commandBuffer[i][j]->execute();
       }        
  }
  //---------------------------------- TEST -----------------------------------

  void Game::test(void)
  {
    /*for (unsigned int i = 0; i < this->buffers.size(); i++)
       delete [] buffers[i];
    buffers.clear();*/
    /*
    I will implement this later on using some correct pointers method on unit
    _w64 int memoryPositionDifference = this->unitArrayCopy - UNIT_NODE_TABLE; 
    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
    units[i]->rawData->previousUnit += memoryPositionDifference;
    units[i]->rawData->nextUnit += memoryPositionDifference;
    }
    
    */
    //if (frameCount % 2 == 0)
    /*for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
    {
      if (units[i]->isValid() &&
          strcmp(units[i]->getOwner()->getName(),"NEM)Marwin") == 0 &&
          units[i]->getPrototype() == Prototypes::Mutalisk)
      {
        units[i]->orderRightClick(this->getScreenX() + this->getMouseX(), this->getScreenY() + this->getMouseY());
      }
    } */

    /*
    char message[50];
    sprintf(message, "Mouse (%d,%d)", this->getScreenX() + this->getMouseX(), this->getScreenY() + this->getMouseY());
    this->print(message); */
    /*if (frameCount > 100)
      this->drawBox(100,100,100,100,100);*/

    FILE *f = fopen("bwapi.log","at"); 
    fprintf(f, "Update   %d\n", this->frameCount);
    /*
    fprintf(f, "Marine          = %s - %d\n", getBinary(BWAPI::Prototypes::Marine->getUnknown()).c_str()              , BWAPI::Prototypes::Marine->getUnknown());
    fprintf(f, "Ghost           = %s - %d\n", getBinary(BWAPI::Prototypes::Ghost->getUnknown()).c_str()               , BWAPI::Prototypes::Ghost->getUnknown());
    fprintf(f, "Vulture         = %s - %d\n", getBinary(BWAPI::Prototypes::Vulture->getUnknown()).c_str()             , BWAPI::Prototypes::Vulture->getUnknown());
    fprintf(f, "Goliath         = %s - %d\n", getBinary(BWAPI::Prototypes::Goliath->getUnknown()).c_str()             , BWAPI::Prototypes::Goliath->getUnknown());
    fprintf(f, "Goliath Turret  = %s - %d\n", getBinary(BWAPI::Prototypes::GoliathTurret->getUnknown()).c_str()       , BWAPI::Prototypes::GoliathTurret->getUnknown());
    fprintf(f, "Tank            = %s - %d\n", getBinary(BWAPI::Prototypes::SiegeTank_TankMode->getUnknown()).c_str()  , BWAPI::Prototypes::SiegeTank_TankMode->getUnknown());
    fprintf(f, "Tank Turret     = %s - %d\n", getBinary(BWAPI::Prototypes::TankTurret_TankMode->getUnknown()).c_str() , BWAPI::Prototypes::TankTurret_TankMode->getUnknown());
    fprintf(f, "SCV             = %s - %d\n", getBinary(BWAPI::Prototypes::SCV->getUnknown()).c_str()                 , BWAPI::Prototypes::SCV->getUnknown());
    fprintf(f, "Wraith          = %s - %d\n", getBinary(BWAPI::Prototypes::Wraith->getUnknown()).c_str()              , BWAPI::Prototypes::Wraith->getUnknown());
    fprintf(f, "\n");
    fprintf(f, "Command center  = %s - %d\n", getBinary(BWAPI::Prototypes::CommandCenter->getUnknown()).c_str()     , BWAPI::Prototypes::CommandCenter->getUnknown());
    fprintf(f, "Barracks        = %s - %d\n", getBinary(BWAPI::Prototypes::Barracks->getUnknown()).c_str()          , BWAPI::Prototypes::Barracks->getUnknown());
    fprintf(f, "Supply depot    = %s - %d\n", getBinary(BWAPI::Prototypes::SupplyDepot->getUnknown()).c_str()       , BWAPI::Prototypes::SupplyDepot->getUnknown());


    fprintf(f, "Firebat         = %s - %d\n", getBinary(BWAPI::Prototypes::Firebat->getUnknown()).c_str(), BWAPI::Prototypes::Firebat->getUnknown());
    fprintf(f, "Medic           = %s - %d\n", getBinary(BWAPI::Prototypes::Medic->getUnknown()).c_str(), BWAPI::Prototypes::Medic->getUnknown());
    
    fprintf(f, "Tank (siege)    = %s - %d\n", getBinary(BWAPI::Prototypes::SiegeTank_SiegeMode->getUnknown()).c_str(), BWAPI::Prototypes::SiegeTank_SiegeMode->getUnknown());
    fprintf(f, "Nuclear missile = %s - %d\n", getBinary(BWAPI::Prototypes::NuclearMissile->getUnknown()).c_str(), BWAPI::Prototypes::NuclearMissile->getUnknown());
    fprintf(f, "Battle cruiser  = %s - %d\n", getBinary(BWAPI::Prototypes::BattleCruiser->getUnknown()).c_str(), BWAPI::Prototypes::BattleCruiser->getUnknown());
    fprintf(f, "Nexus           = %s - %d\n", getBinary(BWAPI::Prototypes::Nexus->getUnknown()).c_str(), BWAPI::Prototypes::Nexus->getUnknown());
    fprintf(f, "Pylon           = %s - %d\n", getBinary(BWAPI::Prototypes::Pylon->getUnknown()).c_str(), BWAPI::Prototypes::Pylon->getUnknown());
    fprintf(f, "Probe           = %s - %d\n", getBinary(BWAPI::Prototypes::Probe->getUnknown()).c_str(), BWAPI::Prototypes::Probe->getUnknown());
    fprintf(f, "Zealot          = %s - %d\n", getBinary(BWAPI::Prototypes::Zealot->getUnknown()).c_str(), BWAPI::Prototypes::Zealot->getUnknown());
    fprintf(f, "Hight templar   = %s - %d\n", getBinary(BWAPI::Prototypes::HighTemplar->getUnknown()).c_str(), BWAPI::Prototypes::HighTemplar->getUnknown());
    fprintf(f, "Hatchery        = %s - %d\n", getBinary(BWAPI::Prototypes::Hatchery->getUnknown()).c_str(), BWAPI::Prototypes::Hatchery->getUnknown());
    fprintf(f, "Overlord        = %s - %d\n", getBinary(BWAPI::Prototypes::Overlord->getUnknown()).c_str(), BWAPI::Prototypes::Overlord->getUnknown());
    fprintf(f, "Larva           = %s - %d\n", getBinary(BWAPI::Prototypes::Larva->getUnknown()).c_str(), BWAPI::Prototypes::Larva->getUnknown());
    fprintf(f, "Drone           = %s - %d\n", getBinary(BWAPI::Prototypes::Drone->getUnknown()).c_str(), BWAPI::Prototypes::Drone->getUnknown());
    fprintf(f, "Zergling        = %s - %d\n", getBinary(BWAPI::Prototypes::Zergling->getUnknown()).c_str(), BWAPI::Prototypes::Zergling->getUnknown());
    fprintf(f, "Queen           = %s - %d\n", getBinary(BWAPI::Prototypes::Queen->getUnknown()).c_str(), BWAPI::Prototypes::Queen->getUnknown());
    fprintf(f, "Mutalisk        = %s - %d\n", getBinary(BWAPI::Prototypes::Mutalisk->getUnknown()).c_str(), BWAPI::Prototypes::Mutalisk->getUnknown());
    
    fprintf(f, "Minerals        = %s - %d\n", getBinary(BWAPI::Prototypes::Minerals->getUnknown()).c_str(), BWAPI::Prototypes::Minerals->getUnknown());
    fprintf(f, "Vaspine gayser  = %s - %d\n", getBinary(BWAPI::Prototypes::VaspineGayser->getUnknown()).c_str(), BWAPI::Prototypes::VaspineGayser->getUnknown()); */

    /*fprintf(f, "Command center = (%d,%d) (%d,%d)\n", BWAPI::Prototypes::CommandCenter->dimensionUp(),
                                                     BWAPI::Prototypes::CommandCenter->dimensionDown(),
                                                     BWAPI::Prototypes::CommandCenter->dimensionLeft(),
                                                     BWAPI::Prototypes::CommandCenter->dimensionRight());
    fprintf(f, "SCV            = (%d,%d) (%d,%d)\n", BWAPI::Prototypes::SCV->dimensionUp(),
                                                     BWAPI::Prototypes::SCV->dimensionDown(),
                                                     BWAPI::Prototypes::SCV->dimensionLeft(),
                                                     BWAPI::Prototypes::SCV->dimensionRight());*/
    frameCount ++;
  }
  //----------------------------- JMP PATCH -----------------------------------
  
  #pragma warning(push)
  #pragma warning(disable:4311)
  #pragma warning(disable:4312)

  void JmpCallPatch(void *pDest, int pSrc, int nNops = 0)
  {
    DWORD OldProt = 0;
    VirtualProtect((LPVOID)pSrc, 5 + nNops, PAGE_EXECUTE_READWRITE, &OldProt);
    unsigned char jmp = 0xE9;
    memcpy((LPVOID)pSrc, &jmp, 1);
    DWORD address = (DWORD)pDest - (DWORD)pSrc - 5;
    memcpy((LPVOID)(pSrc + 1), &address, 4); 
    for (int i = 0; i < nNops; ++i)
      *(BYTE*)((DWORD)pSrc + 5 + i) = 0x90;
    VirtualProtect((LPVOID)pSrc, 5 + nNops, OldProt, &OldProt);
  }

  #pragma warning(pop)


 void __declspec(naked) DrawStaticText(const char* text, int x, int y)
  {
      __asm
      {
          mov eax, text
          mov edx, y
          push edx
          mov esi, x
          call [BW::BWFXN_HUD]
      }
  }

//------------------------------ PRINT X Y ------------------------------------
  void Game::printXY(int x, int y, const char* text)
  {
   char *buffer = new char[strlen(text) + 1];
   strcpy(buffer, text);
   this->buffers.push_back(buffer);
   DrawStaticText(text, x, y);
  }
  //---------------------------------------------------------------------------
  bool Game::isInGame()
  {
    return this->inGame;
  }
  //---------------------------------------------------------------------------
  void Game::setInGame(bool inGame)
  {
    this->inGame = inGame;
  }
  #pragma warning(push)
  #pragma warning(disable:4312)
  //----------------------------------- PRINT ---------------------------------
  void Game::print(char *text) const
  {
   void (_stdcall* sendText)(char *) = (void (_stdcall*) (char *))BW::BWXFN_PrintText;
	 	sendText(text);
  }
  //---------------------------------------------------------------------------
  void Game::printPublic(char *text) const
  {
   void (_stdcall* sendText)(char *) = (void (_stdcall*) (char *))BW::BWXFN_PrintPublicText;
	 	sendText(text);
  }
  #pragma warning(pop)
  //------------------------------- CHANGE SLOT -------------------------------
  void Game::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID),3); 
  }
  //------------------------------ CHANGE RACE ---------------------------------
  void Game::changeRace(BW::Orders::ChangeRace::Race race, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeRace(race, slotID),3); 
  }
  //---------------------------- ADD TO COMMAND BUFFER -------------------------
  void Game::addToCommandBuffer(Command *command)
  {
    command->execute();
    this->commandBuffer[this->commandBuffer.size() - 1].push_back(command);
  }
  //----------------------------- ON GAME START ---------------------------------
  void Game::onGameStart()
  {
    this->setInGame(true);
    this->marwin = NULL;
    for (int i = 0; i < 8; i++)
      if (strcmp(this->players[i]->getName(), "NEM)Marwin") == 0)
          this->marwin = this->players[i];
  }
  //------------------------------ ON GAME END ----------------------------------
  void Game::onGameEnd()
  {
    this->setInGame(false);
  }
  //------------------------------- START GAME ----------------------------------
  void Game::startGame()
  { 
    this->IssueCommand((PBYTE)&BW::Orders::StartGame(),sizeof(BW::Orders::StartGame));
  }
  //------------------------------- GET MOUSE X ---------------------------------
  int Game::getMouseX()
  {
	 	return *((int*)0x006CDDAC);
  }
  //------------------------------- GET MOUSE Y ---------------------------------
  int Game::getMouseY()
  {
	 	return *((int*)0x006CDDB0);
  }
  //------------------------------- GET SCREEN X --------------------------------
  int Game::getScreenX()
  {
	 	return *((int*)0x00628430);
  }
  //------------------------------- GET SCREEN Y --------------------------------
  int Game::getScreenY()
  {
	 	return *((int*)0x00628458);
  }
  //-----------------------------------------------------------------------------
//  int BWFXN_DrawBox = 0x4E18E0;
//  char* BWFXN_pointer = (char*) 0x6CF494;
  void Game::drawBox(DWORD x, DWORD y, DWORD w, DWORD h, BYTE clr)
  {
    static const int BWFXN_DrawBox = 0x4E18E0; 
    __asm 
    {
     /// mov al, clr
     // mov ds:[0x6CF494], al
      push h
      push w
      push y
      push x
      call [BWFXN_DrawBox]
    }  
  }
  //-----------------------------------------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  void Game::refresh()
  {
    void (_stdcall* refresh)(void) = (void (_stdcall*) ())BW::BWXFN_Refresh;
 	 	refresh();
  }
  #pragma warning(pop)
  //-----------------------------------------------------------------------------
  Unit* Game::getUnit(int index)
  {
    return this->units[index];
  }
  //--------------------------------- SAVE SELECTED -----------------------------
  BW::UnitData** Game::saveSelected()
  {
    BW::UnitData** selected = new BW::UnitData * [13];
    memcpy(selected, BW::BWXFN_CurrentPlayerSelectionGroup, 4*12);
    selected[12] = NULL;
    return selected;
  }
  //--------------------------------- LOAD SELECTED -----------------------------
  void Game::loadSelected(BW::UnitData** selected)
  {
    int unitCount = 0;
    while (selected[unitCount] != NULL)
      unitCount ++;
    void (_stdcall* selectUnitsHelperSTD)(int, BW::UnitData * *, bool, bool) = (void (_stdcall*) (int, BW::UnitData * *, bool, bool))0x0049AB90;
	   selectUnitsHelperSTD(unitCount, selected, true, true);
    delete [] selected;
  }
  //-----------------------------------------------------------------------------
  void Game::onCancelTrain()
  {
    BW::UnitData** selected = this->saveSelected();
    if (selected[0] != NULL)
      this->addToCommandBuffer(new CommandCancelTrain(BWAPI::Unit::BWUnitToBWAPIUnit(selected[0])));
    delete [] selected;
  }
  //-----------------------------------------------------------------------------
};

