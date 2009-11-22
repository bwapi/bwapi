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
      virtual void onStart();
      virtual void onEnd();
      virtual void onFrame();
      virtual bool onSendText(std::string text);
      virtual void onUnitCreate(Unit* unit);
      virtual void onUnitDestroy(Unit* unit);
      virtual void onUnitMorph(Unit* unit);
      virtual void onUnitShow(Unit* unit);
      virtual void onUnitHide(Unit* unit);
      virtual void onUnitRenegade(Unit* unit);
  };
}
