#include "Formation.h"

#include <math.h>

#include <Pathfinding/Utilities.h>
#include <BWAI/Globals.h>
#include <BWAI/Unit.h>

namespace BWAI
{
  //---------------------------------------------- CONSTRUCTOR -----------------------------------------------
  Formation::Formation(std::list<Unit*>& units)
  {
    for each (Unit* i in units)
    {
      std::map<BWAPI::UnitType, std::list<Target> >::iterator index;
      index = this->data.find(i->getType().getID());
      if (index == this->data.end())
      {
        this->data.insert(std::pair<BWAPI::UnitType, std::list<Target> >
                         (i->getType(), std::list<Target>()));
        index = this->data.find(i->getType().getID());
      }
      (*index).second.push_back(Target(i, BWAPI::Position(0,0)));
    }
 /*   for each (std::pair<BW::UnitID::Enum, std::list<Target> > i in this->data)
      BWAPI::ScreenLogger().log("%d %ss", i.second.size(), BW::UnitType(i.first).getName());*/
  }
  //------------------------------------------- GENERATE POSITIONS -------------------------------------------
  void Formation::generatePositions(BWAPI::Position center, float angle)
  {
    std::map<BWAPI::UnitType, std::list<Target> >::iterator index;
    index = this->data.find(BWAPI::UnitTypes::Terran_Marine);
    int lineLength = 16;
    int space = 32;
    center.x() -= (int) (sin(angle)*(lineLength/2)*space);
    center.y() -= (int) (cos(angle)*(lineLength/2)*space);
    if (index != this->data.end())
    {
      std::list<Target>* list = &(*index).second;
      int index = 0, verticalIndex = 0;
      for (std::list<Target>::iterator i = list->begin(); i != list->end(); index++)
      {
        BWAPI::Position target = BWAPI::Position(center.x() + (int)((sin(angle)*index*space + sin(angle - 3.14159/2)*verticalIndex*space)),
                                                 center.y() + (int)((cos(angle)*index*space + cos(angle - 3.14159/2)*verticalIndex*space)));
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
  //------------------------------------------------ EXECUTE -------------------------------------------------
  void Formation::execute()
  {
    std::map<BWAPI::UnitType, std::list<Target> >::iterator index;
    index = this->data.find(BWAPI::UnitTypes::Terran_Marine);
    if (index != this->data.end())
    {
      std::list<Target>* list = &(*index).second;
      for each (Target i in *list)
        if (i.unit->isIdle())
          i.unit->orderAttackMove(i.target);
    }
  }
}