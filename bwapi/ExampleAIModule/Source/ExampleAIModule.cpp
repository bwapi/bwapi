#include "ExampleAIModule.h"

#include <set>
#include <vector>

using namespace BWAPI;

void ExampleAIModule::onStart()
{
  // Hello World!
  Broodwar->sendText("Hello world!");


  // Print the map name.
  // BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
  Broodwar->printf("The map is %s!", Broodwar->mapName().c_str() );


  // Enable the UserInput flag, which allows us to control the bot and type messages.
  Broodwar->enableFlag(Flag::UserInput);


  // Uncomment the following line and the bot will know about everything through the fog of war (cheat).
  //Broodwar->enableFlag(Flag::CompleteMapInformation);
  

  // Set the command optimization level so that common commands can be grouped
  // and reduce the bot's APM (Actions Per Minute).
  Broodwar->setCommandOptimizationLevel(2);


  // Check if this is a replay
  if ( Broodwar->isReplay() )
  {

    // Announce the players in the replay
    Broodwar->printf("The following players are in this replay:");
    

    // Iterate all the players in the game using a std:: iterator
    for(std::set<Player*>::iterator p = Broodwar->getPlayers().begin(); p != Broodwar->getPlayers().end(); ++p)
    {

      // Only print the player if they are not an observer
      if ( !(*p)->isObserver() )
        Broodwar->printf("%s, playing as a %s", (*p)->getName().c_str(), (*p)->getRace().c_str() ); 

    }

  }
  else // if this is not a replay
  {

    // Retrieve you and your enemy's races. enemy() will just return the first enemy.
    // If you wish to deal with multiple enemies then you must use enemies().
    if ( Broodwar->enemy() ) // First make sure there is an enemy
      Broodwar->printf("The match up is %s v %s", Broodwar->self()->getRace().c_str(), Broodwar->enemy()->getRace().c_str() );

  }

}

void ExampleAIModule::onEnd(bool isWinner)
{
  // Called when the game ends
  if ( isWinner )
  {
    // Log your win here!
  }
}

void ExampleAIModule::onFrame()
{
  // Called once every game frame


  // Display the game frame rate as text in the upper left area of the screen
  Broodwar->drawTextScreen(200, 0,  "FPS: %d", Broodwar->getFPS() );
  Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS() );


  // Return if the game is a replay or is paused
  if ( Broodwar->isReplay() || Broodwar->isPaused() )
    return;


  // Prevent spamming by only running our onFrame once every number of latency frames.
  // Latency frames are the number of frames before commands are processed.
  if ( Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0 )
    return;


  // Iterate through all the units that we own
  for ( std::set<Unit*>::const_iterator i = Broodwar->self()->getUnits().begin(); i != Broodwar->self()->getUnits().end(); ++i )
  {
    // Get a direct pointer to our unit so we do not have to continuously dereference the iterator
    Unit *u = *i;
    

    // Ignore the unit if it no longer exists
    // Make sure to include this block when handling any Unit pointer!
    if ( !u->exists() )
      continue;


    // Ignore the unit if it has one of the following status ailments
    if ( u->isLockedDown() || u->isMaelstrommed() || u->isStasised() )
      continue;


    // Ignore the unit if it is in one of the following states
    if ( u->isLoaded() || u->isUnpowered() || u->isStuck() )
      continue;

    // Ignore the unit if it is incomplete or busy constructing
    if ( !u->isCompleted() || u->isConstructing() )
      continue;

    // Finally make the unit do some stuff!


    // If the unit is a worker unit
    if ( u->getType().isWorker() )
    {

      // if our worker is idle
      if ( u->isIdle() )
      {
        // Order workers carrying a resource to return them to the center,
        // otherwise find a mineral patch to harvest.
        if ( u->isCarryingGas() || u->isCarryingMinerals() )
        {
          u->returnCargo();
        }
        else if ( !u->getPowerUp() )  // The worker cannot harvest anything if it
        {                             // is carrying a powerup such as a flag
          // Find the closest mineral patch
          // Declare some variables to help with obtaining the closest unit
          Unit *closest = NULL;
          int   closestDistance = 999999;
          for ( std::set<Unit*>::iterator m = Broodwar->getMinerals().begin(); m != Broodwar->getMinerals().end(); ++m )
          {
            int newDistance = u->getDistance(*m);

            // Continue iterating if the new distance is greater than the old distance
            if ( newDistance >= closestDistance )
              continue;

            // Save our distance information if it is closer
            closestDistance = newDistance;
            closest = *m;
          }


          // If a mineral patch was found
          if ( closest )
          {
            // Order our worker to harvest the patch!
            u->gather(closest);

            // Continue iterating other units so we don't interrupt this order and produce unnecessary commands
            continue;
          }
        } // closure: has no powerup
      } // closure: if idle

    }
    else if ( u->getType().isResourceDepot() ) // A resource depot is like a Command Center, Nexus, or Hatchery
    {

      // Order the depot to construct more workers! But only when it is idle.
      if ( u->isIdle() && !u->train(u->getType().getRace().getWorker()) )
      {

        // If that fails, draw the error at the location so that you can visibly see what went wrong!
        Broodwar->drawTextMap( u->getPosition().x(), u->getPosition().y(), "%s", Broodwar->getLastError().c_str() );
      }

    }

  } // closure: unit iterator
}

