#include "helpers.h"

void writeOrderInfo()
{
  std::ofstream of("orders.dox");
  for (auto t : Orders::allOrders())
  {
    if (t == Orders::Unknown || t == Orders::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << icon(t) << " " << docIntro(t) << "\n";

    of << makekeywords(t);

    of << docEnd();
  }
}
