#include "Game.h"

#include "../BWAPI/Player.h"
#include "../BWAPI/Unit.h"
#include "../BWAPI/UnitPrototype.h"
#include "../BWAPI/UnitPrototypeDefinitions.h"

#include "../BW/Offsets.h"
#include "../BW/UnitData.h"

#include <stdio.h>
#include <windows.h>
#include <string>


namespace BWAPI 
{
  //------------------------------ CONSTRUCTOR ----------------------------------
  Game::Game()
  {
    unitArrayCopy = new BW::UnitArray;

    for (int i = 0; i < 12; i++)
      players[i] = new Player(i);    
    
    players[11]->setName("Player 12 (Neutral)");
    
    for (int i = 0; i < 1700; i++)
      units[i] = new Unit(&unitArrayCopy->unit[i]);

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
  int test = 0;
  //---------------------------------- UPDATE -----------------------------------
  void Game::update()
  {
    memcpy(this->unitArrayCopy, BW::UnitNodeTable, sizeof(BW::UnitArray));
    players[11]->setName("Player 12 (Neutral)");
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
    fprintf(f, "Found units :\n");
    bool found = false;
    for (int i = 0; i < 1700; i++)
    {
      if (units[i]->isValid() && 
          units[i]->getPrototype() != NULL &&
          strcmp(units[i]->getOwner()->getName(),"Marwin") == 0)
      {
        fprintf(f, "(%s) (%d,%d) (%s)\n", 
          units[i]->getPrototype()->getName().c_str(),
          units[i]->getPosition().x,
          units[i]->getPosition().y,
          units[i]->getOwner()->getName());
        found = true;
      }
    }
   fclose(f);
   
   if (found)
   { 
     test ++;
     BW::Position target;
     target.x = 0;
     target.y = 0;
     units[0]->order(test,target);            
     char *message = new char[20];
     sprintf(message,"Update %d", test%20);
     f = fopen("bwapi.log","at"); 
     fprintf(f, message);
     fclose(f);
     this->printXY(0, ((test%20)*10)%200, message);
     delete [] message;
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
      
      *(char*)pSrc = (char)0xE9;
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
    
};
