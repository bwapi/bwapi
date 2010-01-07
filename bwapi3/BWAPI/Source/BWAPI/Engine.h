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
    // hooks.
    extern void _onMatchFrame() throw();
    extern void _onMenuFrame() throw();
    extern void _onMatchEnd() throw();
    extern void _onUnitKilled(BW::Unit* unit) throw();
    extern void _onDllLoad() throw();
    extern void _onMessageIntercepted(const char* text) throw();
    extern void _onMatchDrawHigh() throw();
  };
};
