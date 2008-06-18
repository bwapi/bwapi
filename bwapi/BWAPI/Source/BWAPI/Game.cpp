#include "Game.h"

#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

#include <Util/FileLogger.h>
#include <Util/Dictionary.h>
#include <Util/Exceptions.h>

#include <BWAPI/Player.h>
#include <BWAPI/Unit.h>
#include <BWAPI/Command.h>
#include <BWAPI/CommandCancelTrain.h>
#include <BWAPI/Map.h>
#include <BWAPI/ScreenLogger.h>

#include <BW/Unit.h>
#include <BW/Offsets.h>
#include <BW/UnitTarget.h>
#include <BW/OrderTypes.h>
#include <BW/Latency.h>
#include <BW/TileType.h>
#include <BW/TileSet.h>
#include <BW/UnitType.h>
#include <BW/TechType.h>


namespace BWAPI 
{
  //------------------------------ CONSTRUCTOR ----------------------------------
  Game::Game()
  :onStartCalled(false)
  ,unitsOnTile(0,0)
  ,quietSelect(true)
  ,enabled(true)
  {
    try
    {
     this->configuration = new Util::Dictionary("bwapi.ini");
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error","wt");
      fprintf(f, "Couldn't load configuration file bwapi.ini because:", exception.getMessage());
      fclose(f);
    }
    this->commandLog        = new Util::FileLogger(this->configuration->getValue("log_path") + "\\commands", Util::LogLevel::MicroDetailed);
    this->newOrderLog       = new Util::FileLogger(this->configuration->getValue("log_path") + "\\new_orders", Util::LogLevel::MicroDetailed);
    this->badAssumptionLog  = new Util::FileLogger(this->configuration->getValue("log_path") + "\\bad_assumptions", Util::LogLevel::MicroDetailed);
    this->newUnitLog        = new Util::FileLogger(this->configuration->getValue("log_path") + "\\new_unit_id", Util::LogLevel::MicroDetailed);
    this->unitSum           = new Util::FileLogger(this->configuration->getValue("log_path") + "\\unit_sum", Util::LogLevel::MicroDetailed);
    this->fatalError        = new Util::FileLogger(this->configuration->getValue("log_path") + "\\FATAL-ERROR", Util::LogLevel::MicroDetailed);

    unitArrayCopy = new BW::UnitArray;
    unitArrayCopyLocal = new BW::UnitArray;

    for (int i = 0; i < 12; i++)
      players[i] = new Player((u8)i);    
    
   for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      units[i] = new Unit(&unitArrayCopy->unit[i], 
                          &BW::BWXFN_UnitNodeTable->unit[i],
                          &unitArrayCopyLocal->unit[i],
                          i);

    this->latency = BW::Latency::BattlenetLow; // @todo read from the address in update
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
    if (!this->enabled)
      return;
    if (!this->isOnStartCalled())
      this->onGameStart();
    memcpy(this->unitArrayCopy, BW::BWXFN_UnitNodeTable, sizeof(BW::UnitArray));
    memcpy(this->unitArrayCopyLocal, BW::BWXFN_UnitNodeTable, sizeof(BW::UnitArray));
    for (int i = 0; i < 12; i++)
      this->players[i]->update();
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
    
    for (int i = 0; i < 12; i++)
      for (u16 j = 0; j < BW::UNIT_TYPE_COUNT; j++)
      {
        this->players[i]->unitTypeCount[j] = 0;
        this->players[i]->allUnitTypeCount[j] = 0;
      }
    
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      if (i->getOwner()->getID() < 12)
      {
        this->players[i->getOwner()->getID()]->allUnitTypeCount[i->getType().getID()]++;
        if (i->isReady())
          this->players[i->getOwner()->getID()]->unitTypeCount[i->getType().getID()]++;
        if (!i->isReady() && 
             !i->getType().isBuilding() && 
             i->getType().isTerran() &&
             i->getOrderID() != BW::OrderID::Die)
          this->badAssumptionLog->log("%s is in the list but not finished", i->getName().c_str());

        if (i->getBuildUnit() != NULL &&
            i->getType().canProduce())
          this->players[i->getOwner()->getID()]->allUnitTypeCount[i->getBuildUnit()->getType().getID()]++;
      }
  }
  //---------------------------------------------------------------------------
  bool Game::isOnStartCalled() const
  {
    return this->onStartCalled;
  }
  //---------------------------------------------------------------------------
  void Game::setOnStartCalled(bool onStartCalled)
  {
    this->onStartCalled = onStartCalled;
  }
  #pragma warning(push)
  #pragma warning(disable:4312)
  //---------------------------------------------------------------------------
  bool Game::isInGame() const
  {
    return *(BW::BWFXN_InGame) == 0;
  }
  //----------------------------------- PRINT ---------------------------------
  void Game::print(const char *text)
  {
   
   void (_stdcall* sendText)(const char *) = (void (_stdcall*) (const char *))BW::BWXFN_PrintText;
	 	sendText(text);
  }
  //---------------------------------------------------------------------------
  void Game::printPublic(const char *text) const
  {
	 	__asm
	 	{
	 	  mov eax, 0
	 	  mov ecx, 0
	 	  mov edx, text
	 	  mov esi, text
	 	  call [BW::BWXFN_PrintPublicText]
	 	}
  }
  #pragma warning(pop)
  //------------------------------- CHANGE SLOT -------------------------------
  void Game::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID),3); 
  }
  //------------------------------ CHANGE RACE ---------------------------------
  void Game::changeRace(BW::Race::Enum race, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeRace(race, slotID),3); 
  }
  //---------------------------- ADD TO COMMAND BUFFER -------------------------
  void Game::addToCommandBuffer(Command *command)
  {
    this->reselected = true;
    command->execute();
    this->commandBuffer[this->commandBuffer.size() - 1].push_back(command);
    this->commandLog->log("(%4d) %s", this->frameCount, command->describe().c_str());
  }
  //----------------------------- ON GAME START ---------------------------------
  void Game::onGameStart()
  {
    this->frameCount = 0;
    this->setOnStartCalled(true);
    this->BWAPIPlayer = NULL;
    for (int i = 0; i < 8; i++)
      if (this->configuration->getValue("bwapi_name") == this->players[i]->getName())
          this->BWAPIPlayer = this->players[i];
          
    if (this->techNameToType.empty())
      for (int i = 0; i < BW::TECH_TYPE_COUNT; i++)
        if (BW::TechType((BW::TechID::Enum)i).isValid())
          this->techNameToType.insert(std::pair<std::string, BW::TechType>
                                       (
                                         BW::TechType( (BW::TechID::Enum) i ).getName(), 
                                         BW::TechType( (BW::TechID::Enum) i)
                                       )
                                     );           
  }
  //------------------------------ ON SEND TEXT ---------------------------------
  bool Game::onSendText(const char* text)
  {
    std::string message = text;
    std::string prefix = "/bwapi ";
    if (prefix.size() <= message.size() &&
       message.substr(0, prefix.size()) == prefix)
      {
        std::string rest = message.substr(prefix.size(), message.size() - prefix.size());
        if (rest == "on")
        {
          this->enabled = true;
          this->print("bwapi enabled");
        }
        else if (rest == "off")
        {
          this->enabled = false;
          this->print("bwapi disabled");
        }
        else this->print(std::string("Unknown command '" + rest + "' - possible commands are: on, off").c_str());
        return true;
      }
    prefix = "/unit ";
    if (prefix.size() <= message.size() &&
       message.substr(0, prefix.size()) == prefix)
      {
        std::string rest = message.substr(prefix.size(), message.size() - prefix.size());
        if (rest == "info")
        {
//          ScreenLogger().log()
          BW::Unit** selected = this->saveSelected();
          for (u16 i = 0; selected[i] != NULL; i++)
            this->print(BWAPI::Unit::BWUnitToBWAPIUnit(selected[i])->getName().c_str());
        }
        else if (rest == "off")
        {
          this->enabled = false;
          this->print("bwapi disabled");
        }
        else this->print(std::string("Unknown command '" + rest + "' - possible commands are: on, off").c_str());
        return true;
      }
      
    prefix = "/get ";
    if (prefix.size() <= message.size() &&
       message.substr(0, prefix.size()) == prefix)
      {
        std::string rest = message.substr(prefix.size(), message.size() - prefix.size());
        if (rest == "playerID")
        {
          char text[100];
          sprintf(text, "Current player id = %d", this->BWAPIPlayer->getID());
          this->print(text);
        }
        else if (rest.substr(0, strlen("researchState "))  == "researchState ")
        {
          rest = rest.substr(strlen("researchState "), rest.size() - strlen("researchState "));
          BW::TechType tech = this->techNameToType[rest];
          if (tech == BW::TechID::None)
            this->print(std::string("Unknown tech name '" + rest + "'").c_str());
          else
          {
            if (this->BWAPIPlayer->researchInProgress(tech))
              this->print(std::string("Tech '" + rest + "''s research is in progress.").c_str());
            else if (this->BWAPIPlayer->techResearched(tech))
              this->print(std::string("Tech '" + rest + "' is researched.").c_str());
            else
              this->print(std::string("Tech '" + rest + "' is not researched.").c_str());
          }
        }
        else this->print(std::string("Unknown value '" + rest + "' - possible values are: playerID, researchState").c_str());
        return true;
      }
    
    prefix = "/log ";
    if (prefix.size() <= message.size() &&
       message.substr(0, prefix.size()) == prefix)
      {
        std::string rest = message.substr(prefix.size(), message.size() - prefix.size());
        if (rest == "shut")
        {
          ScreenLogger::shut = true;
          this->print("Screen log shutted");
        }
        else if (rest == "unshut")
        {
          ScreenLogger::shut = false;
          this->print("Screen log unshutted");
        }
        else 
          this->print(std::string("Unknown log command '" + rest + "' - possible values are: shut, unshut").c_str());
        return true;
      }
    prefix = "/save ";
    if (prefix.size() <= message.size() &&
       message.substr(0, prefix.size()) == prefix)
      {
        std::string rest = message.substr(prefix.size(), message.size() - prefix.size());
        if (rest == "techs")
        {
          std::string fileName = this->configuration->getValue("data_path") + "\\techs";
          Util::FileLogger techsLog(fileName, Util::LogLevel::MicroDetailed, false);
          for (int i = 0; i < BW::TECH_TYPE_COUNT; i++)
            if (BW::TechType((BW::TechID::Enum)i).isValid())
              techsLog.log("%s=%d",BW::TechType((BW::TechID::Enum)i).getName(), i);
          ScreenLogger().log("Techs saved to %s.ini", fileName.c_str());
        }
        else 
          this->print(std::string("Unknown log command '" + rest + "' - possible values are: shut, unshut").c_str());
        return true;
      }      
    return false;
  }
  //------------------------------ ON GAME END ----------------------------------
  void Game::onGameEnd()
  {
    this->setOnStartCalled(false);
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
    this->reselected = false;
    
    BW::Unit** selected = new BW::Unit * [13];
    memcpy(selected, BW::BWXFN_CurrentPlayerSelectionGroup, 4*12);
    selected[12] = NULL;
    return selected;
  }
  //--------------------------------- LOAD SELECTED -----------------------------
  void Game::loadSelected(BW::Unit** selected)
  {
     if (!this->reselected)
     {
       delete [] selected;
       return;
     }

    int unitCount = 0;
    while (selected[unitCount] != NULL)
      unitCount ++;
    if (quietSelect)
    {
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
        int startX =   (i->getPosition().x - i->getType().dimensionLeft())/BW::TILE_SIZE;
        int endX   =   (i->getPosition().x + i->getType().dimensionRight() + BW::TILE_SIZE - 1)/BW::TILE_SIZE; // Division - round up
        int startY =   (i->getPosition().y - i->getType().dimensionUp())/BW::TILE_SIZE;
        int endY =     (i->getPosition().y + i->getType().dimensionDown() + BW::TILE_SIZE - 1)/BW::TILE_SIZE;
        for (int x = startX; x < endX; x++)
          for (int y = startY; y < endY; y++)
            this->unitsOnTile[x][y].push_back(i);
      }
  }
  //--------------------------------------------------------------------------
};