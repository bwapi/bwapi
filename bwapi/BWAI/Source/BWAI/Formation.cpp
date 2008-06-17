#include "Formation.h"

#include <math.h>

#include <BWAPI/ScreenLogger.h>
#include <Pathfinding/Utilities.h>
#include <BWAI/Globals.h>
#include <BWAI/Unit.h>

namespace BWAI
{
  //--------------------------------------------- CONSTRUCTOR ------------------------------------------------
  Formation::Formation(std::list<Unit*>& units)
  {
    for (std::list<Unit*>::iterator i = units.begin(); i != units.end(); ++i)
    {
      std::map<BW::UnitID::Enum, std::list<Target> >::iterator index;
      index = this->data.find((*i)->getType().getID());
      if (index == this->data.end())
      {
        this->data.insert(std::pair<BW::UnitID::Enum, std::list<Target> >
                         ((*i)->getType().getID(), std::list<Target>()));
        index = this->data.find((*i)->getType().getID());
      }
      (*index).second.push_back(Target(*i, BW::Position(0,0)));
    }
    for (std::map<BW::UnitID::Enum, std::list<Target> >::iterator i = this->data.begin();
         i != this->data.end();
         ++i)
    {
      BWAPI::ScreenLogger().log("%d %ss", (*i).second.size(), BW::UnitType((*i).first).getName());
    }
  }
  //---------------------------------------- GENERATE POSITIONS ----------------------------------------------
  void Formation::generatePositions(BW::Position center, float angle)
  {
    std::map<BW::UnitID::Enum, std::list<Target> >::iterator index;
    index = this->data.find(BW::UnitID::Terran_Marine);
    int lineLength = 9;
    int space = 40;
    center.x -= (int) sin(angle)*(lineLength/2)*space;
    center.y -= (int) cos(angle)*(lineLength/2)*space;
    if (index != this->data.end())
    {
      std::list<Target>* list = &(*index).second;
      int index = 0, verticalIndex = 0;
      for (std::list<Target>::iterator i = list->begin(); i != list->end(); index++)
      {
        BW::Position target = BW::Position(center.x + (int)(sin(angle)*index*space + cos(angle)*verticalIndex*space),
                                           center.y + (int)(cos(angle)*index*space + sin(angle)*verticalIndex*space));
        if (BWAI::ai->pathFinding->canStay((*i).unit->getType(), target))
        {
          (*i).target = target;
           ++i;
        }
        if (index == lineLength)
        {
          verticalIndex ++;
          index = -1;
        }        
      }
    }
  }
  //--------------------------------------------- EXECUTE ----------------------------------------------------
  void Formation::execute()
  {
    std::map<BW::UnitID::Enum, std::list<Target> >::iterator index;
    index = this->data.find(BW::UnitID::Terran_Marine);
    if (index != this->data.end())
    {
      std::list<Target>* list = &(*index).second;
      for (std::list<Target>::iterator i = list->begin(); i != list->end(); ++i)
        (*i).unit->orderRightClick((*i).target);
    }
  }
}