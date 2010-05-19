#include "TestAIModule.h"
using namespace BWAPI;
void TestAIModule::onStart()
{
  test_func=0;
  Broodwar->sendText("Hello world!");
  Broodwar->printf("The map is %s, a %d player map",Broodwar->mapName().c_str(),Broodwar->getStartLocations().size());
  // Enable some cheat flags
  Broodwar->enableFlag(Flag::UserInput);
  // Uncomment to enable complete map information
  //Broodwar->enableFlag(Flag::CompleteMapInformation);


  if (Broodwar->isReplay())
  {
    Broodwar->printf("The following players are in this replay:");
    for(std::set<Player*>::iterator p=Broodwar->getPlayers().begin();p!=Broodwar->getPlayers().end();p++)
    {
      if (!(*p)->getUnits().empty() && !(*p)->isNeutral())
      {
        Broodwar->printf("%s, playing as a %s",(*p)->getName().c_str(),(*p)->getRace().getName().c_str());
      }
    }
  }
  else
  {
    Broodwar->printf("The match up is %s v %s",
      Broodwar->self()->getRace().getName().c_str(),
      Broodwar->enemy()->getRace().getName().c_str());

    //send each worker to the mineral field that is closest to it
    for(std::set<Unit*>::const_iterator i=Broodwar->self()->getUnits().begin();i!=Broodwar->self()->getUnits().end();i++)
    {
      if ((*i)->getType().isWorker())
      {
        Unit* closestMineral=NULL;
        for(std::set<Unit*>::iterator m=Broodwar->getMinerals().begin();m!=Broodwar->getMinerals().end();m++)
        {
          if (closestMineral==NULL || (*i)->getDistance(*m)<(*i)->getDistance(closestMineral))
            closestMineral=*m;
        }
        if (closestMineral!=NULL)
          (*i)->rightClick(closestMineral);
      }
      else if ((*i)->getType().isResourceDepot())
      {
        //if this is a center, tell it to build the appropiate type of worker
        if ((*i)->getType().getRace()!=Races::Zerg)
        {
          (*i)->train(Broodwar->self()->getRace().getWorker());
        }
        else //if we are Zerg, we need to select a larva and morph it into a drone
        {
          std::set<Unit*> myLarva=(*i)->getLarva();
          if (myLarva.size()>0)
          {
            Unit* larva=*myLarva.begin();
            larva->morph(UnitTypes::Zerg_Drone);
          }
        }
      }
    }
  }
}
void TestAIModule::onEnd(bool isWinner)
{
  if (isWinner)
  {
    //log win to file
  }
}
void TestAIModule::onFrame()
{
  if (Broodwar->getKeyState(BWAPI::K_0)) test_func=0;
  if (Broodwar->getKeyState(BWAPI::K_1)) test_func=1;
  if (Broodwar->getKeyState(BWAPI::K_2)) test_func=2;
  if (Broodwar->getKeyState(BWAPI::K_3)) test_func=3;
  if (Broodwar->getKeyState(BWAPI::K_4)) test_func=4;
  if (Broodwar->getKeyState(BWAPI::K_5)) test_func=5;
  if (Broodwar->getKeyState(BWAPI::K_6)) test_func=6;
  if (Broodwar->getKeyState(BWAPI::K_7)) test_func=7;
  if (Broodwar->getKeyState(BWAPI::K_8)) test_func=8;
  if (Broodwar->getKeyState(BWAPI::K_9)) test_func=9;
  Broodwar->drawTextScreen(0,0,"isInGame=%d",Broodwar->isInGame());
  std::set<Unit*> units = Broodwar->getAllUnits();
  if (test_func==1)
  {
    int mx=Broodwar->getMouseX()+Broodwar->getScreenX();
    int my=Broodwar->getMouseY()+Broodwar->getScreenY();
    mx=mx/BWAPI::TILE_SIZE;
    my=my/BWAPI::TILE_SIZE;
    if (Broodwar->canBuildHere(NULL,TilePosition(mx,my),UnitTypes::Terran_Refinery))
      Broodwar->drawBoxMap(mx*32,my*32,mx*32+4*32,my*32+2*32,Colors::Green,false);
    else
      Broodwar->drawBoxMap(mx*32,my*32,mx*32+4*32,my*32+2*32,Colors::Red,false);
  }
  for each(Unit* u in units)
  {
    int x=u->getPosition().x();
    int y=u->getPosition().y();
    switch(test_func)
    {
    case 1:
      Broodwar->drawTextMap(x,y,"%d",u->getGroundWeaponCooldown());
      break;
    case 2:
      Broodwar->drawTextMap(x,y,"%d",u->getAirWeaponCooldown());
      break;
    case 3:
      Broodwar->drawTextMap(x,y,"%d",u->isConstructing());
      break;
    case 4:
      Broodwar->drawTextMap(x,y,"%d",u->isIdle());
      break;
    case 5:
      Broodwar->drawTextMap(x,y,"%d",u->isMorphing());
      break;
    case 6:
      Broodwar->drawTextMap(x,y,"%d",u->isStartingAttack());
      break;
    case 7:
      Broodwar->drawTextMap(x,y,"%d",u->isTraining());
      break;
    case 8:
      Broodwar->drawTextMap(x,y,"%d",u->isAttacking());
      break;
    case 9:
      Broodwar->drawTextMap(x,y,"%d",u->isVisible(Broodwar->enemy()));
      break;
    default:
      break;
    }
  }
  if (Broodwar->isReplay())
    return;

  drawStats();
}

