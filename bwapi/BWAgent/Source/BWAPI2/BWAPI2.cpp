#include "BWAPI2.h"
#include "AIModule.h"

#include <BWAPI\BWAPI.h>

#include <Util\Version.h>


// singleton base class
namespace BWAPI2
{
//private:

  std::string lastError;
  void resetError()
  {
    lastError = "no error";
  }

//public:
  //----------------------------------- GET VERSION -----------------------------------------------
  // BWAPI2_FUNCTION
  int BWAPI2_CALL getVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  // BWAPI2_FUNCTION
  int BWAPI2_CALL connect()
  {
    resetError();
    if(!BWConnect())
    {
      lastError = "could not connect: " + std::string(BWGetLastError());
      return 0;
    }
    return 1;
  }
  //----------------------------------- TAKE OVER -------------------------------------------------
  // BWAPI2_FUNCTION
  bool BWAPI2_CALL takeover(AIModule& aiModule)
  {
    /* TODO: rewrite to use BWAPI1
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
    */
    return true;
  }
  //----------------------------------- GET LAST ERROR --------------------------------------------
  // BWAPI2_FUNCTION
  const char* BWAPI2_CALL getLastError()
  {
    return lastError.c_str();
  }
  //----------------------------------- -----------------------------------------------------------
}
