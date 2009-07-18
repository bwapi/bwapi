#pragma once

#include <list>
namespace BuildOrder { class Command; }
namespace BuildOrder { class Condition; }
class TiXmlElement;

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
      std::string getName();
      Condition* condition;
    private :
      /** Default constructor is forbidden, the customisation of buildorder in runtime is not supported now */
      Branch();
      std::string name;
  };
}