#include "UnitProc.h"
#include <BWAPI.h>

class ScienceVessel : public UnitProc
{
public:
  ScienceVessel();
  ScienceVessel(Unit *pUnit);
  ~ScienceVessel();
  virtual bool GenericIdle();
};
