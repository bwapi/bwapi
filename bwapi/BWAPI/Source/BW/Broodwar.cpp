#include "Broodwar.h"
#include "Offsets.h"

namespace BW
{
//public:
  bool isInReplay()
  {
    return *(BW::BWDATA_InReplay) != 0;
  }
}
