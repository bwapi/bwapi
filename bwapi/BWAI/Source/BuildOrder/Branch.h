class TiXmlElement;

#include <list>

#include <BW/Race.h>

namespace BuildOrder { class Command; }

namespace BuildOrder
{
 /**
  * Represents one branch of the bulid order. It will be referenced by the jump, call command in the build
  * order later on 
  */
  class Branch
  {
    public :
      /** Constructs the branch according to the xml-definition*/
      Branch(TiXmlElement* xmlElement);
      /** Commands in the branch, the order specifies the order of execution. */
      std::list<Command*> commands;
      BW::Race::Enum against;
      std::string getName();
      void loadTypes();
    private :
      /** Default constructor is forbidden, the customisation of bulidorder in runtime is not supported now */
      Branch();
      std::string name;
  };
}