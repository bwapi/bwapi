#include "BWAPI.h"

#include "BridgeClient.h"

#include <Util\Version.h>

// singleton base class
namespace BWAPI
{
//private:

  std::string lastError;
  void resetError()
  {
    lastError = "no error";
  }

//public:
  //----------------------------------- GET VERSION -----------------------------------------------
  // BWAPI_FUNCTION
  int BWAPI_CALL GetVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  // BWAPI_FUNCTION
  bool BWAPI_CALL Connect()
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
  // BWAPI_FUNCTION
  bool BWAPI_CALL Takeover(BWAPI_MatchFrameCallback a,
    BWAPI_MatchStartCallback b,
    BWAPI_UnitAddCallback c,
    BWAPI_UnitRemoveCallback d)
  {
    resetError();
/*  STUB
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
    }*/
    return true;
  }
  //----------------------------------- GET LAST ERROR --------------------------------------------
  // BWAPI_FUNCTION
  const char* BWAPI_CALL GetErrorString()
  {
    return lastError.c_str();
  }
  //----------------------------------- -----------------------------------------------------------
}