void TestAIModule::onUnitCreate(BWAPI::Unit* unit)
{
  if (!Broodwar->isReplay())
    Broodwar->sendText("A %s [%x] has been created at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
  else
  {
    /*if we are in a replay, then we will print out the build order
    (just of the buildings, not the units).*/
    if (unit->getType().isBuilding() && unit->getPlayer()->isNeutral()==false)
    {
      int seconds=Broodwar->getFrameCount()/24;
      int minutes=seconds/60;
      seconds%=60;
      Broodwar->sendText("%.2d:%.2d: %s creates a %s",minutes,seconds,unit->getPlayer()->getName().c_str(),unit->getType().getName().c_str());
    }
  }
}
void TestAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
  if (!Broodwar->isReplay())
    Broodwar->sendText("A %s [%x] has been destroyed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void TestAIModule::onUnitMorph(BWAPI::Unit* unit)
{
  if (!Broodwar->isReplay())
    Broodwar->sendText("A %s [%x] has been morphed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
  else
  {
    /*if we are in a replay, then we will print out the build order
    (just of the buildings, not the units).*/
    if (unit->getType().isBuilding() && unit->getPlayer()->isNeutral()==false)
    {
      int seconds=Broodwar->getFrameCount()/24;
      int minutes=seconds/60;
      seconds%=60;
      Broodwar->sendText("%.2d:%.2d: %s morphs a %s",minutes,seconds,unit->getPlayer()->getName().c_str(),unit->getType().getName().c_str());
    }
  }
}
void TestAIModule::onUnitShow(BWAPI::Unit* unit)
{
  if (!Broodwar->isReplay())
    Broodwar->sendText("A %s [%x] has been spotted at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}
void TestAIModule::onUnitHide(BWAPI::Unit* unit)
{
  if (!Broodwar->isReplay())
    Broodwar->sendText("A %s [%x] was last seen at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}
void TestAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
  if (!Broodwar->isReplay())
    Broodwar->sendText("A %s [%x] is now owned by %s",unit->getType().getName().c_str(),unit,unit->getPlayer()->getName().c_str());
}

void TestAIModule::onSaveGame(std::string gameName)
{
  Broodwar->printf("onSaveGame %s",gameName.c_str());
}
void TestAIModule::onPlayerLeft(BWAPI::Player* player)
{
  Broodwar->sendText("%s left the game.",player->getName().c_str());
}
void TestAIModule::onNukeDetect(BWAPI::Position target)
{
  if (target!=Positions::Unknown)
    Broodwar->printf("Nuclear Launch Detected at (%d,%d)",target.x(),target.y());
  else
    Broodwar->printf("Nuclear Launch Detected");
}

bool TestAIModule::onSendText(std::string text)
{
  if (text=="/show players")
  {
    showPlayers();
    return false;
  } else if (text=="/show forces")
  {
    showForces();
    return false;
  } else if (text=="/merge")
  {
    std::set<Unit*> units=Broodwar->getSelectedUnits();
    Unit* a=NULL;
    Unit* b=NULL;
    for each(Unit* u in units)
    {
      if (u->getType()==UnitTypes::Protoss_High_Templar || u->getType()==UnitTypes::Protoss_Dark_Templar)
      {
        if (a==NULL)
          a=u;
        else
        {
          if (b==NULL && u->getType()==a->getType())
            b=u;
        }
      }
    }
    if (a!=NULL && b!=NULL)
    {
      if (a->getType()==UnitTypes::Protoss_High_Templar)
      {
        Broodwar->printf("calling a->useTech(TechTypes::Archon_Warp,b)");
        a->useTech(TechTypes::Archon_Warp,b);
      }
      else
      {
        Broodwar->printf("calling a->useTech(TechTypes::Dark_Archon_Meld,b)");
        a->useTech(TechTypes::Dark_Archon_Meld,b);
      }
    }
  } else
  {
    Broodwar->printf("You typed '%s'!",text.c_str());
  }
  return true;
}

void TestAIModule::drawStats()
{
  std::set<Unit*> myUnits = Broodwar->self()->getUnits();
  Broodwar->drawTextScreen(5,16,"I have %d units:",myUnits.size());
  std::map<UnitType, int> unitTypeCounts;
  for(std::set<Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++)
  {
    if (unitTypeCounts.find((*i)->getType())==unitTypeCounts.end())
    {
      unitTypeCounts.insert(std::make_pair((*i)->getType(),0));
    }
    unitTypeCounts.find((*i)->getType())->second++;
  }
  int line=2;
  for(std::map<UnitType,int>::iterator i=unitTypeCounts.begin();i!=unitTypeCounts.end();i++)
  {
    Broodwar->drawTextScreen(5,16*line,"- %d %ss",(*i).second, (*i).first.getName().c_str());
    line++;
  }
}

void TestAIModule::showPlayers()
{
  std::set<Player*> players=Broodwar->getPlayers();
  for(std::set<Player*>::iterator i=players.begin();i!=players.end();i++)
  {
    Broodwar->printf("Player [%d]: %s is in force: %s",(*i)->getID(),(*i)->getName().c_str(), (*i)->getForce()->getName().c_str());
  }
}
void TestAIModule::showForces()
{
  std::set<Force*> forces=Broodwar->getForces();
  for(std::set<Force*>::iterator i=forces.begin();i!=forces.end();i++)
  {
    std::set<Player*> players=(*i)->getPlayers();
    Broodwar->printf("Force %s has the following players:",(*i)->getName().c_str());
    for(std::set<Player*>::iterator j=players.begin();j!=players.end();j++)
    {
      Broodwar->printf("  - Player [%d]: %s",(*j)->getID(),(*j)->getName().c_str());
    }
  }
}