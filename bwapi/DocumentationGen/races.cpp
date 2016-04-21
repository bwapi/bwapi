#include "helpers.h"

void writeRaceInfo()
{
  std::ofstream of("races.dox");
  for (auto t : Races::allRaces())
  {
    if (t == Races::Unknown || t == Races::None) continue;
    of << docEnum(t);
    of << docBegin(t);

    of << docIntro(t) << "\n";

    of << "<table>";
    
    of << row("Worker", iconref(t.getWorker()));
    of << row("Resource Depot", iconref(t.getResourceDepot()));
    of << row("Refinery", iconref(t.getRefinery()));
    of << row("Transport", iconref(t.getTransport()));
    of << row("Supply Provider", iconref(t.getSupplyProvider()));

    of << "</table>\n";

    // References
    std::string const & name = t.getName();
    of << "@tl" << name << " @scc" << name << " @wik" << name;
    of << docEnd();
  }
}
