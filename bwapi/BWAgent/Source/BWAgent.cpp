#include "BWAgent.h"
#include "BridgeAgent.h"
#include "AIModule.h"
#include "BridgeAgent.h"

#include "Util\Version.h"

// singleton base class
namespace BWAgent
{
//private:

  std::string lastError;
  void resetError()
  {
    lastError = "no error";
  }

//public:
  // AGENT_API
  int getVersion()
  {
    return SVN_REV;
  }

  // AGENT_API
  int connect(AIModule* aiModule)
  {
    resetError();
    if(!BridgeAgent::connect())
    {
      lastError = "could not connect: " + BridgeAgent::getLastError();
      return 0;
    }
    return 1;
  }

  // AGENT_API
  std::string getLastError()
  {
    return lastError;
  }
}
