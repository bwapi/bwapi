#include "DefaultTestModule.h"
using namespace std;
using namespace BWAPI;
std::map<Unit*,Order> lastOrder;
std::map<Unit*,list<Order> > lastDistinctOrders;
void DefaultTestModule::onStart()
{
  BWAssert(Broodwar->isMultiplayer()==false);
  BWAssert(Broodwar->isReplay()==false);
  Broodwar->enableFlag(Flag::CompleteMapInformation);
  Broodwar->enableFlag(Flag::UserInput);
  Broodwar->sendText("show me the money");
  Broodwar->sendText("operation cwal");
}
void DefaultTestModule::onFrame()
{
  if (Broodwar->getFrameCount()==0) return;
  Broodwar->drawTextScreen(0,0,"FPS: %d",Broodwar->getFPS());
  for each(Unit* u in Broodwar->getAllUnits())
  {
    Broodwar->drawTextMap(u->getPosition().x(),u->getPosition().y()-16,"%s",u->getOrder().getName().c_str());
    Broodwar->drawTextMap(u->getPosition().x(),u->getPosition().y(),"%s",u->getType().getName().c_str());
  }
  for each(Unit* u in Broodwar->getAllUnits())
  {
    if (lastOrder.find(u)==lastOrder.end())
    {
      lastOrder[u]=Orders::None;
    }
  }
  for each(Unit* u in Broodwar->getAllUnits())
  {
    if (lastOrder[u]!=u->getOrder())
    {
      lastDistinctOrders[u].push_front(lastOrder[u]);
      if (lastDistinctOrders[u].size()>5)
        lastDistinctOrders[u].pop_back();
      lastOrder[u]=u->getOrder();
    }
  }
  for each(Unit* u in Broodwar->getAllUnits())
  {
    int y=-32;
    for each(Order o in lastDistinctOrders[u])
    {
      Broodwar->drawTextMap(u->getPosition().x(),u->getPosition().y()+y,"%s",o.getName().c_str());
      y-=16;
    }
  }
}