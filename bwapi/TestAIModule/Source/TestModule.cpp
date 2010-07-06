#include "TestModule.h"
using namespace std;
using namespace BWAPI;
TestModule::TestModule() : currentTestCase(NULL)
{
}

TestModule::~TestModule()
{
  for(std::list<TestCase*>::iterator i=testCases.begin();i!=testCases.end();i++)
  {
    delete *i;
  }
  testCases.clear();
}

void TestModule::onFrame()
{
  if (Broodwar->isPaused()) return;
  if (Broodwar->getFrameCount()==0) return;
  Broodwar->drawTextScreen(0,0,"FPS: %d",Broodwar->getFPS());
  Broodwar->drawTextScreen(0,20,"Assert success count: %d",assert_success_count);
  Broodwar->drawTextScreen(0,40,"Assert failed count: %d",assert_fail_count);
  for each(Unit* u in Broodwar->self()->getUnits())
  {
    Broodwar->drawTextMap(u->getPosition().x(),u->getPosition().y()-16,"%s",u->getType().getName().c_str());
    Broodwar->drawTextMap(u->getPosition().x(),u->getPosition().y(),"%s",u->getOrder().getName().c_str());
    Broodwar->drawTextMap(u->getPosition().x(),u->getPosition().y()+16,"ID: %d",u->getID());
  }
  runTestCases();
}

void TestModule::runTestCases()
{
  if (currentTestCase==NULL && testCases.empty()==false)
  {
    currentTestCase = *testCases.begin();
    currentTestCase->start();
  }
  if (currentTestCase!=NULL)
  {
    if (currentTestCase->isRunning())
      currentTestCase->update();
    else
    {
      currentTestCase->stop();
      delete currentTestCase;
      currentTestCase = NULL;
      testCases.erase(testCases.begin());
    }
  }
}

void TestModule::addTestCase(TestCase* testCase)
{
  testCases.push_back(testCase);
}

TestCase* TestModule::getCurrentTestCase() const
{
  return currentTestCase;
}