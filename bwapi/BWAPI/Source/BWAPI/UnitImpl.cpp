#include "UnitImpl.h"

#include <math.h>
#include <limits>
#include <sstream>

#include <Util/Foreach.h>
#include <Util/Gnu.h>

#include <BWAPI/Player.h>
#include <BWAPI/Order.h>
#include "BWAPI/GameImpl.h"
#include <BWAPI/WeaponType.h>
#include "Command.h"
#include "DLLMain.h"
#include "Detours.h"
#include "TemplatesImpl.h"

#include <BW/Unit.h>
#include <BW/Order.h>
#include <BW/Offsets.h>
#include <BW/UnitID.h>

#include "../../Debug.h"

namespace BWAPI
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  UnitImpl::UnitImpl(BW::Unit* originalUnit,
                     u16 index)
      : getOriginalRawData(originalUnit)
      , index(index)
      , userSelected(false)
      , isAlive(false)
      , wasAlive(false)
      , wasCompleted(false)
      , wasAccessible(false)
      , wasVisible(false)
      , staticInformation(false)
      , lastType(UnitTypes::Unknown)
      , lastPlayer(NULL)
      , nukeDetected(false)
      , lastGroundWeaponCooldown(0)
      , lastAirWeaponCooldown(0)
      , startingAttack(false)
      , lastCommandFrame(0)
      , lastCommand()
      , id(-1)
      , clientInfo(NULL)
  {
    self = &data;
    clear();
  }
  //--------------------------------------------- DESTRUCTOR -------------------------------------------------
  UnitImpl::~UnitImpl()
  {
  }
  //--------------------------------------------- SET ID -----------------------------------------------------
  void UnitImpl::setID(int newID)
  {
    id = newID;
  }
  //--------------------------------------------- ISSUE COMMAND ----------------------------------------------
  bool UnitImpl::issueCommand(UnitCommand command)
  {
    if ( !canIssueCommand(command) )
      return false;

    command.unit = this;

    if ( (command.type == UnitCommandTypes::Train ||
          command.type == UnitCommandTypes::Morph) &&
         getType().producesLarva() && command.getUnitType().whatBuilds().first == UnitTypes::Zerg_Larva )
      command.unit = *getLarva().begin();

    ((UnitImpl*)command.unit)->lastCommandFrame = BroodwarImpl.frameCount;
    ((UnitImpl*)command.unit)->lastCommand      = command;
    if (command.type == UnitCommandTypes::Use_Tech_Unit && command.target && 
       (command.extra == TechTypes::Archon_Warp || command.extra == TechTypes::Dark_Archon_Meld))
    {
      ((UnitImpl*)command.target)->lastCommandFrame = BroodwarImpl.frameCount;
      ((UnitImpl*)command.target)->lastCommand      = command;
    }

    if ( BroodwarImpl.addToCommandOptimizer(command) )
      return true;

    if (command.type == UnitCommandTypes::Use_Tech_Unit && command.target &&
       (command.extra == TechTypes::Archon_Warp || command.extra == TechTypes::Dark_Archon_Meld))
    {
      //select both units for archon warp or dark archon meld
      UnitImpl *sel2[2];
      sel2[0] = (UnitImpl*)command.unit;
      sel2[1] = (UnitImpl*)command.target;
      BW::Orders::Select sel(2, sel2);
      botAPM_select++;
      QueueGameCommand(&sel, sel.size);
    }
    else if ( command.type != UnitCommandTypes::Unload || BroodwarImpl.commandOptimizerLevel < 2 )
      ((UnitImpl*)command.unit)->orderSelect();

    BroodwarImpl.executeCommand( command, true);
    return true;
  }
  //--------------------------------------------- SET SELECTED -----------------------------------------------
  void UnitImpl::setSelected(bool selectedState)
  {
    userSelected = selectedState;
  }
  //---------------------------------------------- ORDER SELECT ----------------------------------------------
  void UnitImpl::orderSelect()
  {
    UnitImpl *u = this;
    BW::Orders::Select sel = BW::Orders::Select(1, &u);
    botAPM_select++;
    QueueGameCommand(&sel, sel.size);
  }
  //----------------------------------------------------------------------------------------------------------
  UnitImpl* UnitImpl::BWUnitToBWAPIUnit(BW::Unit* unit)
  {
    if ( !unit )
      return NULL;

    u16 index = (u16)( ((u32)unit - (u32)BW::BWDATA_UnitNodeTable) / 336) & 0x7FF;
    if (index > UNIT_ARRAY_MAX_LENGTH)
    {
      if (BroodwarImpl.invalidIndices.find(index) == BroodwarImpl.invalidIndices.end())
        BroodwarImpl.invalidIndices.insert(index);
      return NULL;
    }
    return BroodwarImpl.getUnitFromIndex(index);
  }

  void UnitImpl::die()
  {
    //set pointers to null so we don't read information from unit table anymore
    getOriginalRawData = NULL;
    index              = 0xFFFF;
    userSelected       = false;
    isAlive            = false;
    wasAlive           = false;
    wasCompleted       = false;
    wasAccessible      = false;
    wasVisible         = false;
    nukeDetected       = false;
    lastType           = UnitTypes::Unknown;
    lastPlayer         = NULL;
    clientInfo         = NULL;
    updateData();
  }

  /* canAccess returns true if the AI module is allowed to access the unit.
    If the unit is visible, returns true.
    If the unit is does not exist, returns false.
    If the unit is not visible and exists, returns true only if CompleteMapInformation is enabled.
  */
  bool UnitImpl::canAccess() const
  {
    if (!this->isAlive)
      return false;
    if (this->isVisible())
      return true;
    //if we get here, the unit exists but is not visible
    if (BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
      return true;
    /* neutral units visible during AIModule::onStart */
    if (Broodwar->getFrameCount() == 0)
      if (this->_getType.isNeutral() || this->_getPlayer->isNeutral())
        return true;
    return false;
  }
  bool UnitImpl::canAccessDetected() const
  {
    if (!canAccess())
      return false;
    if (this->_getPlayer == BroodwarImpl.self())
      return true;
    if (BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation))
      return true;
    /* neutral units visible during AIModule::onStart */
    if (Broodwar->getFrameCount() == 0)
      if (this->_getType.isNeutral())
        return true;
    return self->isDetected;

  }

  //returns true if canAccess() is true and the unit is owned by self (or complete map info is turned on)
  bool UnitImpl::canAccessInside() const
  {
    if (!canAccessDetected())
      return false;
    if (this->_getPlayer == BroodwarImpl.self())
      return true;

    //If we get here, canAccess()==true but unit is not owned by self.
    //Return value depends on state of complete map info flag
    return BroodwarImpl.isFlagEnabled(Flag::CompleteMapInformation);
  }

  //--------------------------------------------- GET NEXT ---------------------------------------------------
  UnitImpl* UnitImpl::getNext() const
  {
    return UnitImpl::BWUnitToBWAPIUnit(getOriginalRawData->next);
  }
  //--------------------------------------------- GET INDEX --------------------------------------------------
  u16 UnitImpl::getIndex() const
  {
    return this->index;
  }
};
