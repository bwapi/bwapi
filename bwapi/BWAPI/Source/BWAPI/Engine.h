#pragma once
/**********************************************************************
 *  The main class controlling the bridge server and data flow
 *
 ********/

namespace BW { struct Unit; }

namespace BWAPI
{
  namespace Engine
  {
    // hooks
    extern void onMatchFrame();
    extern void onMenuFrame();
    extern void onMatchEnd();
    extern bool onSendText(const char* text);
    extern void onUnitDeath(BW::Unit* unit);
    extern void onDllLoad();
    extern void onMessageIntercepted(const char* text);
  };
};
