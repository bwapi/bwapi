#include "UnitProc.h"
#include <BWAPI.h>

class Transport : public UnitProc
{
public:
  Transport();
  Transport(Unit *pUnit);
  ~Transport();
  virtual bool GenericIdle();
};
