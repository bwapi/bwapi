#pragma once
#include <string>
namespace BWAPI
{
  class Unit;
  class AIModule
  {
    public:
      AIModule();
      virtual ~AIModule();
      virtual void __cdecl onStart();
      virtual void __cdecl onEnd();
      virtual void __cdecl onFrame();
      virtual void __cdecl onAddUnit(Unit* unit);
      virtual void __cdecl onRemoveUnit(Unit* unit);
      virtual bool __cdecl onSendText(std::string text);
  };
}
