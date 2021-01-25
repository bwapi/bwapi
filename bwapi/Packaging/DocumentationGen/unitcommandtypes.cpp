#include "helpers.h"

void writeUnitCommandInfo()
{
  std::ofstream of("unitcommandtypes.dox");
  for (auto t : UnitCommandTypes::allUnitCommandTypes())
  {
    if (t == UnitCommandTypes::Unknown || t == UnitCommandTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}
