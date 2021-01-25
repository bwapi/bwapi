#include "helpers.h"

void writeDamageTypeInfo()
{
  std::ofstream of("damagetypes.dox");
  for (auto t : DamageTypes::allDamageTypes())
  {
    if (t == DamageTypes::Unknown || t == DamageTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}
