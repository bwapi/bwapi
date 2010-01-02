#include <stdio.h>
#include <tchar.h>

#include <BWAPI2\all.h>

#include "windows.h"

#include <string>

int _tmain(int argc, _TCHAR* argv[])
{
  printf("%s\n", BWAPI2::UnitTypes::Critter_Kakaru.getSubLabel().c_str());
  system("pause");
  return 0;
}

