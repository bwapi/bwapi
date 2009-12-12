#include "BWAPI.h"
#include "BridgeClient.h"

#include <Util\Version.h>
#include <Util\Types.h>

#include <Bridge\SharedStuff.h>

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
  //----------------------------------- CLOSE HANDLE ----------------------------------------------
  BWAPI_FUNCTION void BWCloseHandle(HANDLE h)
  {
    if(!h)
      return;
    delete h;
  }
  //----------------------------------- GET VERSION -----------------------------------------------
  BWAPI_FUNCTION int BWAPI_CALL BWGetVersion()
  {
    return SVN_REV;
  }
  //----------------------------------- CONNECT ---------------------------------------------------
  BWAPI_FUNCTION bool BWAPI_CALL BWConnect()
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
  BWAPI_FUNCTION bool BWAPI_CALL BWTakeover(BWMatchFrameCallback onMatchFrame,
    BWMatchStartCallback onMatchStart,
    BWUnitAddCallback onUnitAdd,
    BWUnitRemoveCallback onUnitRemove,
    BWSendTextCallback onSendText)
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
          if(onMatchStart)onMatchStart(BridgeClient::isMatchStartFromBeginning);
        }break;
      case BridgeClient::OnFrame:
        {
          for each(const std::string &input in BridgeClient::getUserInputStrings())
          {
            if(onSendText)onSendText(input.c_str());
          }
          onMatchFrame();
        }break;
      }
    }
    return true;
  }
  //----------------------------------- -----------------------------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWDrawText(int x, int y, const char* text)
  {
    BridgeClient::pushDrawText(x, y, text);
  }
  //----------------------------------- -----------------------------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWDrawRect(int x, int y, int w, int h, int color)
  {
    BridgeClient::pushDrawRectangle(x, y, w, h, color);
  }
  //----------------------------------- ALL UNITS ITERATOR ----------------------------------------
  struct AllUnitsHandle
  {
    Bridge::SharedStuff::KnownUnitSet::Index index;
  };
  //----------------------------------- ALL UNITS BEGIN -------------------------------------------
  BWAPI_FUNCTION HANDLE BWAPI_CALL BWAllUnitsBegin()
  {
    AllUnitsHandle *handle = new AllUnitsHandle;
    handle->index = BridgeClient::sharedStuff.knownUnits.begin();
    return handle;
  }
  //----------------------------------- ALL UNITS NEXT --------------------------------------------
  BWAPI_FUNCTION BWAPI::UnitState* BWAPI_CALL BWAllUnitsNext(HANDLE h)
  {
    AllUnitsHandle &handle = *(AllUnitsHandle*)h;
    if(!handle.index.isValid())
      return NULL;
    BWAPI::UnitState &retval = BridgeClient::sharedStuff.knownUnits.get(handle.index);
    handle.index = BridgeClient::sharedStuff.knownUnits.getNext(handle.index);
    return &retval;
  }
  //----------------------------------- ALL UNITS CLOSE -------------------------------------------
  BWAPI_FUNCTION void BWAPI_CALL BWAllUnitsClose(HANDLE h)
  {
    // for now just assume AI didn't screw up
    delete (AllUnitsHandle*)h;
  }
  //----------------------------------- GET LAST ERROR --------------------------------------------
  BWAPI_FUNCTION const char* BWAPI_CALL BWGetLastError()
  {
    return lastError.c_str();
  }
  //----------------------------------- -----------------------------------------------------------
}
