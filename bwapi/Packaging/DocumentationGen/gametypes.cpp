#include "helpers.h"

void writeGameTypeInfo()
{
  std::ofstream of("gametypes.dox");
  for (auto t : GameTypes::allGameTypes())
  {
    if (t == GameTypes::Unknown || t == GameTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}
