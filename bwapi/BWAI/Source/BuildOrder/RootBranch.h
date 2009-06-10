#include "Branch.h"

#include <BWAPI.h>

namespace BuildOrder
{
 /**
  * Represents one branch of the bulid order. It will be referenced by the jump, call command in the build
  * order later on 
  */
  class RootBranch : public Branch
  {
    public :
      /** Constructs the branch according to the xml-definition*/
      RootBranch(TiXmlElement* xmlElement);
      BWAPI::Race race, against;
    private :
      /** Default constructor is forbidden, the customisation of bulidorder in runtime is not supported now */
      RootBranch();
  };
}