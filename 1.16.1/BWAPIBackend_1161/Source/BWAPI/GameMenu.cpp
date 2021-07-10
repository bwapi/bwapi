#include "GameImpl.h"

#include "../Config.h"
#include "../DLLMain.h"
#include "../NewHackUtil.h"

#include <BWAPI/PlayerType.h>
#include <BWAPI/Race.h>

#include "../BW/MenuPosition.h"
#include "../BW/Dialog.h"
#include "../BW/OrderTypes.h"
#include <algorithm>

#include <BWAPI4/Event.h>

namespace BWAPI4
{
  //------------------------------------------- LOAD AUTO MENU DATA ------------------------------------------
  void GameImpl::loadAutoMenuData()
  {
    this->autoMenuManager.reloadConfig();

    // Not related to the auto-menu, but it should be loaded every time auto menu data gets reloaded
    this->seedOverride = LoadConfigInt("starcraft", "seed_override", std::numeric_limits<decltype(this->seedOverride)>::max());
    this->speedOverride = LoadConfigInt("starcraft", "speed_override", std::numeric_limits<decltype(this->speedOverride)>::min());
    this->wantDropPlayers = LoadConfigStringUCase("starcraft", "drop_players", "ON") == "ON";
  }
  //--------------------------------------------- GET LOBBY STUFF --------------------------------------------
  Race GameImpl::getMenuRace(const std::string &sChosenRace)
  {
    // Determine the current player's race
    Race race;
    if ( sChosenRace == "RANDOMTP" )
      race = rand() % 2 == 0 ? Races::Terran : Races::Protoss;
    else if ( sChosenRace == "RANDOMTZ" )
      race = rand() % 2 == 0 ? Races::Terran : Races::Zerg;
    else if ( sChosenRace == "RANDOMPZ" )
      race = rand() % 2 == 0 ? Races::Protoss : Races::Zerg;
    else if ( sChosenRace == "RANDOMTPZ" )
      race = Race{rand() % 3};
    else
      race = Race::getType(sChosenRace);
    return race;
  }

  //---------------------------------------------- ON MENU FRAME ---------------------------------------------
  void GameImpl::onMenuFrame()
  {
    //this function is called each frame while starcraft is in the main menu system (not in-game).
    this->inGame        = false;

    events.push_back(Event::MenuFrame());
    this->server.update();

    this->autoMenuManager.onMenuFrame();
  }
  //---------------------------------------------- CHANGE RACE -----------------------------------------------
  void GameImpl::_changeRace(int slot, BWAPI::Race race)
  {
    if ( race == Races::Unknown || race == Races::None )
      return;

    // Obtain the single player dialog
    BW::dialog *custom = BW::FindDialogGlobal("Create");
    if ( custom )
    {
      slot = std::clamp(slot, 0, 7);
      // Apply the single player change
      BW::dialog *slotCtrl = custom->findIndex((short)(28 + slot));  // 28 is the CtrlID of the first slot
      if ( slotCtrl && (int)slotCtrl->getSelectedValue() != race )
        slotCtrl->setSelectedByValue(race);
      return;
    }

    // Obtain the multi-player dialog
    custom = BW::FindDialogGlobal("Chat");
    if ( !custom ) // return if not found
      return;

    // Obtain the countdown control
    BW::dialog *countdown = custom->findIndex(24);
    if ( !countdown ) // return if not found
      return;

    // Obtain the countdown control's text
    const char *txt = countdown->getText();
    if ( txt && txt[0] && txt[0] < '2' )
      return; // return if the countdown is less than 2

    // Send the change race command for multi-player
    QUEUE_COMMAND(BW::Orders::RequestChangeRace, slot, race);
  }
}

