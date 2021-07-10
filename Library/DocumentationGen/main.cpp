#include "main.h"

#include <BWAPI.h>
#include <fstream>
#include <iostream>
#include <set>
#include "helpers.h"

int main()
{
  writeUnitInfo();
  writeColorInfo();
  writeWeaponInfo();
  writeUpgradeInfo();
  writeUnitSizeInfo();
  writeUnitCommandInfo();
  writeTechInfo();
  writeRaceInfo();
  writePlayerTypeInfo();
  writeOrderInfo();
  writeGameTypeInfo();
  writeExplosionTypeInfo();
  writeDamageTypeInfo();
  writeBulletTypeInfo();
  writeErrorInfo();
}
