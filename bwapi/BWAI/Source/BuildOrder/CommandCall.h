#include "Command.h"

#include <string>
class TiXmlElement;

namespace BuildOrder
{
 /** Represnts task to make building in the build order. */
  class CommandCall : public Command
  {
    public :
      CommandCall(TiXmlElement* xmlElement);
      virtual bool executeInternal(Executor* executor);
    private :
      std::string name;
  };
}