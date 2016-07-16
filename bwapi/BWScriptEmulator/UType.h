#pragma once
#include <BWAPI.h>

class UType : public BWAPI::UnitType
{
public:
  UType();
  UType(int id);
  ~UType();
  const char *name();
  int aiFlag();
  int aiIdle();
};
