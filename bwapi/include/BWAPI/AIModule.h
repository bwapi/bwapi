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
      virtual void onAddUnit(Unit* unit);
      virtual void onRemoveUnit(Unit* unit);
      virtual bool onSendText(std::string text);
  };
}
