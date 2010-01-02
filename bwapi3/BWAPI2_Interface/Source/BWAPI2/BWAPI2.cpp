#include "BWAPI2.h"
#include "AIModule.h"
#include "Error.h"
#include "Race.h"
#include "DamageType.h"
#include "ExplosionType.h"
#include "Order.h"
#include "TechType.h"
#include "PlayerType.h"
#include "UpgradeType.h"
#include "WeaponType.h"
#include "UnitSizeType.h"
#include "UnitType.h"
#include "AttackType.h"
#include "Color.h"

#include <BWAPI\all.h>

#include <Util\Version.h>
#include <Util\Exceptions.h>

// singleton base class
namespace BWAPI2
{
//private:

  Error lastError;
  void resetError()
  {
    lastError = Errors::None;
  }

//public:
  //----------------------------------- GET VERSION -----------------------------------------------
  BWAPI2_EXPORT int getVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  BWAPI2_EXPORT int connect()
  {
    resetError();
    try
    {
      BWAPI::Connect();
    }
    catch(GeneralException &)
    {
      lastError = Errors::Connection_Failure;
      return 0;
    }
    return 1;
  }
  //----------------------------------- TAKE OVER -------------------------------------------------
  BWAPI2_EXPORT bool takeover(AIModule& aiModule)
  {
    resetError();
    try
    {
      BWAPI::CallTypeId purpose = BWAPI::WaitForEvent();

      // react upon bridge state
      switch(purpose)
      {
      case BWAPI::CallTypeIds::OnMatchStart:
        {
          BWAPI2::Races::init();
          BWAPI2::DamageTypes::init();
          BWAPI2::ExplosionTypes::init();
          BWAPI2::Orders::init();
          BWAPI2::TechTypes::init();
          BWAPI2::PlayerTypes::init();
          BWAPI2::UpgradeTypes::init();
          BWAPI2::WeaponTypes::init();
          BWAPI2::UnitSizeTypes::init();
          BWAPI2::UnitTypes::init();
          BWAPI2::AttackTypes::init();
          BWAPI2::Errors::init();
          BWAPI2::Colors::init();

          aiModule.onStart();
        }break;
      case BWAPI::CallTypeIds::OnFrame:
        {
          // TODO: implement
          //BWAPI::GetUserInput()
          //aiModule.onSendText(input);
          aiModule.onFrame();
        }break;
      }
    }
    catch(GeneralException &)
    {
      lastError = Errors::Connection_Failure;
      return false;
    }
    return true;
  }
  //----------------------------------- GET LAST ERROR --------------------------------------------
  BWAPI2_EXPORT Error getLastError()
  {
    // returns the last error encountered in BWAPI
    return lastError;
  }
  //----------------------------------- -----------------------------------------------------------
}
