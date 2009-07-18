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
      virtual void onRemoveUnit(Unit* unit);
  };
}
