#include "Game.h"

#include "../BWAPI/Player.h"
#include "../BWAPI/Unit.h"
#include "../BWAPI/UnitPrototype.h"
#include "../BWAPI/UnitPrototypeDefinitions.h"
#include "../BWAPI/Command.h"

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
    
    for (int i = 0; i < 1700; i++)
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

    for (int i = 0; i < 1700; i++)
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
  Unit *cc;
  //-------------------------------- Closer to CC -----------------------------
  int closerToCC(Unit*& unit1, Unit*& unit2)
  {
   return cc->getDistance(unit1) < cc->getDistance(unit2);
  }
  //---------------------------------- TEST -----------------------------------

  void Game::test(void)
  {

    /*
    I will implement this later on using some correct pointers method on unit
    _w64 int memoryPositionDifference = this->unitArrayCopy - UNIT_NODE_TABLE; 
    for (int i = 0; i < 1700; i++)
    {
    units[i]->rawData->previousUnit += memoryPositionDifference;
    units[i]->rawData->nextUnit += memoryPositionDifference;
    }
    
    */
    //if (frameCount % 2 == 0)
    /*for (int i = 0; i < 1700; i++)
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

    FILE *f = fopen("bwapi.log","at"); 
    fprintf(f, "Update %d\n", this->frameCount);
    //fprintf(f, "Mutalisk hp =  %d\n", BWAPI::Prototypes::Mutalisk->getMaxHealthPoints());
    fprintf(f, "Marine =  %d\n", BWAPI::Prototypes::Marine->getArmor());
    fprintf(f, "Ghost =  %d\n", BWAPI::Prototypes::Ghost->getArmor());
    fprintf(f, "Vulture =  %d\n", BWAPI::Prototypes::Vulture->getArmor());
    fprintf(f, "Goliath =  %d\n", BWAPI::Prototypes::Goliath->getArmor());
    fprintf(f, "Battle cruiser =  %d\n", BWAPI::Prototypes::BattleCruiser->getArmor());
    bool found = false;
    std::vector<Unit*> unitList;
    cc = NULL;
    Player *marwin = NULL;
    bool reselected = false;
    for (int i = 0; i < 1700; i++)
    {
      if (units[i]->isValid() &&
          strcmp(units[i]->getOwner()->getName(),"NEM)Marwin") == 0)
      {
        marwin = units[i]->getOwner();
        found = true;
        if (units[i]->getPrototype() == Prototypes::SCV &&
            units[i]->getOrderIDLocal() == BW::OrderID::Idle)
          unitList.push_back(this->units[i]);
        else
         if (units[i]->getPrototype() == Prototypes::CommandCenter)
           cc = this->units[i];
      }
    }
    BW::UnitData** selected = NULL;
    if (marwin != NULL)
    {
      selected = new BW::UnitData * [13];
      memcpy(selected, BW::BWXFN_CurrentPlayerSelectionGroup, 4*12);
      selected[12] = NULL;
    }
    if (cc != NULL)
    {
     fprintf(f, "Terran Suppplies %d/%d\n", marwin->getSuppliesUsedTerran(), marwin->getSuppliesAvailableTerran());
     fprintf(f, "Free Terran Suppplies %d\n", marwin->freeSuppliesTerranLocal());
     fprintf(f, "SCV supplie use %d\n", BWAPI::Prototypes::SCV->getSupplies());
     fprintf(f, "SCV mineral price %d\n", BWAPI::Prototypes::SCV->getMineralPrice());
     if (cc->hasEmptyBuildQueueLocal() && 
         marwin->getMineralsLocal() >= BWAPI::Prototypes::SCV->getMineralPrice() &&
         marwin->freeSuppliesTerranLocal() >= BWAPI::Prototypes::SCV->getSupplies())
      {
        reselected = true;
        cc->orderSelect();
        cc->trainUnit(BWAPI::Prototypes::SCV);
      }
    }
    if (unitList.size() != 0)
    {
      std::vector<Unit *> mineralList;
      for (int i = 0; i < 1700; i++)
      {
        if (units[i]->isValid() && 
            units[i]->getPrototype() == Prototypes::Minerals)
          mineralList.push_back(units[i]);
      }
      std::sort(mineralList.begin(),mineralList.end(), closerToCC);
      for (unsigned int i = 0; i < unitList.size(); i++)
      {
        reselected = true;
        unitList[i]->orderRightClick(mineralList[i]);
      }
    }
   
   if (reselected)
    {
      int unitCount = 0;
      while (selected[unitCount] != NULL)
        unitCount ++;
      void (_stdcall* selectUnitsHelperSTD)(int, BW::UnitData * *, bool, bool) = (void (_stdcall*) (int, BW::UnitData * *, bool, bool))0x0049AB90;
	     selectUnitsHelperSTD(unitCount, selected, true, true);
    }
    if (selected)
      delete [] selected;
    
    fclose(f);
    frameCount ++;
  }
  //----------------------------- JMP PATCH -----------------------------------
  
  #pragma warning(push)
  #pragma warning(disable:4311)
  #pragma warning(disable:4312)

  VOID JmpPatch(void *pDest, void *pSrc, int nNops = 0)
  {
      DWORD OldProt;
      
      VirtualProtect(pSrc, 5 + nNops, PAGE_EXECUTE_READWRITE, &OldProt);
      
      *(u8*)pSrc = (u8)0xE9;
      *(DWORD*)((DWORD)pSrc + 1) = (DWORD)pDest - (DWORD)pSrc - 5;
      
      for (int i = 0; i < nNops; ++i)
      {
          *(BYTE*)((DWORD)pSrc + 5 + i) = 0x90;
      }
      
      VirtualProtect(pSrc, 5 + nNops, OldProt, &OldProt);
  }
  VOID JmpPatch(void *pDest, void *pSrc, unsigned char jmp = 0xE9, int nNops = 0)
  {
	  DWORD OldProt;
      
      VirtualProtect(pSrc, 5 + nNops, PAGE_EXECUTE_READWRITE, &OldProt);
      
      *(char*)pSrc = (char)jmp;
      *(DWORD*)((DWORD)pSrc + 1) = (DWORD)pDest - (DWORD)pSrc - 5;
      
	  for (int i = 0; i < nNops; ++i)
	  {
	      *(BYTE*)((DWORD)pSrc + 5 + i) = 0x90;
	  }
  	    
	  VirtualProtect(pSrc, 5 + nNops, OldProt, &OldProt);
  }

  #pragma warning(pop)

  void DrawStaticText(char* Text, int X, int Y)
  {
      __asm
      {
          mov eax, Text
          mov edx, Y
          push edx
          mov esi, X
          call [BW::BWFXN_HUD]
      }
  }
  char buffer[200];
  int xToPrint, yToPrint;
  void ProcessTest()
  {
    DrawStaticText(buffer, xToPrint , yToPrint);
  }

  void __declspec(naked) Test()
  {
    static int ReturnAddr = (0x48CC25+6);
    __asm 
	   {
      pushad
      call ProcessTest
      popad
      mov esi, 0x0A
      jmp [ReturnAddr]
    }
  }
//------------------------------ PRINT X Y ------------------------------------
  void Game::printXY(int x, int y, char* text) const
  {
   strcpy(buffer, text);
   xToPrint = x;
   yToPrint = y;
   JmpPatch(&Test,(PBYTE)0x48CC25,6);
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
    this->commandBuffer[this->commandBuffer.size() - 1].push_back(command);
  }
  //----------------------------- ON GAME START ---------------------------------
  void Game::onGameStart()
  {
    this->setInGame(true);
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
};
