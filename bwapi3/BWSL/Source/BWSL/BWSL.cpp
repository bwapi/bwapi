#include "BWSL.h"
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
namespace BWSL
{
//private:

  Error lastError;
  void resetError()
  {
    lastError = Errors::None;
  }

//public:
  //----------------------------------- GET VERSION -----------------------------------------------
  BWSL_EXPORT int getVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  BWSL_EXPORT bool connect()
  {
    resetError();
    try
    {
      return BWAPI::connect();
    }
    catch(GeneralException &)
    {
      lastError = Errors::Connection_Failure;
      return 0;
    }
    return 1;
  }
  //----------------------------------- TAKE OVER -------------------------------------------------
  BWSL_EXPORT bool takeover(AIModule& aiModule)
  {
    resetError();
    try
    {
      for(;;)
      {
        BWAPI::CallTypeId purpose = BWAPI::waitForEvent();

        // react upon bridge state
        switch(purpose)
        {
        case BWAPI::CallTypeIds::OnMatchStart:
          {
            aiModule.onStart();
          }break;
        case BWAPI::CallTypeIds::OnFrame:
          {
            // TODO: implement
            //BWAPI::GetUserInput()
            //aiModule.onSendText(input);
            aiModule.onFrame();
          }break;
        case BWAPI::CallTypeIds::OnDisconnect:
          {
          }return true;
        }
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
  BWSL_EXPORT Error getLastError()
  {
    // returns the last error encountered in BWAPI
    return lastError;
  }
  //----------------------------------- -----------------------------------------------------------
}
