class TiXmlElement;

#include <list>
namespace BuildOrder { class Command; }

namespace BuildOrder
{
  class Branch
  {
    public :
      Branch(TiXmlElement* xmlElement);
      std::list<Command*> commands;
     
  };
}