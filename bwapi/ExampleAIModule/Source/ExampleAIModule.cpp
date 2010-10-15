#include "ExampleAIModule.h"
using namespace BWAPI;
/*
bool analyzed;
bool analysis_just_finished;
BWTA::Region* home;
BWTA::Region* enemy_base;
*/
void ExampleAIModule::onStart()
{
  Broodwar->sendText("Hello world!");
  Broodwar->printf("The map is %s, a %d player map",Broodwar->mapName().c_str(),Broodwar->getStartLocations().size());
  // Enable some cheat flags
  Broodwar->enableFlag(Flag::UserInput);
  // Uncomment to enable complete map information
  //Broodwar->enableFlag(Flag::CompleteMapInformation);
  /*
  //read map information into BWTA so terrain analysis can be done in another thread
  BWTA::readMap();
  analyzed=false;
  analysis_just_finished=false;
  */
  show_units           = false;
  show_bullets         = false;
  show_visibility_data = false;

  if (Broodwar->isReplay())
  {
    Broodwar->printf("The following players are in this replay:");
    for(std::set<Player*>::iterator p=Broodwar->getPlayers().begin();p!=Broodwar->getPlayers().end();p++)
    {
      if (!(*p)->getUnits().empty() && !(*p)->isNeutral())
        Broodwar->printf("%s, playing as a %s",(*p)->getName().c_str(),(*p)->getRace().getName().c_str());
    }
  }
  else
  {
    Broodwar->printf("The match up is %s v %s",
    Broodwar->self()->getRace().getName().c_str(),
    Broodwar->enemy()->getRace().getName().c_str());
  }

  scoutFrameCount = 0;
  scout = NULL;
}

void ExampleAIModule::onEnd(bool isWinner)
{
  if (isWinner)
  {
    //log win to file
  }
}

void ExampleAIModule::onFrame()
{
  Broodwar->drawTextScreen(200,0,"FPS: %d",Broodwar->getFPS());
  Broodwar->drawTextScreen(200,20,"Average FPS: %f",Broodwar->getAverageFPS());
  if (show_visibility_data)
    drawVisibilityData();

  if (show_units)
    drawUnits();

  if (show_bullets)
    drawBullets();

  if (Broodwar->isReplay())
    return;

  drawStats();
 
  for each ( Unit *u in Broodwar->self()->getUnits() )
  {
    if ( (!scout || !scout->exists()) && u->getType().isWorker() && !u->isCarryingGas() && !u->isCarryingMinerals() )
    {
      scout = u;
      scoutFrameCount = 0;
    } // if worker

    if ( u->getType().isWorker() && u != scout && u->isIdle() && Broodwar->getFrameCount() - u->getLastOrderFrame() > 10 )
    {
      Unit *closestMineral = NULL;
      for each ( Unit *m in Broodwar->getMinerals() )
      {
        if ( !closestMineral || u->getDistance(m) < u->getDistance(closestMineral) )
          closestMineral = m;
      }

      if ( closestMineral )
        u->rightClick(closestMineral);
    }

    if ( u->getType().isResourceDepot() )
    {
      //if this is a center, tell it to build the appropiate type of worker
      if ( u->getType().getRace() != Races::Zerg && u->isIdle() )
      {
        if ( Broodwar->canMake( u, Broodwar->self()->getRace().getWorker()) )
          u->train(Broodwar->self()->getRace().getWorker());
      }
      else //if we are Zerg, we need to select a larva and morph it into a drone
      {
        std::set<Unit*> myLarva = u->getLarva();
        if ( myLarva.size() > 0 && Broodwar->canMake( NULL, UnitTypes::Zerg_Drone) )
        {
          Unit* larva = *myLarva.begin();
          larva->morph(UnitTypes::Zerg_Drone);
        } // if larva
      } // if zerg
    } // if resource depot

  } // for each unit

  int thisFrameCount = Broodwar->getFrameCount();
  if ( thisFrameCount - scoutFrameCount > 40 && scout && scout->exists() )
  {
    scoutFrameCount = thisFrameCount;
    bool wantBreak = false;
    for ( int y = 0; y < Broodwar->mapHeight() - 1 && !wantBreak; ++y )
    {
      for ( int x = 0; x < Broodwar->mapWidth() && !wantBreak; ++x )
      {
        if ( !Broodwar->isExplored(x, y) && scout->hasPath(BWAPI::TilePosition(x, y)) )
        {
          wantBreak = true;
          scout->move(BWAPI::TilePosition(x, y));
        } // if
      } // for y
    } // for x
  } // frame count

}

void ExampleAIModule::onSendText(std::string text)
{
  if (text=="/show units")
  {
    show_units = !show_units;
  } else if (text=="/show bullets")
  {
    show_bullets = !show_bullets;
  } else if (text=="/show players")
  {
    showPlayers();
  } else if (text=="/show forces")
  {
    showForces();
  } else if (text=="/show visibility")
  {
    show_visibility_data=!show_visibility_data;
  }
  else
  {
    Broodwar->printf("You typed '%s'!",text.c_str());
  }
}

