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
   #include <stdio.h>
  std::string getBinary(UNKNOWN_TYPE value)
  {
   std::string result;
    for (int i = 0; i < UNKNOWN_BIT_SIZE; i++)
      if (value  & (1 << (UNKNOWN_BIT_SIZE-1-i)))
         result += "1";
      else
         result += "0";
    return result;
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
    if (frameCount > 100)
      this->drawBox(100,100,100,100,100);

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

    //fprintf(f, "Command cent = %s - %d\n", getBinary(BWAPI::Prototypes::Marine->getUnknown()).c_str()              , BWAPI::Prototypes::Marine->getUnknown());

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
      /*
      if (selected[0] != NULL)
      {
        char message[50];
        sprintf(message, "Selected unit id = %d", selected[0]->unitID);
        this->print(message);
      }*/
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
};

