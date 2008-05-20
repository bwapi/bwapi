#include "Game.h"

#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

#include <Logger.h>
#include <Dictionary.h>
#include <Exceptions.h>

#include "../BWAPI/Player.h"
#include "../BWAPI/Unit.h"
#include "../BWAPI/Command.h"
#include "../BWAPI/CommandCancelTrain.h"
#include "../BWAPI/Map.h"

#include "../BW/Offsets.h"
#include "../BW/Unit.h"
#include "../BW/UnitTarget.h"
#include "../BW/OrderTypes.h"
#include "../BW/Latency.h"
#include "../BW/TileType.h"
#include "../BW/TileSet.h"
#include "../BW/UnitType.h"


namespace BWAPI 
{
  //------------------------------ CONSTRUCTOR ----------------------------------
  Game::Game()
  :inGame(false)
  ,unitsOnTile(0,0)
  {
    try
    {
     this->configuration = new Dictionary("bwapi.ini");
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error","wt");
      fprintf(f, "Couldn't load configuration file bwapi.ini because:", exception.getMessage());
      fclose(f);
    }
    this->commandLog        = new Util::Logger(this->configuration->getValue("log_path") + "\\commands", LogLevel::MicroDetailed);
    this->newOrderLog       = new Util::Logger(this->configuration->getValue("log_path") + "\\new_orders", LogLevel::MicroDetailed);
    this->badAssumptionLog  = new Util::Logger(this->configuration->getValue("log_path") + "\\bad_assumptions", LogLevel::MicroDetailed);
    this->newUnitLog        = new Util::Logger(this->configuration->getValue("log_path") + "\\new_unit_id", LogLevel::MicroDetailed);
    this->unitSum           = new Util::Logger(this->configuration->getValue("log_path") + "\\unit_sum", LogLevel::MicroDetailed);
    this->fatalError        = new Util::Logger(this->configuration->getValue("log_path") + "\\FATAL-ERROR", LogLevel::MicroDetailed);

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
    this->latency = BW::Latency::BattlenetLow; // @todo read from the address in update
    this->quietSelect = true;
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
    
    delete this->commandLog;
    delete this->newOrderLog;
    delete this->badAssumptionLog;
    delete this->newUnitLog;
    delete this->unitSum;
    delete this->fatalError;
    delete this->configuration;
  }
  //------------------------------- ISSUE COMMAND -------------------------------
  void Game::IssueCommand(PBYTE pbBuffer, u32 iSize) 
  {
    __asm 
    {
      MOV ECX, pbBuffer
      MOV EDX, iSize
      CALL [BW::BWFXN_IssueCommand]
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
    while (this->commandBuffer.size() > this->getLatency())
      this->commandBuffer.erase(this->commandBuffer.begin());
    this->commandBuffer.push_back(a);
    for (unsigned int i = 0; i < this->commandBuffer.size(); i++)
       for (unsigned int j = 0; j < this->commandBuffer[i].size(); j++)
         this->commandBuffer[i][j]->execute();
    this->first = Unit::BWUnitToBWAPIUnit(*BW::BWXFN_UnitNodeTable_FirstElement);
    if (this->getFirst() != NULL)
      this->getFirst()->updateNext();
    this->frameCount ++;
    this->logUnknownOrStrange();
    this->updateUnitsOnTile();    
  }
  //---------------------------------------------------------------------------
  bool Game::isInGame() const
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
  void Game::print(char *text)
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
    this->commandLog->log("(%4d) %s", this->frameCount, command->describe().c_str());
  }
  //----------------------------- ON GAME START ---------------------------------
  void Game::onGameStart()
  {
    this->frameCount = 0;
    this->setInGame(true);
    this->BWAPIPlayer = NULL;
    for (int i = 0; i < 8; i++)
      if (strcmp(this->players[i]->getName(), "BWAPI") == 0)
          this->BWAPIPlayer = this->players[i];
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
  int Game::getMouseX() const
  {
    return *(BW::BWXFN_MouseX);
  }
  //------------------------------- GET MOUSE Y ---------------------------------
  int Game::getMouseY() const
  {
   return *(BW::BWXFN_MouseY);
  }
  //------------------------------- GET SCREEN X --------------------------------
  int Game::getScreenX() const
  {
   return *(BW::BWXFN_ScreenX);
  }
  //------------------------------- GET SCREEN Y --------------------------------
  int Game::getScreenY() const
  {
   return *(BW::BWXFN_ScreenY);
  }
  //-----------------------------------------------------------------------------
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
  BW::Unit** Game::saveSelected()
  {
    BW::Unit** selected = new BW::Unit * [13];
    memcpy(selected, BW::BWXFN_CurrentPlayerSelectionGroup, 4*12);
    selected[12] = NULL;
    return selected;
  }
  //--------------------------------- LOAD SELECTED -----------------------------
  void Game::loadSelected(BW::Unit** selected)
  {

    /** Deselecting unit is not ilegal, but at least strange, so I will diable it*/
    if (selected[0] == NULL)
    {
      delete [] selected;
      return;
    }
    
   /* BW::Unit** selectedNow = this->saveSelected();
    int i;
    for (i = 0; selected[i] ==  selectedNow[i]; i++)
    if (selected[i] == NULL && selectedNow[i] == NULL)
    {
       delete [] selected;
       delete [] selectedNow;
       return;
    }
    delete selectedNow;*/

    int unitCount = 0;
    while (selected[unitCount] != NULL)
      unitCount ++;
    if (quietSelect)
    {
      /*byte* inputData = new byte[2 + unitCount*2];
      inputData[0] = 0x09;
      inputData[1] = unitCount;
      for (int i = 0; i < unitCount; i++)
      {
        BW::UnitTarget target = BW::UnitTarget(BWAPI::Unit::BWUnitToBWAPIUnit(selected[i]));
        memcpy(inputData + 2 + 2*i, &target, sizeof(BW::UnitTarget));
      }
      this->IssueCommand(inputData, 2 + unitCount*2);
      delete [] inputData;*/
      void (_stdcall* selectUnits)(int, BW::Unit * *) = (void (_stdcall*) (int, BW::Unit * *)) 0x004C04E0;
	     selectUnits(unitCount, selected);
    }
    else
    {
      void (_stdcall* selectUnitsHelperSTD)(int, BW::Unit * *, bool, bool) = (void (_stdcall*) (int, BW::Unit * *, bool, bool)) 0x0049AB90;
	     selectUnitsHelperSTD(unitCount, selected, true, true);
      
    }
    delete [] selected;   
  }
  //---------------------------------- ON CANCEL TRAIN --------------------------
  void Game::onCancelTrain()
  {
    try
    {
      BW::Unit** selected = this->saveSelected();
      if (selected[0] != NULL)
        this->addToCommandBuffer(new CommandCancelTrain(BWAPI::Unit::BWUnitToBWAPIUnit(selected[0])));
      delete [] selected;
    }
    catch (GeneralException& exception)
    {
      Util::Logger::globalLog->log("Exception in onCancelTrain: " + exception.getMessage());
    }
  }
  //----------------------------- ON REMOVE UNIT --------------------------------
  void Game::onRemoveUnit(BW::Unit *unit)
  {
  }
  //--------------------------- LOG UNKNOWN OR STRANGE --------------------------
  void Game::logUnknownOrStrange()
  {
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (!i->getType().isValid())
        this->newUnitLog->log("%s", i->getName().c_str());
  }
  //----------------------------------- GET BINARY ---------------------------
  template <class Type>
  std::string Game::getBinary(Type value)
  {
   std::string result;
    for (int i = 0; i < sizeof(Type)*8; i++)
      if (value  & (1 << (sizeof(Type)*8-1-i)))
         result += "1";
      else
         result += "0";
    return result;
  }
  //--------------------------------- LOG UNIT LIST ---------------------------
  void Game::logUnitList()
  {
    this->unitSum->log("----------------------------------------");
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
       this->unitSum->log("%s", i->getName().c_str());
    this->unitSum->log("----------------------------------------");
  }
  //-------------------------------- GET FIRST -------------------------------
  Unit* Game::getFirst()
  {
    return this->first;
  }
  //------------------------------ GET LATENCY -------------------------------
  BW::Latency::Enum Game::getLatency()
  {
    return this->latency;
  }
  //------------------------------ PRINT UNIT COUNT PER TILE -----------------
  void Game::printUnitCountPerTile()
  {
    FILE *f = fopen("unit-counts.txt", "wt");
    for (int y = 0; y < Map::getHeight();y++)
    {
      for (int x = 0; x < Map::getWidth(); x++)
      {
         if ((((BW::TileSet::getTileType(BWAPI::Map::getTile(x, y))->buildability >> 4) & 0X8)) == 0)
          fprintf(f, "%d", this->unitsOnTile[x][y].size());
        else
          fprintf(f, "X");
      }
      fprintf(f,"\n");
    }
    fclose(f);
  }
  //------------------------------ UPDATE UNITS ON TILE ----------------------
  void Game::updateUnitsOnTile()
  {
    this->unitsOnTile.resize(Map::getWidth(), Map::getHeight());
    for (int y = 0; y < Map::getHeight(); y++)
      for (int x = 0; x < Map::getWidth(); x++)
        this->unitsOnTile[x][y].clear();
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->isValid())
      {
        int startX =   (i->getPosition().x - i->getType().dimensionLeft())/BW::TileSize;
        int endX = (int) ceil((i->getPosition().x + i->getType().dimensionRight())/((float)BW::TileSize));
        int startY =   (i->getPosition().y - i->getType().dimensionUp())/BW::TileSize;
        int endY = (int) ceil((i->getPosition().y + i->getType().dimensionDown())/((float)BW::TileSize));
        for (int x = startX; x < endX; x++)
          for (int y = startY; y < endY; y++)
            this->unitsOnTile[x][y].push_back(i);
      }
  }
  //--------------------------------------------------------------------------
};

