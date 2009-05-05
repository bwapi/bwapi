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
#include <BW/GameType.h>

#include "Globals.h"

namespace BWAPI 
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Game::Game()
  :onStartCalled(false)
  ,unitsOnTileData(0,0)
  ,quietSelect(true)
  ,enabled(true)
  {
    units.reserve(BW::UNIT_ARRAY_MAX_LENGTH);
    try
    {
     this->configuration = new Util::Dictionary("bwapi-data\\bwapi.ini");
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
                                &BW::BWDATA_UnitNodeTable->unit[i],
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

    for (int i = 0; i < BW::UNIT_ARRAY_MAX_LENGTH; i++)
      delete unitArray[i];

    for (int i = 0; i < 12; i++)
      delete players[i];

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
    try
    {
      if (!this->isOnStartCalled())
        this->onGameStart();
      if (!this->enabled)
        return;

      memcpy(this->unitArrayCopy, BW::BWDATA_UnitNodeTable, sizeof(BW::UnitArray));
      memcpy(this->unitArrayCopyLocal, BW::BWDATA_UnitNodeTable, sizeof(BW::UnitArray));
      for (int i = 0; i < BW::PLAYER_COUNT; i++)
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
    }
    catch (GeneralException& exception)
    {
      FILE*f = fopen("bwapi-error","wt");
      fprintf(f, "Exception caught inside Game::update: %s", exception.getMessage().c_str());
      fclose(f);
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
    return *(BW::BWDATA_InGame) == 0;
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
    
    char* txtout = buffer;
    if (*BW::BWDATA_InGame)
      __asm
      {
        pushad
        push 0       // Unknown
        mov eax, 8   // Player ID (-1 for notification area)
        push txtout  // Text
        call dword ptr [BW::BWFXN_PrintText]
        popad
      }
    else
      printPublic(txtout); // until lobby print private text is found
  }
  //---------------------------------------------- PRINT WITH PLAYER ID --------------------------------------
  void Game::printEx(s32 pID, const char *text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf(buffer, BUFFER_SIZE, text, ap); 
    va_end(ap);
    
    char* txtout = buffer;
    if (*BW::BWDATA_InGame)
      __asm
      {
        pushad
        push 0       // Unknown
        mov eax, pID   // Player ID (-1 for notification area)
        push txtout  // Text
        call dword ptr [BW::BWFXN_PrintText]
        popad
      }
    else
      printPublic(txtout); // until lobby print private text is found
  }
  //---------------------------------------------- PRINT PUBLIC ----------------------------------------------
  void Game::printPublic(const char *text, ...)
  {
    va_list ap;
    va_start(ap, text);
    vsnprintf(buffer, BUFFER_SIZE, text, ap); 
    va_end(ap);
    
    char* txtout = buffer;
    if (*(BW::BWDATA_InGame))
    {
      memcpy(BW::BWDATA_SendTextRequired, &BW::BWDATA_FullMask, 2);
      __asm
      {
        pushad
        mov esi, txtout
        call [BW::BWFXN_SendPublicCallTarget]
        popad
      }
    }
    else
      __asm
      {
        pushad
        mov edi, txtout
        call [BW::BWFXN_SendLobbyCallTarget]
        popad
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
    map.load();

    if (*(BW::BWDATA_InReplay))
      return;

    for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
      if (strcmp(BW::BWDATA_CurrentPlayer, this->players[i]->getName()) == 0)
        this->BWAPIPlayer = this->players[i];

    if (this->BWAPIPlayer == NULL ||
        this->BWAPIPlayer->getForceName() == "Observers" ||
        this->BWAPIPlayer->getForceName() == "Observer")
      {
        this->BWAPIPlayer = NULL;
        return;
      }

    for (int i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
      if ((this->players[i]->getOwner() == BW::PlayerType::Computer ||
           this->players[i]->getOwner() == BW::PlayerType::Human ||
           this->players[i]->getOwner() == BW::PlayerType::ComputerSlot) &&
           this->opponent == NULL &&
           this->players[i]->getForceName() != "Observers" &&
           this->players[i]->getForceName() != "Observer" &&
           this->BWAPIPlayer->getAlliance(i) == 0)
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
      else if (parsed[1] == "unitCount")
       { 
        BW::UnitType unit = BW::UnitID::All;
         if (parsed[2] != "")
         {
           std::string unitName= message.substr(strlen("/get unitCount "), message.size() - strlen("/get unitCount "));
           unit = this->unitNameToType[unitName];
           if (unit == BW::UnitID::None)
           { 
             this->print("Unknown unit name '%s'", unitName.c_str());
             return true;
           }
         }
         this->print("Count of %s's:", unit.getName());
         for (u8 i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
           this->print("Counted %d units for player %d.", this->players[i]->getAllUnits(unit), i+1);
       }
       else if (parsed[1] == "unfinishedCount")
       { 
         BW::UnitType unit = BW::UnitID::All;
         if (parsed[2] != "")
         {
           std::string unitName= message.substr(strlen("/get unfinishedCount "), message.size() - strlen("/get unfinishedCount "));
           unit = this->unitNameToType[unitName];
           if (unit == BW::UnitID::None)
           { 
             this->print("Unknown unit name '%s'", unitName);
             return true;
           }
         }
         this->print("Count of unfinished %s's:", unit.getName());
         for (u8 i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
           this->print("Counted %d units for player %d.", this->players[i]->getIncompleteUnits(unit), i+1);
       }
       else if (parsed[1] == "completedCount")
       { 
         BW::UnitType unit = BW::UnitID::All;
         if (parsed[2] != "")
         {
           std::string unitName= message.substr(strlen("/get completedCount "), message.size() - strlen("/get completedCount "));
           unit = this->unitNameToType[unitName];
           if (unit == BW::UnitID::None)
           { 
             this->print("Unknown unit name '%s'", unitName);
             return true;
           }
         }
         this->print("Count of completed %s's:", unit.getName());
         for (u8 i = 0; i < BW::PLAYABLE_PLAYER_COUNT; i++)
           this->print("Counted %d units for player %d.", this->players[i]->getCompletedUnits(unit), i+1);
       }

      else this->print("Unknown value '%s' - possible values are: playerID, researchState, upgradeState, unitCount", parsed[1].c_str());
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
        BW::BWDATA_PlayerResources->minerals.player[this->BWAPIPlayer->getID()] = 10000;
        BW::BWDATA_PlayerResources->gas.player[this->BWAPIPlayer->getID()] = 10000;
        for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        {
          BW::UnitType type((BW::UnitID::Enum)i);
          if (type.isValid())
            BW::BWDATA_BuildTime->buildTime[i] = 16;
        }
        this->print("BWAPI gas/mineral/build time cheat activated (only local ofcourse)");
      }
      else if (parsed[1] == "ore")
      {
        BW::BWDATA_PlayerResources->minerals.player[this->BWAPIPlayer->getID()] += 10000;
        this->print("BWAPI mineral cheat activated (only local ofcourse)");
      }
      else if (parsed[1] == "gas")
      {
        BW::BWDATA_PlayerResources->gas.player[this->BWAPIPlayer->getID()] += 10000;
        this->print("BWAPI gas cheat activated (only local ofcourse)");
      }
      else if (parsed[1] == "speed")
      {
        for (u16 i = 0; i < BW::UNIT_TYPE_COUNT; i++)
        {
          BW::UnitType type((BW::UnitID::Enum)i);
          if (type.isValid())
            BW::BWDATA_BuildTime->buildTime[i] = 16;
        }
        this->print("BWAPI speed cheat activated (only local ofcourse)");
      }
      else 
        this->print("Unknown log command '%s''s - possible values are: all, ore, gas, speed", parsed[1].c_str());
      return true;
    }
    else if (parsed[0] == "/set")
    {
      if (parsed[1] == "range")
      {
        u16 range = atoi(parsed[2].c_str());
        size_t length = strlen("/set range  ") + parsed[2].size();
        std::string name = message.substr(length, message.size() - length);
        BW::UnitType unit = unitNameToType[name];
        if (unit == BW::UnitID::None)
        {
          this->print("Unknown unit name '%s'", name.c_str());
          return true;
        }
        BW::BWDATA_WeaponRange->weapon[BW::BWDATA_UnitGroundWeapon->unit[unit.getID()]] = range;
        BW::BWDATA_UnitSeekRange->unit[unit.getID()] = range << 8;
        this->print("Set range of '%s' to %d", name.c_str(), range);
      }
      else if (parsed[1] == "sight")
      {
        u8 range = atoi(parsed[2].c_str());
        if (range > 11)
        {
          this->print("Max sight range size is 11");
          return true;
        }
        size_t length = strlen("/set sight  ") + parsed[2].size();
        std::string name = message.substr(length, message.size() - length);
        BW::UnitType unit = unitNameToType[name];
        if (unit == BW::UnitID::None)
        {
          this->print("Unknown unit name '%s'", name.c_str());
          return true;
        }
        BW::BWDATA_UnitSightRange->unit[unit.getID()] = range;
        this->print("Set range of '%s' to %d", name.c_str(), range);
      }
      else
        this->print("Unknown thing to set '%s'", parsed[1].c_str());
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
  //----------------------------------------------- PAUSE GAME -----------------------------------------------
  void Game::pauseGame()
  { 
    this->IssueCommand((PBYTE)&BW::Orders::PauseGame(),sizeof(BW::Orders::PauseGame));
  }
  //---------------------------------------------- RESUME GAME -----------------------------------------------
  void Game::resumeGame()
  { 
    this->IssueCommand((PBYTE)&BW::Orders::ResumeGame(),sizeof(BW::Orders::ResumeGame));
  }
  //---------------------------------------------- GET MOUSE X -----------------------------------------------
  int Game::getMouseX() const
  {
    return *(BW::BWDATA_MouseX);
  }
  //---------------------------------------------- GET MOUSE Y -----------------------------------------------
  int Game::getMouseY() const
  {
   return *(BW::BWDATA_MouseY);
  }
  //---------------------------------------------- GET SCREEN X ----------------------------------------------
  int Game::getScreenX() const
  {
   return *(BW::BWDATA_ScreenX);
  }
  //---------------------------------------------- GET SCREEN Y ----------------------------------------------
  int Game::getScreenY() const
  {
   return *(BW::BWDATA_ScreenY);
  }
  //----------------------------------------------------------------------------------------------------------
  #pragma warning(push)
  #pragma warning(disable:4312)
  void Game::refresh()
  {
    void (_stdcall* refresh)(void) = (void (_stdcall*) ())BW::BWFXN_Refresh;
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
    memcpy(selected, BW::BWDATA_CurrentPlayerSelectionGroup, 4*12);
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
      BW::selectUnits(unitCount, selected);
    else
      BW::selectUnitsHelperSTD(unitCount, selected, true, true);
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
    return Unit::BWUnitToBWAPIUnit(*BW::BWDATA_UnitNodeTable_FirstElement);
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
        if (this->map.buildable(x,y))
          fprintf(f, "%d", this->unitsOnTile(x,y).size());
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
    this->unitsOnTileData.resize(Map::getWidth(), Map::getHeight());
    for (int y = 0; y < Map::getHeight(); y++)
      for (int x = 0; x < Map::getWidth(); x++)
        this->unitsOnTileData[x][y].clear();
    for each (Unit* i in this->units)
      if (i->isValid())
      {
        int startX =   (i->getPosition().x - i->getType().dimensionLeft())/BW::TILE_SIZE;
        int endX   =   (i->getPosition().x + i->getType().dimensionRight() + BW::TILE_SIZE - 1)/BW::TILE_SIZE; // Division - round up
        int startY =   (i->getPosition().y - i->getType().dimensionUp())/BW::TILE_SIZE;
        int endY =     (i->getPosition().y + i->getType().dimensionDown() + BW::TILE_SIZE - 1)/BW::TILE_SIZE;
        for (int x = startX; x < endX; x++)
          for (int y = startY; y < endY; y++)
            this->unitsOnTileData[x][y].push_back(i);
      }
  }
  //---------------------------------------------- GET UNIT TYPE ---------------------------------------------
  BW::UnitType Game::getUnitType(std::string &name) const
  {
    std::map<std::string, BW::UnitType>::const_iterator i=this->unitNameToType.find(name);
    if (i==this->unitNameToType.end())
    {
      return BW::UnitType();
    }
    else
    {
      return i->second;
    }
  }
  //---------------------------------------------- GET TECH TYPE ---------------------------------------------
  BW::TechType Game::getTechType(std::string &name) const
  {
    std::map<std::string, BW::TechType>::const_iterator i=this->techNameToType.find(name);
    if (i==this->techNameToType.end())
    {
      return BW::TechType();
    }
    else
    {
      return i->second;
    }
  }
  //--------------------------------------------- GET UPGRADE TYPE -------------------------------------------
  BW::UpgradeType Game::getUpgradeType(std::string &name) const
  {
    std::map<std::string, BW::UpgradeType>::const_iterator i=this->upgradeNameToType.find(name);
    if (i==this->upgradeNameToType.end())
    {
      return BW::UpgradeType();
    }
    else
    {
      return i->second;
    }
  }
  //--------------------------------------------- GET FRAME COUNT --------------------------------------------
  int Game::getFrameCount() const
  {
    return this->frameCount;
  }
  //--------------------------------------------- UNITS ON TILE ----------------------------------------------
  std::list<Unit*> Game::unitsOnTile(int x, int y) const
  {
    return unitsOnTileData[x][y];
  }
  //----------------------------------------------------------------------------------------------------------
};