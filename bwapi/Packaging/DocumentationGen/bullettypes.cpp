#include "helpers.h"

void writeBulletTypeInfo()
{
  std::ofstream of("bullettypes.dox");
  for (auto t : BulletTypes::allBulletTypes())
  {
    if (t == BulletTypes::Unknown || t == BulletTypes::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}
