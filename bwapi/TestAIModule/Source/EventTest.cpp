#include "EventTest.h"
#include <iostream>
#include <fstream>
using namespace std;
using namespace BWAPI;
EventTest::EventTest()
{
  strictEventChecking = false;
  onStartCalled = false;
  onEndCalled = false;
  state = Start;
}
bool EventTest::isEventExpectedNext(Event e)
{
  if (expectedEvents.empty()) return false;
  Event e2 = (*expectedEvents.begin());
  return (e.type==e2.type);
}
void EventTest::onStart()
{
  BWAssert(onStartCalled==false);
  BWAssert(onEndCalled==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->setLocalSpeed(0);
  Broodwar->sendText("show me the money");
  onStartCalled = true;
}
void EventTest::onEnd(bool isWinner)
{
  ofstream er;
  er.open("test_module.log");
  if (onStartCalled==false)
  {
    er << "onEnd: Error: onStartCalled == false\n";
    er.close();
    return;
  }
  if (onEndCalled==true)
  {
    er << "onEnd: Error: onEndCalled == true\n";
    er.close();
    return;
  }
  onEndCalled = true;
  if (expectedEvents.empty())
  {
    er << "onEnd: Error: expectedEvents.empty() == true\n";
    er.close();
    return;
  }
  if ((*expectedEvents.begin()).type!=EventType::MatchEnd)
  {
    er << "onEnd: Error: expected event was " << (*expectedEvents.begin()).type << ", not MatchEnd\n";
    er.close();
    return;
  }
  expectedEvents.pop_front();
  er << "onEnd: Success!\n";
  er.close();
  return;
}
void EventTest::onFrame()
{
  BWAssert(onStartCalled==true);
  BWAssert(onEndCalled==false);
  strictEventChecking = true;
  if (Broodwar->getFrameCount()<5) return;
  Broodwar->drawTextScreen(0,0,"FPS: %d",Broodwar->getFPS());
  Broodwar->drawTextScreen(0,20,"Assert success count: %d",assert_success_count);
  if (assert_fail_count==0)
    Broodwar->drawTextScreen(0,40,"Assert failed count: %d",assert_fail_count);
  else
    Broodwar->drawTextScreen(0,40,"\x08 Assert failed count: %d",assert_fail_count);
  if (state == Start)
  {
    state = TrainingSCV;

    Unit* cc = NULL;
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Terran_Command_Center)
        cc = u;
    BWAssert(cc!=NULL);

    cc->train(UnitTypes::Terran_SCV);
    expectedEvents.push_back(Event::UnitCreate(NULL));
    expectedEvents.push_back(Event::UnitDiscover(NULL));
    expectedEvents.push_back(Event::UnitShow(NULL));
  }
  else if (state == TrainingSCV)
  {
    if (expectedEvents.empty())
    {
      state = BuildingRefinery;

      Unit* scv = NULL;
      for each(Unit* u in Broodwar->self()->getUnits())
        if (u->getType()==UnitTypes::Terran_SCV && u->isCompleted())
          scv = u;
      BWAssert(scv!=NULL);

      TilePosition tilePosition=TilePositions::None;
      for each(Unit* u in Broodwar->getGeysers())
        tilePosition=u->getTilePosition();
      scv->build(tilePosition,UnitTypes::Terran_Refinery);
      expectedEvents.push_back(Event::UnitMorph(NULL));
      expectedEvents.push_back(Event::UnitRenegade(NULL));
    }
  }
  else if (state == BuildingRefinery)
  {
    if (expectedEvents.empty())
    {
      state = KillingSCV;

      Unit* scv = NULL;
      for each(Unit* u in Broodwar->self()->getUnits())
        if (u->getType()==UnitTypes::Terran_SCV && u->isCompleted())
          scv=u;
      BWAssert(scv!=NULL);

      for each(Unit* u in Broodwar->self()->getUnits())
        if (u->getType()==UnitTypes::Terran_Marine)
          u->attack(scv);
      expectedEvents.push_back(Event::UnitHide(NULL));
      expectedEvents.push_back(Event::UnitEvade(NULL));
      expectedEvents.push_back(Event::UnitDestroy(NULL));
    }
  }
  else if (state == KillingSCV)
  {
    if (expectedEvents.empty())
    {
      state = TrainingNuke;

      Unit* nukeSilo = NULL;
      for each(Unit* u in Broodwar->self()->getUnits())
        if (u->getType()==UnitTypes::Terran_Nuclear_Silo)
          nukeSilo=u;
      BWAssert(nukeSilo!=NULL);

      nukeSilo->train(UnitTypes::Terran_Nuclear_Missile);
      expectedEvents.push_back(Event::UnitCreate(NULL));
      expectedEvents.push_back(Event::UnitDiscover(NULL));
      expectedEvents.push_back(Event::UnitShow(NULL));
    }
  }
  else if (state == TrainingNuke)
  {

    Unit* nukeSilo = NULL;
    for each(Unit* u in Broodwar->self()->getUnits())
      if (u->getType()==UnitTypes::Terran_Nuclear_Silo)
        nukeSilo=u;
    BWAssert(nukeSilo!=NULL);

    if (nukeSilo->isIdle() && nukeSilo->hasNuke())
    {
      state = UsingNuke;
      Unit* ghost = NULL;
      for each(Unit* u in Broodwar->self()->getUnits())
        if (u->getType()==UnitTypes::Terran_Ghost)
          ghost=u;
      BWAssert(ghost!=NULL);
      Position p(ghost->getPosition());
      ghost->useTech(TechTypes::Nuclear_Strike,p);
      expectedEvents.push_back(Event::NukeDetect(p));
      expectedEvents.push_back(Event::UnitHide(NULL));
      expectedEvents.push_back(Event::UnitEvade(NULL));
      expectedEvents.push_back(Event::UnitDestroy(NULL));
      expectedEvents.push_back(Event::UnitHide(NULL));
      expectedEvents.push_back(Event::UnitEvade(NULL));
      expectedEvents.push_back(Event::UnitDestroy(NULL));
    }
  }
  else if (state == UsingNuke)
  {
    if (expectedEvents.empty())
    {
      state = DestroyingEnemy;
      for each(Unit* u in Broodwar->self()->getUnits())
        if (u->getType()==UnitTypes::Terran_Marine)
          u->attack(Position(Broodwar->mapWidth()*16,0));
      expectedEvents.push_back(Event::UnitDiscover(NULL));
      expectedEvents.push_back(Event::UnitShow(NULL));
      expectedEvents.push_back(Event::UnitHide(NULL));
      expectedEvents.push_back(Event::UnitEvade(NULL));
      expectedEvents.push_back(Event::UnitDestroy(NULL));
      expectedEvents.push_back(Event::MatchEnd(NULL));
    }
  }
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