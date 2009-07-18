#include "Command.h"

#include <string>
class TiXmlElement;

namespace BuildOrder
{
  /** Represnts task to register exception globally. */
  class CommandRegisterException : public Command
  {
    public :
      CommandRegisterException(TiXmlElement* xmlElement);
      virtual bool executeInternal(Executor* executor);
    private :
      std::string name;
  };
}