#include "Game.h"

#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

#include <Util/FileLogger.h>
#include <Util/Dictionary.h>
#include <Util/Sentence.h>
#include <Util/Exceptions.h>
#include <Util/Strings.h>

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

#include "Globals.h"

namespace BWAPI 
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Game::Game()
  :onStartCalled(false)
  ,unitsOnTile(0,0)
  ,quietSelect(true)
  ,enabled(true)
  {
    units.reserve(BW::UNIT_ARRAY_MAX_LENGTH);
    try
    {
     this->configuration = new Util::Dictionary("bwapi.ini");
     config = this->configuration;
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error","wt");
      fprintf(f, "Couldn't load configuration file bwapi.ini because: %s", exception.getMessage().c_str());
      fclose(f);
    }
    
    try
    {
      this->commandLog        = new Util::FileLogger(config->get("log_path") + "\\commands", Util::LogLevel::MicroDetailed);
      this->newOrderLog       = new Util::FileLogger(config->get("log_path") + "\\new_orders", Util::LogLevel::MicroDetailed);
      this->badAssumptionLog  = new Util::FileLogger(config->get("log_path") + "\\bad_assumptions", Util::LogLevel::MicroDetailed);
      this->newUnitLog        = new Util::FileLogger(config->get("log_path") + "\\new_unit_id", Util::LogLevel::MicroDetailed);
      this->unitSum           = new Util::FileLogger(config->get("log_path") + "\\unit_sum", Util::LogLevel::MicroDetailed);
      this->fatalError        = new Util::FileLogger(config->get("log_path") + "\\FATAL-ERROR", Util::LogLevel::MicroDetailed);
      
      int intForSScanf;
      
      Util::DictionaryFile unitNames(config->get("unit_names_path"));
      for each (Util::Sentence* i in unitNames.usedLines)
      {
        sscanf(i->getSentence().c_str(), "0x%02X", &intForSScanf);
        BW::UnitID::Enum unit((BW::UnitID::Enum) intForSScanf);
        this->unitNameToType.insert(std::pair<std::string, BW::UnitType>(i->getKey(), unit));
      }

      Util::DictionaryFile techNames(config->get("tech_names_path"));
      for each (Util::Sentence* i in techNames.usedLines)
      {
        sscanf(i->getSentence().c_str(), "0x%02X", &intForSScanf);
        BW::TechID::Enum tech((BW::TechID::Enum) intForSScanf);
        this->techNameToType.insert(std::pair<std::string, BW::TechType>(i->getKey(), tech));
      }
       
      Util::DictionaryFile upgradeNames(config->get("upgrade_names_path"));
      for each (Util::Sentence* i in upgradeNames.usedLines)
      {
        sscanf(i->getSentence().c_str(), "0x%02X", &intForSScanf);
        BW::UpgradeID::Enum upgrade((BW::UpgradeID::Enum) intForSScanf);
        this->upgradeNameToType.insert(std::pair<std::string, BW::UpgradeType>(i->getKey(), upgrade));
      }

      unitArrayCopy = new BW::UnitArray;
      unitArrayCopyLocal = new BW::UnitArray;

      for (int i = 0; i < 12; i++)
        players[i] = new Player((u8)i);    
      
      for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
        unitArray[i] = new Unit(&unitArrayCopy->unit[i], 
                                &BW::BWXFN_UnitNodeTable->unit[i],
                                &unitArrayCopyLocal->unit[i],
                                i);

      this->latency = BW::Latency::BattlenetLow; // @todo read from the address in update
      //this->latency = BW::Latency::LanLow;
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error","wt");
      fprintf(f, "Exception caught inside Game constructor: %s", exception.getMessage().c_str());
      fclose(f);
    }
  }
  //----------------------------------------------- DESTRUCTOR -----------------------------------------------
  Game::~Game()
  {
    delete unitArrayCopy;
    delete unitArrayCopyLocal;

    for (int i = 0; i < 12; i++)
      delete players[i];

    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete unitArray[i];
    
    delete this->commandLog;
    delete this->newOrderLog;
    delete this->badAssumptionLog;
    delete this->newUnitLog;
    delete this->unitSum;
    delete this->fatalError;
    delete this->configuration;
  }
  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  void Game::IssueCommand(PBYTE pbBuffer, u32 iSize) 
  {
    __asm 
    {
      MOV ECX, pbBuffer
      MOV EDX, iSize
      CALL [BW::BWFXN_IssueCommand]
    }
  }
  //------------------------------------------------- UPDATE -------------------------------------------------
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
   
    this->units.clear();
    for (Unit* i = this->getFirst(); i != NULL; i = i->getNext())
      this->units.push_back(i);

    while (this->commandBuffer.size() > this->getLatency())
      this->commandBuffer.erase(this->commandBuffer.begin());
    this->commandBuffer.push_back(std::vector<Command *>());
    for (unsigned int i = 0; i < this->commandBuffer.size(); i++)
       for (unsigned int j = 0; j < this->commandBuffer[i].size(); j++)
         this->commandBuffer[i][j]->execute();
    this->frameCount ++;
    this->logUnknownOrStrange();
    this->updateUnitsOnTile();    
    
    for (int i = 0; i < 12; i++)
      for (u16 j = 0; j < BW::UNIT_TYPE_COUNT; j++)
      {
        this->players[i]->unitTypeCount[j] = 0;
        this->players[i]->allUnitTypeCount[j] = 0;
      }
    
    for each (Unit* i in this->units)
    //for (Game::Units::iterator i = this->units.begin(); i != this->units.end(); ++i)
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
  //------------------------------------------- IS ON START CALLED -------------------------------------------
  bool Game::isOnStartCalled() const
  {
    return this->onStartCalled;
  }
  //------------------------------------------ SET ON START CALLED -------------------------------------------
  void Game::setOnStartCalled(bool onStartCalled)
  {
    this->onStartCalled = onStartCalled;
  }
  #pragma warning(push)
  #pragma warning(disable:4312)
  //----------------------------------------------- IS IN GAME -----------------------------------------------
  bool Game::isInGame() const
  {
    return *(BW::BWFXN_InGame) == 0;
  }
  const int BUFFER_SIZE = 2048;
  char buffer[BUFFER_SIZE];
  //------------------------------------------------- PRINT --------------------------------------------------
  void Game::print(const char *text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf(buffer, BUFFER_SIZE, text, ap); 
    va_end(ap);
    void (_stdcall* sendText)(const char *) = (void (_stdcall*) (const char *))BW::BWXFN_PrintText;
	  	sendText(buffer);
  }
  //---------------------------------------------- PRINT PUBLIC ----------------------------------------------
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
  //---------------------------------------------- CHANGE SLOT -----------------------------------------------
  void Game::changeSlot(BW::Orders::ChangeSlot::Slot slot, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeSlot(slot, slotID),3); 
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void Game::changeRace(BW::Race::Enum race, u8 slotID)
  {
    IssueCommand((PBYTE)&BW::Orders::ChangeRace(race, slotID),3); 
  }
  //----------------------------------------- ADD TO COMMAND BUFFER ------------------------------------------
  void Game::addToCommandBuffer(Command *command)
  {
    this->reselected = true;
    command->execute();
    this->commandBuffer[this->commandBuffer.size() - 1].push_back(command);
    this->commandLog->log("(%4d) %s", this->frameCount, command->describe().c_str());
  }
  //--------------------------------------------- ON GAME START ----------------------------------------------
  void Game::onGameStart()
  {
    this->frameCount = 0;
    this->setOnStartCalled(true);
    this->BWAPIPlayer = NULL;
    this->opponent = NULL;
    for (int i = 0; i < 8; i++)
      if (config->get("bwapi_name") == this->players[i]->getName())
          this->BWAPIPlayer = this->players[i];
      else
        if (strcmp(this->players[i]->getName(),"") != 0 &&
           (opponent == NULL || this->players[i]->getRace() != BW::Race::Terran))
          this->opponent = this->players[i];
  }
  //---------------------------------------------- ON SEND TEXT ----------------------------------------------
  bool Game::onSendText(const char* text)
  {
    std::string message = text;
    std::vector<std::string> parsed = Util::Strings::splitString(message, " ");
    if (parsed.size() < 3)
      parsed.push_back("");
    if (parsed.size() < 3)
      parsed.push_back("");
    if (parsed[0] == "/bwapi")
    {
      if (parsed[1] == "on")
      {
        this->enabled = true;
        this->print("bwapi enabled");
      }
      else if (parsed[1] == "off")
      {
        this->enabled = false;
        this->print("bwapi disabled");
      }
      else 
        this->print("Unknown command '%s''s - possible commands are: on, off", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/unit")
    {
      if (parsed[1] == "info")
      {
        BW::Unit** selected = this->saveSelected();
        for (u16 i = 0; selected[i] != NULL; i++)
          this->print(BWAPI::Unit::BWUnitToBWAPIUnit(selected[i])->getName().c_str());
      }
      else
        this->print("Unknown command '%s''s - possible commands are: info", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/get")
    {
      if (parsed[1] == "playerID")
      {
        char text[100];
        sprintf(text, "Current player id = %d", this->BWAPIPlayer->getID());
        this->print(text);
      }
      else if (parsed[1] == "researchState")
      {
        std::string techName = message.substr(strlen("/get researchState "), message.size() - strlen("/get researchState "));
        BW::TechType tech = this->techNameToType[techName];
        if (tech == BW::TechID::None)
          this->print("Unknown tech name '%s'", techName.c_str());
        else
        {
          if (this->BWAPIPlayer->researchInProgress(tech))
            this->print("Tech '%s's research is in progress.", techName.c_str());
          else if (this->BWAPIPlayer->techResearched(tech))
            this->print("Tech '%s''s is researched.", techName.c_str());
          else
            this->print("Tech '%s''s is not researched.", techName.c_str());
        }
      }
      else if (parsed[1] == "upgradeState")
      {
        std::string upgradeName = message.substr(strlen("/get upgradeState "), message.size() - strlen("/get upgradeState "));
        BW::UpgradeType upgrade = this->upgradeNameToType[upgradeName];
        if (upgrade == BW::UpgradeID::None)
          this->print("Unknown upgrade name '%s'", upgradeName.c_str());
        else
        {
          this->print("Level is %u.", this->BWAPIPlayer->upgradeLevel(upgrade));
          if (this->BWAPIPlayer->upgradeInProgress(upgrade))
            this->print("Another level in progress");
          else
            this->print("Another level is not in progress");
        }
      }
      else this->print("Unknown value '%s' - possible values are: playerID, researchState, upgradeState", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/log")
    {
      if (parsed[1] == "shut")
      {
        ScreenLogger::shut = true;
        this->print("Screen log shutted");
      }
      else if (parsed[1] == "unshut")
      {
        ScreenLogger::shut = false;
        this->print("Screen log unshutted");
      }
      else 
        this->print("Unknown log command '%s''s - possible values are: shut, unshut", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/cheat")
    {
      if (parsed[1] == "all")
      {
        BW::BWXFN_PlayerResources->minerals.player[this->BWAPIPlayer->getID()] = 10000;
        BW::BWXFN_PlayerResources->gas.player[this->BWAPIPlayer->getID()] = 10000;
        for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        {
          BW::UnitType type((BW::UnitID::Enum)i);
          if (type.isValid())
            BW::BWXFN_BuildTime->buildTime[i] = 6;
        }
        this->print("BWAPI gas/mineral/build time cheat activated (only local ofcourse)");
      }
      else 
        this->print("Unknown log command '%s''s - possible values are: all", parsed[1].c_str());
      return true;
    }
    return false;
  }
  //---------------------------------------------- ON GAME END -----------------------------------------------
  void Game::onGameEnd()
  {
    this->setOnStartCalled(false);
  }
  //----------------------------------------------- START GAME -----------------------------------------------
  void Game::startGame()
  { 
    this->IssueCommand((PBYTE)&BW::Orders::StartGame(),sizeof(BW::Orders::StartGame));
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int Game::getMouseX() const
  {
    return *(BW::BWXFN_MouseX);
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int Game::getMouseY() const
  {
   return *(BW::BWXFN_MouseY);
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int Game::getScreenX() const
  {
   return *(BW::BWXFN_ScreenX);
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int Game::getScreenY() const
  {
   return *(BW::BWXFN_ScreenY);
  }
  //----------------------------------------------------------------------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  void Game::refresh()
  {
    void (_stdcall* refresh)(void) = (void (_stdcall*) ())BW::BWXFN_Refresh;
 	 	refresh();
  }
  #pragma warning(pop)
  //----------------------------------------------------------------------------------------------------------
  Unit* Game::getUnit(int index)
  {
    return this->unitArray[index];
  }
  //--------------------------------------------- SAVE SELECTED ----------------------------------------------
  BW::Unit** Game::saveSelected()
  {
    this->reselected = false;
    
    BW::Unit** selected = new BW::Unit * [13];
    memcpy(selected, BW::BWXFN_CurrentPlayerSelectionGroup, 4*12);
    selected[12] = NULL;
    return selected;
  }
  //--------------------------------------------- LOAD SELECTED ----------------------------------------------
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
  //-------------------------------------------- ON CANCEL TRAIN ---------------------------------------------
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
      Util::Logger::globalLog->log("Exception in onCancelTrain: %s", exception.getMessage().c_str());
    }
  }
  //--------------------------------------------- ON REMOVE UNIT ---------------------------------------------
  void Game::onRemoveUnit(BW::Unit *unit)
  {
  }
  //----------------------------------------- LOG UNKNOWN OR STRANGE -----------------------------------------
  void Game::logUnknownOrStrange()
  {
    for each (Unit* i in this->units)
      if (!i->getType().isValid())
        this->newUnitLog->log("%s", i->getName().c_str());
  }
  //--------------------------------------------- LOG UNIT LIST ----------------------------------------------
  void Game::logUnitList()
  {
    this->unitSum->log("----------------------------------------");
    for each (Unit* i in this->units)
       this->unitSum->log("%s", i->getName().c_str());
    this->unitSum->log("----------------------------------------");
  }
  //----------------------------------------------- GET FIRST ------------------------------------------------
  Unit* Game::getFirst()
  {
    return Unit::BWUnitToBWAPIUnit(*BW::BWXFN_UnitNodeTable_FirstElement);
  }
  //---------------------------------------------- GET LATENCY -----------------------------------------------
  BW::Latency::Enum Game::getLatency()
  {
    return this->latency;
  }
  //--------------------------------------- PRINT UNIT COUNT PER TILE ----------------------------------------
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
  //------------------------------------------ UPDATE UNITS ON TILE ------------------------------------------
  void Game::updateUnitsOnTile()
  {
    this->unitsOnTile.resize(Map::getWidth(), Map::getHeight());
    for (int y = 0; y < Map::getHeight(); y++)
      for (int x = 0; x < Map::getWidth(); x++)
        this->unitsOnTile[x][y].clear();
    for each (Unit* i in this->units)
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
  //----------------------------------------------------------------------------------------------------------
};