void ExampleAIModule::onSendText(std::string text)
{

  // Send the text to the game if it is not being processed.
  Broodwar->sendText("%s", text.c_str());


  // Make sure to use %s and pass the text as a parameter,
  // otherwise you may run into problems when you use the %(percent) character!

}

void ExampleAIModule::onReceiveText(BWAPI::Player* player, std::string text)
{
  // Parse the received text
  Broodwar->printf("%s said '%s'", player->getName().c_str(), text.c_str());
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player* player)
{
  // Interact verbally with the other players in the game by
  // announcing that the other player has left.
  Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

  // Check if the target is a valid position
  if ( target )
  {

    // if so, print the location of the nuclear strike target
    Broodwar->printf("Nuclear Launch Detected at (%d,%d)", target.x(), target.y() );

  }
  else 
  {

    // Otherwise, ask other players where the nuke is!
    Broodwar->sendText("Where's the nuke?");

  }

  // You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!

}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit* unit)
{/*
  if ( !Broodwar->isReplay() )
    Broodwar->sendText("A %s [%x] has been discovered at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
*/}

void ExampleAIModule::onUnitEvade(BWAPI::Unit* unit)
{/*
  if ( !Broodwar->isReplay() )
    Broodwar->sendText("A %s [%x] was last accessible at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
*/}

void ExampleAIModule::onUnitShow(BWAPI::Unit* unit)
{/*
  if ( !Broodwar->isReplay() )
    Broodwar->sendText("A %s [%x] has been spotted at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
*/}

void ExampleAIModule::onUnitHide(BWAPI::Unit* unit)
{/*
  if ( !Broodwar->isReplay() )
    Broodwar->sendText("A %s [%x] was last seen at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
*/}

void ExampleAIModule::onUnitCreate(BWAPI::Unit* unit)
{/*
  if ( !Broodwar->isReplay() )
    Broodwar->sendText("A %s [%x] has been created at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
  else*/
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
{/*
  if ( !Broodwar->isReplay() )
    Broodwar->sendText("A %s [%x] has been destroyed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
*/}

void ExampleAIModule::onUnitMorph(BWAPI::Unit* unit)
{/*
  if ( !Broodwar->isReplay() )
    Broodwar->sendText("A %s [%x] has been morphed at (%d,%d)",unit->getType().getName().c_str(),unit,unit->getPosition().x(),unit->getPosition().y());
  else*/
  if ( Broodwar->isReplay() )
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
{/*
  if (!Broodwar->isReplay())
    Broodwar->sendText("A %s [%x] is now owned by %s",unit->getType().getName().c_str(),unit,unit->getPlayer()->getName().c_str());
*/}

void ExampleAIModule::onSaveGame(std::string gameName)
{
  Broodwar->printf("The game was saved to \"%s\".", gameName.c_str());
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit *unit)
{/*
  if ( Broodwar->self() == unit->getPlayer() )
    Broodwar->printf("A %s has just finished constructing.", unit->getType().getName().c_str());
*/}