void ExampleAIModule::onReceiveText(BWAPI::Player* player, std::string text)
{
  Broodwar->printf("%s said '%s'", player->getName().c_str(), text.c_str());
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player* player)
{
  Broodwar->sendText("%s left the game.",player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{
  if (target!=Positions::Unknown)
    Broodwar->printf("Nuclear Launch Detected at (%d,%d)",target.x(),target.y());
  else
    Broodwar->printf("Nuclear Launch Detected");
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit* unit)
{
  //if (!Broodwar->isReplay())
    //Broodwar->sendText("A %s [%x] has been discovered at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void ExampleAIModule::onUnitEvade(BWAPI::Unit* unit)
{
  //if (!Broodwar->isReplay())
    //Broodwar->sendText("A %s [%x] was last accessible at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void ExampleAIModule::onUnitShow(BWAPI::Unit* unit)
{
  //if (!Broodwar->isReplay())
    //Broodwar->sendText("A %s [%x] has been spotted at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void ExampleAIModule::onUnitHide(BWAPI::Unit* unit)
{
  //if (!Broodwar->isReplay())
    //Broodwar->sendText("A %s [%x] was last seen at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit* unit)
{
  //if (!Broodwar->isReplay())
    //Broodwar->sendText("A %s [%x] has been created at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
  if ( Broodwar->isReplay() )
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

void ExampleAIModule::onUnitDestroy(BWAPI::Unit* unit)
{
  //if (!Broodwar->isReplay())
    //Broodwar->sendText("A %s [%x] has been destroyed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit* unit)
{
  //if (!Broodwar->isReplay())
    //Broodwar->sendText("A %s [%x] has been morphed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
  if (Broodwar->isReplay())
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

void ExampleAIModule::onUnitRenegade(BWAPI::Unit* unit)
{
  //if (!Broodwar->isReplay())
    //Broodwar->sendText("A %s [%x] is now owned by %s",unit->getType().getName().c_str(),unit,unit->getPlayer()->getName().c_str());
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
  Broodwar->printf("The game was saved to \"%s\".", gameName.c_str());
}

void ExampleAIModule::drawStats()
{
  std::set<Unit*> myUnits;
  if (Broodwar->isReplay())
    myUnits = Broodwar->getAllUnits();
  else
    myUnits = Broodwar->self()->getUnits();

  Broodwar->drawTextScreen(5,0,"I have %d units:",myUnits.size());
  std::map<UnitType, int> unitTypeCounts;
  for(std::set<Unit*>::iterator i=myUnits.begin();i!=myUnits.end();i++)
  {
    if (unitTypeCounts.find((*i)->getType())==unitTypeCounts.end())
    {
      unitTypeCounts.insert(std::make_pair((*i)->getType(),0));
    }
    unitTypeCounts.find((*i)->getType())->second++;
  }
  int line=1;
  for(std::map<UnitType,int>::iterator i=unitTypeCounts.begin();i!=unitTypeCounts.end();i++)
  {
    Broodwar->drawTextScreen(5,16*line,"- %d %ss",(*i).second, (*i).first.getName().c_str());
    line++;
  }
}

void ExampleAIModule::drawUnits()
{
  std::set<Unit*> units = Broodwar->getAllUnits();
  for(std::set<Unit*>::iterator i=units.begin();i!=units.end();i++)
  {
    Position p=(*i)->getPosition();
    Broodwar->drawCircleMap(p.x(),p.y(),30,Colors::Green,false);
  }
}

void ExampleAIModule::drawBullets()
{
  std::set<Bullet*> bullets = Broodwar->getBullets();
  for(std::set<Bullet*>::iterator i=bullets.begin();i!=bullets.end();i++)
  {
    Position p=(*i)->getPosition();
    double velocityX = (*i)->getVelocityX();
    double velocityY = (*i)->getVelocityY();
    if ((*i)->getPlayer()==Broodwar->self())
    {
      Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Green);
      Broodwar->drawTextMap(p.x(),p.y(),"\x07%s",(*i)->getType().getName().c_str());
    }
    else
    {
      Broodwar->drawLineMap(p.x(),p.y(),p.x()+(int)velocityX,p.y()+(int)velocityY,Colors::Red);
      Broodwar->drawTextMap(p.x(),p.y(),"\x06%s",(*i)->getType().getName().c_str());
    }
  }
}

void ExampleAIModule::drawVisibilityData()
{
  for(int x=0;x<Broodwar->mapWidth();x++)
  {
    for(int y=0;y<Broodwar->mapHeight();y++)
    {
      if (Broodwar->isExplored(x,y))
      {
        if (Broodwar->isVisible(x,y))
          Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Green);
        else
          Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Blue);
      }
      else
        Broodwar->drawDotMap(x*32+16,y*32+16,Colors::Red);
    }
  }
}

void ExampleAIModule::showPlayers()
{
  std::set<Player*> players=Broodwar->getPlayers();
  for(std::set<Player*>::iterator i=players.begin();i!=players.end();i++)
  {
    Broodwar->printf("Player [%d]: %s is in force: %s",(*i)->getID(),(*i)->getName().c_str(), (*i)->getForce()->getName().c_str());
  }
}

void ExampleAIModule::showForces()
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
