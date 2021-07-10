#include "helpers.h"

void writeErrorInfo()
{
  std::ofstream of("errors.dox");
  for (auto t : Errors::allErrors())
  {
    if (t == Errors::Unknown || t == Errors::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}
