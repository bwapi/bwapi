#include "BWAgent.h"
#include "BridgeClient.h"
#include "AIModule.h"

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
    if(!BridgeClient::connect())
    {
      lastError = "could not connect: " + BridgeClient::getLastError();
      return 0;
    }
    return 1;
  }

  // AGENT_API
  bool takeover()
  {
    resetError();
    if(BridgeClient::waitForEvent())
    {
      lastError = "error while waiting for event. " + BridgeClient::getLastError();
      return false;
    }
    return true;
  }

  // AGENT_API
  std::string getLastError()
  {
    return lastError;
  }
}
