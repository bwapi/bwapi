#include "Command.h"

#include <string>
class TiXmlElement;

namespace BuildOrder
{
  /** Represnts task to call function. */
  class CommandCall : public Command
  {
    public :
      CommandCall(TiXmlElement* xmlElement);
      CommandCall(const std::string& name);
      virtual bool executeInternal(Executor* executor);
    private :
      std::string name;
  };
}