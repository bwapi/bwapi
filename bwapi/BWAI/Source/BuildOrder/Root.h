#include <list>
#include <map>
#include <string>


#include "../../../BWAPI/Source/BW/UnitType.h"

namespace BuildOrder { class Branch; }

namespace BuildOrder
{
  class Root
  {
    public :
      Root(const std::string& xmlFileName);
      ~Root();
      std::list<Branch*> buildOrders;
      std::map<std::string, std::pair<BW::UnitType,int>* > weights;
  };
}