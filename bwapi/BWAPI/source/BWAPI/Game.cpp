#include "Game.h"

#include "../BWAPI/Player.h"
#include "../BWAPI/Unit.h"
#include "../BWAPI/UnitPrototype.h"
#include "../BWAPI/UnitPrototypeDefinitions.h"

#include "../BW/Offsets.h"
#include "../BW/UnitData.h"
#include "../BW/OrderTypes.h"

#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>


namespace BWAPI 
{
  //------------------------------ CONSTRUCTOR ----------------------------------
  Game::Game()
  {
    unitArrayCopy = new BW::UnitArray;

    for (int i = 0; i < 12; i++)
      players[i] = new Player((u8)i);    
    
    players[11]->setName("Player 12 (Neutral)");
    
    for (int i = 0; i < 1700; i++)
      units[i] = new Unit(&unitArrayCopy->unit[i], &BW::UnitNodeTable->unit[i]);

    this->update();
  }
  //------------------------------- DESTRUCTOR ----------------------------------
  Game::~Game()
  {
    delete unitArrayCopy;

    for (int i = 0; i < 12; i++)
      delete players[i];

    for (int i = 0; i < 1700; i++)
      delete units[i];
  }
  int count = 0;
     void __fastcall IssueCommand(PBYTE pbBuffer, int iSize) {
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
    memcpy(this->unitArrayCopy, BW::UnitNodeTable, sizeof(BW::UnitArray));
    players[11]->setName("Player 12 (Neutral)");
  }
  //----------------------------- JMP PATCH -----------------------------------
  void Game::test(void)
  {
   if (count > 500)
       return;
    /*
    I will implement this later on using some correct pointers method on unit
    _w64 int memoryPositionDifference = this->unitArrayCopy - UNIT_NODE_TABLE; 
    for (int i = 0; i < 1700; i++)
    {
    units[i]->rawData->previousUnit += memoryPositionDifference;
    units[i]->rawData->nextUnit += memoryPositionDifference;
    }
    */
    FILE *f;
    f = fopen("bwapi.log","at"); 
    if (this->inGame())
      fprintf(f, "Game running.\n");
    else
      fprintf(f, "Game not running.\n");
    fprintf(f, "Found units :\n");
    bool found = false;
    std::vector<Unit*> unitList;
    u16 x = 0,y = 0;
    for (int i = 0; i < 1700; i++)
    {
      if (units[i]->isValid() && 
          units[i]->getPrototype() != NULL &&
          // Proceed only when the owner of units is called "Marwin"
          strcmp(units[i]->getOwner()->getName(),"NEM)Marwin") == 0 
          && units[i]->getPrototype() == Prototypes::SCV)
      {
        fprintf(f, "(%s) (%d,%d) (%s) (%d)\n", 
          units[i]->getPrototype()->getName().c_str(),
          units[i]->getPosition().x,
          units[i]->getPosition().y,
          units[i]->getOwner()->getName(),
          units[i]->getOrderID());
        found = true;
        unitList.push_back(units[i]);
        units[i]->getOriginalRawData()->orderSignal = BW::OrderID::Moving;
        if (count % 2 ==0)
          x = units[i]->getPosition().x + 100;
        else
          x = units[i]->getPosition().x - 100;
        y = units[i]->getPosition().y;     
      }
    }
   fclose(f);
   
   // Selection command seeding.
   int n = (int)unitList.size();
   if (n > 0)
   {
     /*BYTE MoveCommand[] = {x & 255, x >> 8, y&255 , 0x14,
                           0xe4   , 0x0   , 0x0   , y >> 8,
                           0x0,0x0};*/
     /*BYTE MoveCommand[] = {0x5  , 0x0 , 0x5 , 0x14,
                           0xe4 , 0x0 , 0x0 , 0x0,
                           0x0,0x0};*/
     /*BYTE MoveCommand[] = {0x14  , 0x5 , 0x0 , 0x5,
                           0x0 , 0x0 , 0x0 , 0xe4,
                           0x0,0x0,0x0,0x0,};*/

    IssueCommand((PBYTE)&BW::Orders::Move(x,y), 0xA);

     char * text = new char[100];
     sprintf(text,"Order given (%d)", count);
     this->print(text);
     delete[] text;

     //BYTE BuildDrone[] = {0x23, 0x23, 0x00};
     //void (_stdcall* sendCommand)(int x, int y, int value, int code) = (void (_stdcall*) (int x, int y, int value, int code))0x004BFF80;
     // call the selection
	 	  //sendCommand(0, 0, 0xe4, 0);
     //IssueCommand((PBYTE)BuildDrone, 3);
     
     //IssueCommand((PBYTE)BuildDrone, 3);
     
 
// heres how u use
// Zerg Mineral hack
// NOTE: must select an egg and must be able to build a drone for this to work
     
     //IssueCommand((PBYTE)CancelLastQueue, 3); 
     
     count ++;
     /*
	 	  char * text = new char[100];
     sprintf(text,"%d units selected", n);
     this->print(text);
     delete[] text;
     // array of pointers of units to be selected
     BW::UnitData * * list = new BW::UnitData * [n + 1];
	    for (int i = 0; i < n; i++)
       // I will insert every unit in the unitList(Every scv of player "Marwin")
       list[i] = unitList[i]->getOriginalRawData();
         // I must put the pointer to the original data structure of bw not our's copy, as bw function will process it
     // The last item must be nulled (But I'm not sure if this is mandatory, to be tested)
	    list[n] = NULL;
     // Definition of the selection function (note that this funciton is 0x0049AB90 not 0x0049ABA0 as multicommand says)
     void (_stdcall* selectUnitsHelperSTD)(int, BW::UnitData * *, bool, bool) = (void (_stdcall*) (int, BW::UnitData * *, bool, bool))0x0049AB90;
     // call the selection
	 	  selectUnitsHelperSTD(n, list, true, true);
     // delete the array of pointers
     delete[] list;*/
   }
   else
   {
     this->changeSlot(BW::Orders::ChangeSlot::Computer, 1);
     this->changeRace(BW::Orders::ChangeRace::Zerg, 1);
     this->changeRace(BW::Orders::ChangeRace::Terran, 0);
   }
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
  //-----------------------------------------------------------------------------
  bool Game::inGame()
  {
    return (*(char *)BW::BWFXN_InGame != 0);
  }
  //----------------------------------- PRINT -----------------------------------
  void Game::print(char *text) const
  {
   void (_stdcall* sendText)(char *) = (void (_stdcall*) (char *))BW::BWXFN_PrintText;
	 	sendText(text);
  }
  //-----------------------------------------------------------------------------
  void Game::printPublic(char *text) const
  {
   void (_stdcall* sendText)(char *) = (void (_stdcall*) (char *))BW::BWXFN_PrintPublicText;
	 	sendText(text);
  }
  //--------------------------------- CHANGE SLOT -----------------------------
  void Game::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID),3); 
  }
  //--------------------------------- CHANGE RACE -------------------------------
  void Game::changeRace(BW::Orders::ChangeRace::Race race, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeRace(race, slotID),3); 
  }
  //-----------------------------------------------------------------------------
};
