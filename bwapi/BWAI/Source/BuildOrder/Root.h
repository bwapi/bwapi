#include <list>
#include <string>

namespace BuildOrder { class Branch; }

namespace BuildOrder
{
  class Root
  {
    public :
      Root(const std::string& xmlFileName);
      ~Root();
      std::list<Branch*> buildOrders;
  };
}