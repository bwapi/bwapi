#include "helpers.h"

void writeExplosionTypeInfo()
{
  std::ofstream of("explosiontypes.dox");
  for (auto t : ExplosionTypes::allExplosionTypes())
  {
    if (t == ExplosionTypes::Unknown || t == ExplosionTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}
