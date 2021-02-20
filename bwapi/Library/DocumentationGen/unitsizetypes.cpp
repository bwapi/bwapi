#include "helpers.h"

void writeUnitSizeInfo()
{
  std::ofstream of("unitsizetypes.dox");
  for (auto t : UnitSizeTypes::allUnitSizeTypes())
  {
    if (t == UnitSizeTypes::Unknown || t == UnitSizeTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}
