#include "EventTest.h"
using namespace std;
using namespace BWAPI;
EventTest::EventTest()
{
  strictEventChecking = false;
  onStartCalled = false;
  onEndCalled = false;
}
bool EventTest::isEventExpectedNext(Event e)
{
  if (expectedEvents.empty()) return false;
  Event e2 = (*expectedEvents.begin());
  return (e==e2);
}
void EventTest::onStart()
{
  BWAssert(onStartCalled==false);
  BWAssert(onEndCalled==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->setLocalSpeed(0);
  onStartCalled = true;
}
void EventTest::onEnd(bool isWinner)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  onEndCalled = true;
  BWAssert(isEventExpectedNext(Event::MatchEnd(isWinner)));
  if (expectedEvents.size()>0)
    expectedEvents.pop_front();
}
void EventTest::onFrame()
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  strictEventChecking = true;
}
void EventTest::onSendText(std::string text)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::SendText(text)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onReceiveText(Player* player, std::string text)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::ReceiveText(player,text)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onPlayerLeft(Player* player)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::PlayerLeft(player)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onNukeDetect(Position target)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::NukeDetect(target)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onUnitDiscover(Unit* unit)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::UnitDiscover(unit)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onUnitEvade(Unit* unit)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::UnitEvade(unit)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onUnitShow(Unit* unit)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::UnitShow(unit)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onUnitHide(Unit* unit)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::UnitHide(unit)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onUnitCreate(Unit* unit)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::UnitCreate(unit)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onUnitDestroy(Unit* unit)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::UnitDestroy(unit)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onUnitMorph(Unit* unit)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::UnitMorph(unit)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onUnitRenegade(Unit* unit)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::UnitRenegade(unit)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}
void EventTest::onSaveGame(std::string gameName)
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  if (strictEventChecking)
  {
    BWAssert(isEventExpectedNext(Event::SaveGame(gameName)));
    if (expectedEvents.size()>0)
      expectedEvents.pop_front();
  }
}