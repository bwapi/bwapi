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
  //----------------------------------- GET VERSION -----------------------------------------------
  // AGENT_API
  int getVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  // AGENT_API
  int connect()
  {
    resetError();
    if(!BridgeClient::connect())
    {
      lastError = "could not connect: " + BridgeClient::getLastError();
      return 0;
    }
    return 1;
  }
  //----------------------------------- TAKE OVER -------------------------------------------------
  // AGENT_API
  bool takeover(AIModule& aiModule)
  {
    resetError();
    while(true)
    {
      if(!BridgeClient::waitForEvent())
      {
        lastError = __FUNCTION__ ": " + BridgeClient::getLastError();
        return false;
      }

      // react upon bridge state
      BridgeClient::RpcState rpcState = BridgeClient::getCurrentRpc();
      switch(rpcState)
      {
      case BridgeClient::OnInitMatch:
        {
          aiModule.onStartMatch();
        }break;
      case BridgeClient::OnFrame:
        {
          for each(const std::string &input in BridgeClient::getUserInputStrings())
          {
            aiModule.onSendText(input);
          }
          aiModule.onFrame();
        }break;
      }
    }
    return true;
  }
  //----------------------------------- GET LAST ERROR --------------------------------------------
  // AGENT_API
  std::string getLastError()
  {
    return lastError;
  }
  //----------------------------------- -----------------------------------------------------------
}
