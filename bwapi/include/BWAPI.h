#include <BWAPI/AIModule.h>
#include <BWAPI/AttackType.h>
#include <BWAPI/Color.h>
#include <BWAPI/Constants.h>
#include <BWAPI/CoordinateType.h>
#include <BWAPI/DamageType.h>
#include <BWAPI/Error.h>
#include <BWAPI/Event.h>
#include <BWAPI/EventType.h>
#include <BWAPI/ExplosionType.h>
#include <BWAPI/Flag.h>
#include <BWAPI/Force.h>
#include <BWAPI/Game.h>
#include <BWAPI/Input.h>
#include <BWAPI/Latency.h>
#include <BWAPI/Order.h>
#include <BWAPI/Player.h>
#include <BWAPI/PlayerType.h>
#include <BWAPI/Position.h>
#include <BWAPI/Race.h>
#include <BWAPI/TechType.h>
#include <BWAPI/TilePosition.h>
#include <BWAPI/Unit.h>
#include <BWAPI/UnitCommand.h>
#include <BWAPI/UnitCommandType.h>
#include <BWAPI/UnitSizeType.h>
#include <BWAPI/UnitType.h>
#include <BWAPI/UpgradeType.h>
#include <BWAPI/WeaponType.h>

namespace BWAPI
{
  /** You have to call this from your AIModule Dllmain function.
   *
   * \code
   * BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
   * {
   *   switch (ul_reason_for_call)
   *   {
   *     case DLL_PROCESS_ATTACH:
   *       BWAPI::BWAPI_init();
   *       break;
   *     case DLL_PROCESS_DETACH:
   *       break;
   *   }
   *
   *   return TRUE;
   * }
   * \endcode */
  void BWAPI_init();
}
