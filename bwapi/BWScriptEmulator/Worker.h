#include "UnitProc.h"
#include <BWAPI.h>

class Worker : public UnitProc
{
public:
  Worker();
  Worker(Unit *pUnit);
  ~Worker();
  virtual bool GenericIdle();
};
