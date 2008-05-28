class TiXmlElement;

#include <list>
namespace BuildOrder { class Command; }

namespace BuildOrder
{
 /** 
  * Represents one branch of the bulid order.
  * It will be referenced by the jump, call command in the build order later on 
  */
  class Branch
  {
    public :
      Branch(TiXmlElement* xmlElement);
      std::list<Command*> commands;
     
  };
}