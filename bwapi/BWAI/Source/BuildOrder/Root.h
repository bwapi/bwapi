#include <list>
#include <map>
#include <string>

#include <BW/UnitType.h>

namespace BuildOrder { class Branch; }
namespace BuildOrder { class BuildWeights; }
namespace Util { class Logger; }

namespace BuildOrder
{
  /** 
   * Holder of all build order branches and build-order specific settings.
   */
  class Root
  {
    public :
      Root(const std::string& xmlFileName);
      ~Root();
      std::list<Branch*> buildOrders;
      std::map<std::string, BuildWeights*> weights;
      Util::Logger *log;
  };
}